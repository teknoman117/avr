
/****************************************************************************
 Title  :   C  test file for the SRF08 FUNCTIONS library (test_srf08.c)
 Author:    Chris efstathiou hendrix@otenet.gr
 Date:      13/Jul/2002
 Software:  AVR-GCC with AVR-AS
 Target:    any AVR device
 Comments:  This software is FREE.

*****************************************************************************/

#define  F_CPU   3686400L

#include <avr/io.h>
#include "lcd_io.h"
#include "srf08.h"

#define AVG_FLT_SAMPLES   3

/*#################################################################################################*/
void delay(unsigned long us)
{

   while ( us ) { us--; }  /* 6 cpu cycles per loop */
 
}
/*#################################################################################################*/

void main(void)
{
unsigned int range=0;
unsigned char counter1=0, gain=0;

/* Delay is there to ensure that SRF08 is out of reset and operational */
/* 
delay(1000000);
srf08_change_i2c_address(SRF08_UNIT_1); 
delay(100000);
*/

/*
   By default SRF_UNIT_0 is selected so the below command shows that the change of i2c address
   by the above "srf08_change_i2c_address(SRF08_UNIT_1);" command was succesfull.
   All commands after this, refer to that unit only! 
*/
srf08_select_unit(SRF08_UNIT_1);   

srf08_init();                      /* Only the selected SRF08 unit will be initialized!     */  
gain=SRF08_MAX_GAIN;
srf08_set_gain(gain);
srf08_set_range(SRF08_MAX_RANGE);   /* Set range to 11008 mm */ 

lcd_init();
lcd_clrscr();
lcd_gotoxy(0,0); lcd_puts_P("RANGE (E1)  = ");
lcd_gotoxy(0,1); lcd_puts_P("RANGE (E2)  = ");
lcd_gotoxy(0,2); lcd_puts_P("RANGE (E5)  = ");
lcd_gotoxy(0,3); lcd_puts_P("LIGHT sensor= ");

while(1)
    {
       
       /* AVERAGING FILTER */
       for(counter1=0,range=0; counter1<AVG_FLT_SAMPLES; counter1++)
         {
            range+=srf08_ping(SRF08_CENTIMETERS);
         } 
       range /= AVG_FLT_SAMPLES;

       /* AUTOMATIC GAIN CONTROL */
       if(srf08_read_register(SRF08_ECHO_5)!=0 )
        { 
           if(gain>=5) { srf08_set_gain(gain-=5); } else { srf08_set_gain(gain=0); }  
        } 
       else if(srf08_read_register(SRF08_ECHO_2)<=0 && gain!=31) { srf08_set_gain(++gain); } 

       /* DISPLAY TO LCD */
       lcd_gotoxy(14,0); lcd_puti(range,2);
       lcd_gotoxy(14,1); lcd_puti(srf08_read_register(SRF08_ECHO_2), 2);
       lcd_gotoxy(14,2); lcd_puti(srf08_read_register(SRF08_ECHO_5), 2);
       lcd_gotoxy(14,3); lcd_puti(srf08_read_register(SRF08_LIGHT),  2);


    }

return;
}
/*#################################################################################################*/
       

