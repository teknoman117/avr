
/****************************************************************************
 Title  :   C  file for the SRF08 FUNCTIONS library (srf08.c)
 Author:    Chris efstathiou hendrix@otenet.gr
 Date:      13/Jul/2002
 Software:  AVR-GCC with AVR-AS
 Target:    any AVR device
 Comments:  This software is FREE.

*****************************************************************************/

#include <avr/io.h>
#include <avr/eeprom.h>
#include "i2c.h"
#include "srf08.h"

/* Global Variables */
static unsigned char address=SRF08_UNIT_0;

/*#################################################################################################*/

void srf08_init(void)
{
unsigned int  range=0;
        i2c_init();
        I2C_START_TX(address);
        i2c_transmit(0);                    
        i2c_transmit(0x51);
       
        do{
            i2c_start();
            range=i2c_transmit(address);               
            i2c_stop();
          }while(range != I2C_NO_ERROR);                               
                                

return;
}
/*#################################################################################################*/

void srf08_set_gain(unsigned char gain)
{
    if(gain>31) { gain=31; }
    I2C_START_TX(address);         
    i2c_transmit(1);                               
    i2c_transmit(gain);
    i2c_stop();                            

return;
}
/*#################################################################################################*/

void srf08_set_range(unsigned int millimeters)
{
    millimeters= (millimeters/43); 
    if(millimeters > 0xff ) { millimeters=0xff; }
    I2C_START_TX(address);         
    i2c_transmit(2);                               
    i2c_transmit(millimeters);
    i2c_stop();                            

return;
}
/*#################################################################################################*/

unsigned int srf08_ping(unsigned char metric_unit)
{
     union i2c_union {
                              unsigned int  rx_word; 
                              unsigned char rx_byte[2];
                     } i2c;

       
        I2C_START_TX(address);
        i2c_transmit(0);                    
        i2c_transmit(metric_unit);
       
        do{
            i2c_start();
            i2c.rx_byte[0]=i2c_transmit(address);               
            i2c_stop();
          }while(i2c.rx_byte[0] != I2C_NO_ERROR);
       

        I2C_START_TX(address);
        i2c_transmit(SRF08_ECHO_1);
        I2C_START_RX(address);
        i2c.rx_byte[1]=i2c_receive(I2C_CONTINUE);  /* get high byte msb first */                         
        i2c.rx_byte[0]=i2c_receive(I2C_QUIT);      /* get low byte msb first  */                     
        i2c_stop();

return(i2c.rx_word);
}
/*#################################################################################################*/

unsigned int srf08_read_register(unsigned char srf08_register)
{
union i2c_union {
                   unsigned int  rx_word; 
                   unsigned char rx_byte[2];
                } i2c;


        I2C_START_TX(address);
        i2c_transmit(srf08_register);
        I2C_START_RX(address);
       
        /* get high byte msb first */ 
        if(srf08_register>=2) { i2c.rx_byte[1]=i2c_receive(I2C_CONTINUE); }                         
       
        /* get low byte msb first  */ 
        i2c.rx_byte[0]=i2c_receive(I2C_QUIT);                          

        i2c_stop();

return(i2c.rx_word);
}
/*#################################################################################################*/

void srf08_change_i2c_address(unsigned char new_i2c_address)
{


    /* Start the I2C address changing procedure */
    I2C_START_TX(address);         
    i2c_transmit(SRF08_COMMAND); 
    i2c_transmit(0XA0); 
    i2c_stop(); 
    
    I2C_START_TX(address);         
    i2c_transmit(SRF08_COMMAND); 
    i2c_transmit(0XAA);                                
    i2c_stop();

    I2C_START_TX(address);         
    i2c_transmit(SRF08_COMMAND); 
    i2c_transmit(0XA5);
    i2c_stop();

    I2C_START_TX(address);         
    i2c_transmit(SRF08_COMMAND); 
    i2c_transmit(new_i2c_address); 
    i2c_stop(); 

    /* Make the new i2c address the active one. */
    address=new_i2c_address;                           

return;
}
/*#################################################################################################*/

void srf08_select_unit(unsigned char srf08_address)
{
    /* New address validity check */

    if( (srf08_address<0xE0 || srf08_address>0XFE) && srf08_address != 0 )  { return; }
    if(srf08_address%2) { return; }

    /* Make the new i2c address the active one. */
    address=srf08_address;                           

return;
}


/*######################################################################################################*/
/*                                         T H E   E N D                                                */
/*######################################################################################################*/

