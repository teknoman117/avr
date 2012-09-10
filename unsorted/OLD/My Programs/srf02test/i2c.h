
/****************************************************************************
 Title	:   C  include file for the I2C FUNCTIONS library (i2c.h)
 Author:    Chris efstathiou hendrix@otenet.gr
 Date:	    13/Jul/2002
 Software:  AVR-GCC with AVR-AS
 Target:    any AVR device
 Comments:  This software is FREE.

*****************************************************************************/


#ifndef I2C_H
#define I2C_H	1

/*##############################################################################################*/
/*			       START OF CONFIGURATION BLOCK					*/
/*##############################################################################################*/

#ifndef  F_CPU
#define  F_CPU		  11059200UL  /* The cpu clock frequency in Hertz (used to calculate time)*/
#endif

#define I2C_SDA_PORT	  C	    /* The SDA port. Use capital letter (A,B,C,D... etc.) */
#define SDA_PIN 	  0	    /* The SDA port pin */

#define I2C_SCL_PORT	  C	    /* The SCL port. Use capital letter (A,B,C,D... etc.) */
#define SCL_PIN 	  1	    /* The SCL port pin */
/*
   The I2C_DELAY_TIME normally is 5 microseconds for a 100 Khz I2C bus ( (1/100000)/2 ).
   but due to bus capacitance and device responce time in my application i use 50 microseconds.
   The I2C_TIMEOUT_TIME is set to whatever is the maximum time you think your device will take
   to perform the requested task. After that time the i2c_transmit function will return
   a "I2C_ERROR_DEVICE_NOT_RESPONDING" or "I2C_ERROR_DEVICE_BUSY" error code. 
*/ 
#define I2C_DELAY_TIME	  50	   /* in microseconds (max over 1 second) */
#define I2C_TIMEOUT_TIME  1000	   /* in microseconds (max over 1 second) */

/*##############################################################################################*/
/*				END OF CONFIGURATION BLOCK					*/
/*##############################################################################################*/

/* Keyword definitions */

#define I2C_READ	1
#define I2C_WRITE	0

#define I2C_QUIT	0
#define I2C_CONTINUE	1

#define I2C_NO_ERROR			 0
#define I2C_ERROR_DEVICE_BUSY		 1
#define I2C_ERROR_DEVICE_NOT_RESPONDING  2


/* Function Declaration */

extern	void	      i2c_init(void);
extern	void	      i2c_start(void);
extern	void	      i2c_stop(void);
extern	unsigned char i2c_transmit(unsigned char data);
extern	unsigned char i2c_receive(unsigned char ack);



/* Macro definitions */

#define I2C_START(ADDRESS)     { i2c_start(); i2c_transmit(ADDRESS); }
#define I2C_START_TX(ADDRESS)  I2C_START(ADDRESS)
#define I2C_START_RX(ADDRESS)  I2C_START(ADDRESS | I2C_READ)

#endif	/*  #ifndef I2C_H */
/*######################################################################################################*/
/*					   T H E   E N D						*/
/*######################################################################################################*/

