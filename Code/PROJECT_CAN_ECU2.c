
#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 20000000

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

// ================= CLCD =================
#define CLCD_PORT PORTD
#define CLCD_EN   RC2
#define CLCD_RS   RC1
#define CLCD_RW   RC0
#define CLCD_BUSY RD7
#define PORT_DIR  TRISD7

#define HI 1
#define LO 0

#define DATA_COMMAND 1
#define INSTRUCTION_COMMAND 0

#define LINE1(x) (0x80 + x)
#define LINE2(x) (0xC0 + x)

void clcd_write(unsigned char byte, unsigned char control_bit)
{
    CLCD_RS = control_bit;
    CLCD_PORT = byte;

    CLCD_EN = HI;
    CLCD_EN = LO;

    PORT_DIR = 1;
    CLCD_RW = HI;
    CLCD_RS = 0;

    while(CLCD_BUSY)
    {
        CLCD_EN = HI;
        CLCD_EN = LO;
    }

    CLCD_RW = LO;
    PORT_DIR = 0;
}



void init_clcd()
{
    TRISD = 0x00;
    TRISC &= 0xF8;

    CLCD_RW = LO;

    __delay_ms(30);

    clcd_write(0x38, 0);
    clcd_write(0x0C, 0);
    clcd_write(0x01, 0);
}

void clcd_print(const char *data, unsigned char addr)
{
    clcd_write(addr, 0);
    while(*data)
        clcd_write(*data++, 1);

}

void clcd_putch(const unsigned char data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);
	clcd_write(data, DATA_COMMAND);
}

// ================= SSD =================
// Move SSD to PORTB
char digit[] = {0xE7,0x21,0xCB,0x6B,0x2D,0x6E,0xEE,0x23,0xEF,0x6F};

void display_ssd(uint16_t rpm)
{
    PORTB = digit[rpm / 1000];
    __delay_ms(2);

    PORTB = digit[(rpm / 100) % 10];
    __delay_ms(2);

    PORTB = digit[(rpm / 10) % 10];
    __delay_ms(2);

    PORTB = digit[rpm % 10];
    __delay_ms(2);
}

// ================= ADC =================
void init_adc()
{
    ADCON0 = 0x01;
    ADCON1 = 0x00;
    ADCON2 = 0xA4;
}

unsigned short read_adc(unsigned char ch)
{
    ADCON0 &= 0xC3;
    ADCON0 |= (ch << 2);

    __delay_us(5);

    GO = 1;
    while(GO);

    return ((ADRESH << 8) | ADRESL);
}

uint16_t get_speed()
{
    return (read_adc(4) / 10.20);
}

// ================= CAN =================
#define SPEED_MSG_ID 0x10
#define GEAR_MSG_ID 0x20

void set_msg_id_std(unsigned int id)
{
    TXB0SIDL = (id & 0x07) << 5;
    TXB0SIDH = id >> 3;
}

uint16_t get_msg_id_std()
{
    return ((RXB0SIDH << 3) | (RXB0SIDL >> 5));
}

void init_can()
{
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 1;

    CANCON = 0x80; //setting to config
    while((CANSTAT & 0xE0) != 0x80);

    BRGCON1 = 0x00;
    BRGCON2 = 0x90;
    BRGCON3 = 0x02;

    RXB0CON = 0x60;

    CANCON = 0x00; //setting to normal operation
    while((CANSTAT & 0xE0) != 0x40);
}

void can_transmit(uint16_t id, uint8_t *data, uint8_t len)
{
    while(TXB0CONbits.TXREQ);

    set_msg_id_std(id);

    TXB0DLC = len;

    uint8_t *ptr = (uint8_t*)&TXB0D0;

    for(int i=0;i<len;i++)
        ptr[i] = data[i];

    TXB0CONbits.TXREQ = 1;
}

void can_receive(uint16_t *id, uint8_t *data, uint8_t *len)
{
    if(RXB0CONbits.RXFUL)
    {
        *id = get_msg_id_std();
        *len = RXB0DLC;

        uint8_t *ptr = (uint8_t*)&RXB0D0;

        for(int i=0;i<*len;i++)
            data[i] = ptr[i];

        RXB0CONbits.RXFUL = 0;
    }
    else
        *len = 0;
}

// ================= MAIN =================
void main()
{
    char key_indi = 0;
    TRISB2 = 0; /* CAN TX */
    TRISB3 = 1;
  //  TRISA = 0x00;

    init_adc();
    init_can();
    init_clcd();   // ? IMPORTANT

    uint16_t speed;
    char tx[4];
    char rx[4];
    
    uint16_t gear;
    char gear_tx[2];
    char gear_rx[2];

    uint16_t id;
    uint8_t len;
    
    uint16_t gear_id;
    uint8_t gear_len;

    while(1)
    {
        clcd_write(0x01, 0); // clear
        speed = get_speed();

        
//        display_ssd(rpm); // SSD display

        tx[0] = (speed / 100) + '0';
        tx[1] = (speed / 10) > 9 ? '0' : + (speed / 10) + '0';
        //tx[2] = ((rpm / 10) % 10) + '0';
        tx[2] = (speed % 10) + '0';
        tx[3] = '\0';
        
//        clcd_putch(rx[0],LINE1(0));
//        clcd_putch(rx[1],LINE1(1));
//        clcd_putch(rx[2],LINE1(2));
//        clcd_putch(rx[3],LINE1(3));
        
//        char key = PORTC & 0x0f;
//        if(key == 0x0e)
//        {
//            ind_tx[0] = 'l';
//            ind_tx[1] = '\0';  
//        }
//        else if(key == 0x0d)
//        {
//            ind_tx[0] = 'r';
//            ind_tx[1] = '\0';
//        }
//        else
//        {
//            ind_tx[0] = 0;
//            ind_tx[1] = '\0';
//        }
//        ind_tx[0] = 'l';
//        ind_tx[1] = '\0';  
        
        gear_tx[0] = 'N';
        gear_tx[1] = '\0';

        can_transmit(SPEED_MSG_ID, (uint8_t*)tx, 4);

        __delay_ms(80);

//        can_receive(&id, (uint8_t*)rx, &len);
//
//        if(len == 4)
//        {
//            rx[3] = '\0';
//
//             
//            //clcd_print("RPM:", LINE1(0));
//           // clcd_print(rx, LINE2(0));
//            clcd_putch('-',LINE2(3));
//            clcd_putch(rx[0],LINE2(4));
//            clcd_putch(rx[1],LINE2(5));
//            clcd_putch(rx[2],LINE2(6));
//            clcd_putch('-',LINE2(7));
//        }
        
        
        
        can_transmit(GEAR_MSG_ID, (uint8_t*)gear_tx, 2);

        __delay_ms(80);

     //   can_receive(&gear_id, (uint8_t*)gear_rx, &len);
      //  for(int i = 0; i < 30000; i++);
        
       // clcd_putch('G',LINE1(0));
        //clcd_putch('-',LINE1(1));
        //clcd_putch(gear_rx[0],LINE1(3));
        
        
    }
}