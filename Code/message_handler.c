#include <xc.h>
#include <string.h>
#include "message_handler.h"
#include "msg_id.h"
#include "can.h"
#include "clcd.h"
int msg_id;
char data[6];
char len;
unsigned int count = 0;
volatile unsigned char led_state = LED_OFF, status = e_ind_off;
void __interrupt() isr()
{
    if(TMR0IF)
    {
        TMR0 = TMR0 + 8;
        if(count++ == 20000)
        {
            count = 0;
            if(status == e_ind_left)
            {
                PORTBbits.RB0 = !PORTBbits.RB0;
                PORTBbits.RB1 = !PORTBbits.RB1;
            }
            else if(status  == e_ind_right)
            {
                PORTBbits.RB6 = !PORTBbits.RB6;
                PORTBbits.RB7 = !PORTBbits.RB7;
            }
            else if(status  == e_ind_off)
            {
                PORTBbits.RB0 = 0;
                PORTBbits.RB1 = 0;
                PORTBbits.RB6 = 0;
                PORTBbits.RB7 = 0;
            }
        }
        TMR0IF = 0;
    }
}



void handle_speed_data(uint8_t *data, uint8_t len)
{
    //Implement the speed function
      clcd_print("SP",LINE1(0));
    clcd_print(data, LINE2(0));
}

void handle_gear_data(uint8_t *data, uint8_t len) 
{
    //Implement the gear function
  
   clcd_print("GR",LINE1(4));
    clcd_print(data,LINE2(4));
}

void handle_rpm_data(uint8_t *data, uint8_t len) 
{
       clcd_print("RPM",LINE1(8));
    clcd_print(data,LINE2(8));
    
}

/*void handle_engine_temp_data(uint8_t *data, uint8_t len) 
{
    //Implement the temperature function
} */

void handle_indicator_data(uint8_t *data, uint8_t len) 
{
    clcd_print("IND",LINE1(12));
    if(data[0] == e_ind_left)
    {
         status = e_ind_left;
        clcd_print("<--",LINE2(12));
    }
    else if(data[0] == e_ind_right)
    {
        status = e_ind_right;
        clcd_print("-->",LINE2(12));
    }
    else if(data[0] == e_ind_off)
    {
        status = e_ind_off;
        clcd_print("---",LINE2(12));
    }
    
}

void process_canbus_data() 
{   
   can_receive(&msg_id,data,&len);
   data[len]='\0';
   if(msg_id==SPEED_MSG_ID)
   {
      handle_speed_data(data,len);  
   }
   else if(msg_id==GEAR_MSG_ID)
   {
     handle_gear_data(data,len);  
   }
   
   else if(msg_id==RPM_MSG_ID)
   {
       handle_rpm_data(data,len);
   }
   else if(msg_id==INDICATOR_MSG_ID)
   {
       handle_indicator_data(data,len);
   }
    
}