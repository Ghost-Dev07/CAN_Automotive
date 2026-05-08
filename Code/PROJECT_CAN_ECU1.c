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

uint16_t get_rpm()
{
    return (read_adc(4) / 10.23) * 60;
}

// ================= CAN =================
#define RPM_MSG_ID 0x30
#define INDICATOR_MSG_ID 0x50

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

    CANCON = 0x80;
    while((CANSTAT & 0xE0) != 0x80);

    BRGCON1 = 0x00;
    BRGCON2 = 0x90;
    BRGCON3 = 0x02;

    RXB0CON = 0x60;

    CANCON = 0x00; 
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

    uint16_t rpm;
    char tx[5];
    char rx[5];
    
    uint16_t indicator;
    char ind_tx[3];
    char ind_rx[3];

    uint16_t id;
    uint8_t len;
    
    uint16_t ind_id;
    uint8_t ind_len;

    while(1)
    {
        clcd_write(0x01, 0); // clear
        rpm = get_rpm();

        
//        display_ssd(rpm); // SSD display

        tx[0] = (rpm / 1000) + '0';
        tx[1] = ((rpm / 100) % 10) + '0';
        tx[2] = ((rpm / 10) % 10) + '0';
        tx[3] = (rpm % 10) + '0';
        tx[4] = '\0';
        
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
        ind_tx[0] = 'l';
        ind_tx[1] = '\0';  

        can_transmit(RPM_MSG_ID, (uint8_t*)tx, 5);

        __delay_ms(80);

        
        
        
        can_transmit(INDICATOR_MSG_ID, (uint8_t*)ind_tx, 3);

        __delay_ms(80);

    }
}