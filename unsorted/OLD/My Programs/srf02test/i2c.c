
/****************************************************************************
 Title	:   C  file for the I2C FUNCTIONS library (i2c.c)
 Author:    Chris efstathiou hendrix@otenet.gr
 Date:	    13/Jul/2002
 Software:  AVR-GCC with AVR-AS
 Target:    any AVR device
 Comments:  This software is FREE.

*****************************************************************************/

#include <avr/io.h>
#include "i2c.h"

#ifndef CONCAT1
#define CONCAT1(a, b) CONCAT2(a, b)
#endif

#ifndef CONCAT2
#define CONCAT2(a, b) a ## b
#endif


/* Conversion of microseconds to the right value for the delay function */
#define I2C_DELAY	   ( (I2C_DELAY_TIME*(F_CPU/60000))/100 )  
#define I2C_TIMEOUT	   ( (I2C_TIMEOUT_TIME*(F_CPU/60000))/100 ) 

/* Register name forming */
#define I2C_SDA_OUT_REG   CONCAT1(PORT, I2C_SDA_PORT)
#define I2C_SCL_OUT_REG   CONCAT1(PORT, I2C_SCL_PORT)
#define I2C_SDA_DDR_REG   CONCAT1(DDR, I2C_SDA_PORT)
#define I2C_SCL_DDR_REG   CONCAT1(DDR, I2C_SCL_PORT)
#define I2C_SDA_PIN_REG   CONCAT1(PIN, I2C_SDA_PORT)
#define I2C_SCL_PIN_REG   CONCAT1(PIN, I2C_SCL_PORT)

/* Conversion of microseconds to the right value for the delay function */
#define I2C_DELAY	  ( (I2C_DELAY_TIME*(F_CPU/60000))/100 )  
#define I2C_TIMEOUT	  ( (I2C_TIMEOUT_TIME*(F_CPU/60000))/100 ) 

/* Pin states */
#define SCL_1() 	  cbi(I2C_SCL_DDR_REG, SCL_PIN)
#define SCL_0() 	  sbi(I2C_SCL_DDR_REG, SCL_PIN)
#define SDA_1() 	  cbi(I2C_SDA_DDR_REG, SDA_PIN)
#define SDA_0() 	  sbi(I2C_SDA_DDR_REG, SDA_PIN)

#define RELEASE_I2C_BUS() { SCL_1(); SDA_1(); }

/*#################################################################################################*/

static void delay(unsigned long us)
{

   while ( us ) { us--; }  /* 6 cpu cycles per loop */
 
}
/*#################################################################################################*/

void i2c_init(void)
{
	cbi(I2C_SDA_OUT_REG, SDA_PIN);
	cbi(I2C_SCL_OUT_REG, SCL_PIN);
	RELEASE_I2C_BUS();
	delay(I2C_TIMEOUT);
	i2c_start();
	delay(I2C_TIMEOUT);
	i2c_stop();
	delay(I2C_TIMEOUT);


return;
}
/*#################################################################################################*/

void i2c_start(void)
{
	RELEASE_I2C_BUS();
	delay(I2C_DELAY);
	SDA_0();
	delay(I2C_DELAY);
	SCL_0();
	delay(I2C_DELAY);

return;
}
/*#################################################################################################*/

void i2c_stop(void)
{
	SDA_0();
	SCL_1();
	delay(I2C_DELAY);
	SDA_1();
	delay(I2C_DELAY);
	SCL_0();
	delay(I2C_DELAY);

return;
}
/*#################################################################################################*/

unsigned char i2c_transmit(unsigned char data)
{
register unsigned char bit=0;

	for(bit=0; bit<=7; bit++)
	  {
	      if( data & 0x80 ) { SDA_1(); } else { SDA_0(); }
	      SCL_1();
	      delay(I2C_DELAY);
	      SCL_0();
	      delay(I2C_DELAY);
	      data = (data<<1);
	  }
	/* Look for AKNOWLEDGE */
	RELEASE_I2C_BUS();
	delay(I2C_DELAY);


	if( bit_is_clear(I2C_SDA_PIN_REG, SDA_PIN) )
	 {
	     SCL_0();
	     delay(I2C_DELAY);
	 }
	else{
		 delay(I2C_TIMEOUT);
		 if( bit_is_clear(I2C_SDA_PIN_REG, SDA_PIN) )
		  {
		     SCL_0();
		     delay(I2C_DELAY);
		  }
		 else { return(I2C_ERROR_DEVICE_NOT_RESPONDING); }
	    }


	if( bit_is_clear(I2C_SDA_PIN_REG, SDA_PIN) ) 
	 { 
	       delay(I2C_TIMEOUT);
	       if( bit_is_clear(I2C_SDA_PIN_REG, SDA_PIN) ) { return(I2C_ERROR_DEVICE_BUSY); }
	 }   


return(I2C_NO_ERROR);	  
}
/*#################################################################################################*/

unsigned char i2c_receive(unsigned char ack)
{
register unsigned char bit=0, data=0;

	SDA_1();
	for(bit=0; bit<=7; bit++)
	  {
	      SCL_1();
	      delay(I2C_DELAY);
	      data = (data<<1);
	      if( bit_is_set(I2C_SDA_PIN_REG, SDA_PIN) ) { data++; }
	      SCL_0();
	      delay(I2C_DELAY);
	  }
	
	/* if CONTINUE then send AKNOWLEDGE else if QUIT do not send AKNOWLEDGE (send Nack) */	     
	if(ack==I2C_CONTINUE) { SDA_0(); }  else { SDA_1(); }
	SCL_1();
	delay(I2C_DELAY);
	SCL_0();
	delay(I2C_DELAY);

return data;
}
/*#################################################################################################*/


