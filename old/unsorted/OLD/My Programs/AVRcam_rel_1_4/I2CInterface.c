/*
    Copyright (C) 2004    John Orlando
    
   AVRcam: a small real-time image processing engine.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

   For more information on the AVRcam, please contact:

   john@jrobot.net

   or go to www.jrobot.net for more details regarding the system.
*/
/***********************************************************
	Module Name: I2CInterface.c
	Module Date: 4/10/2004
	Module Auth: John Orlando
	
	Description: This module is responsible for providing a
	low-level interface to the I2C hardware resident on the
	mega8 processor (also known as the Two-Wire Interface,
	or TWI).  The interface is needed to configure the
	needed registers in the OV6620 camera.  This interface
	is interrupt-driven based on the events that should
	occur upon successful writing of an I2C register.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.

***********************************************************/

/*	Includes */
#include <avr/io.h>
#include <avr/twi.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "CamConfig.h"
#include "CommonDefs.h"

/*  Local Variables */

/* These variables are used as storage space for the current
   I2C command being sent over the interface.  They need to
   be volatile since they are dealt with an the TWI ISR */
volatile static unsigned char twi_address;
volatile static unsigned char *twi_data;
volatile static unsigned char twi_ddr;
volatile static unsigned char twi_bytes;
volatile static unsigned char status;
volatile static unsigned char retry_cnt;

/* 	Local Structures and Typedefs */

/*  Extern Variables */

/*  Definitions */
/* Bit definitions for the tw_status register */
#define MAX_TWI_RETRIES 2
#define BUSY 7

/***********************************************************
	Function Name: I2CInt_init
	Function Description: This function is responsible
	for setting up the registers needed for the TWI 
	interface
	
	Inputs:  none
	Outputs: none
***********************************************************/	
void I2CInt_init(void)
{
	TWSR = 0;
    
	/* init the speed of the I2C interface, running at
    100 Kbps */
	TWBR = (FOSC / I2C_SPEED - 16)/2;
}

/***********************************************************
	Function Name: I2CInt_writeData
	Function Description: This function is responsible for
	initiating the process of writing a sequence of bytes
	an I2C slave address.  This function will try to write
	the data three times before giving up.
	Inputs: address: the address of the I2C slave device
			data: a pointer to the data to be written 
				  to the slave...for camera interfacing,
				  the data follows a <register #><data>
				  format
			bytes: the number of bytes to write 
	Outputs: none
***********************************************************/
void I2CInt_writeData(unsigned char address, unsigned char *data, unsigned char bytes)
{
	while(status & (1<<BUSY));		/* Bus is busy wait (or exit with error code) */
	while(TWCR & (1<<TWSTO));
	
	/* copy the needed data and state info to our local I2C command structure */
	twi_address = address;
	twi_data = data;
	twi_bytes = bytes;
	twi_ddr = TW_WRITE;

	retry_cnt = 0;
	
	/* Generate start condition, the remainder of the transfer is interrupt driven and
	   will be performed in the background */
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
	
	status |= (1<<BUSY);
}

/***********************************************************
	Function Name: I2CInt_readData
	Function Description: This funcion is responsible for
	reading the specified number of bytes from a slave
	device.
	Inputs:  address: the slave address to read from
			 data: a pointer to where the data will be stored
			 bytes: the number of bytes to read
	Outputs: none
***********************************************************/
void I2CInt_readData(unsigned char address, unsigned char *data, unsigned char bytes)
{
    /* Bus is busy wait (or exit with error code) */
	while(status & (1<<BUSY));									

	twi_address = address;
	twi_data = data;
	twi_bytes = bytes;
	twi_ddr = TW_READ;

	retry_cnt = 0;
	
	/* Generate start condition, the remainder of the transfer is interrupt driven and
	   will be performed in the background */
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
	
	status |= (1<<BUSY);
}

/***********************************************************
	Function Name: I2CInt_isI2cBusy
	Function Description: This funcion is responsible for
	indicating if the I2C bus is currently busy to external
	modules.
	device.
	Inputs:  none
	Outputs: bool_t - indicating if bus is busy
***********************************************************/
bool_t I2CInt_isI2cBusy(void)
{
	bool_t retVal = FALSE;
	if ( (status & (1<<BUSY)) != 0)
	{
		retVal = TRUE;
	}
	
	return(retVal);
}

/***********************************************************
	Function Name: <interrupt handler for I2C>
	Function Description: This function is responsible for
	implementing the control logic needed to perform a
	read or write operation with an I2C slave.
	Inputs:  none
	Outputs: none
***********************************************************/
SIGNAL(SIG_2WIRE_SERIAL)
{
	unsigned char TWI_status = TWSR & TW_STATUS_MASK;   /* grab just the status bits */
	
    /* the entire I2C handler is state-based...determine
    what needs to be done based on TWI_status */
	switch(TWI_status) 
    {
        case TW_START:									/* Start condition */
        case TW_REP_START:								/* Repeated start condition */
            if(retry_cnt > MAX_TWI_RETRIES) 
            {
                /* generate stop condition if we've reached our retry limit */
                TWCR |= (1<<TWINT)|(1<<TWSTO);					
                status &= ~(1<<BUSY);								
                return;												
            }
            /* indicate read or write */
            TWDR = (twi_address<<1) + twi_ddr;	
            /* TWSTA must be cleared...also clears TWINT */
            TWCR &= ~(1<<TWSTA);
            break;

        case TW_MT_SLA_ACK:							/* Slave acknowledged address, */
            retry_cnt = 0;					
            /* tx the data, and increment the data pointer */
            TWDR = *twi_data;										
            twi_data++;			

            /* clear the int to continue */
            TWCR |= (1<<TWINT);						
            break;

        case TW_MT_SLA_NACK:							/* Slave didn't acknowledge address, */
        case TW_MR_SLA_NACK:
            retry_cnt++;		

            /* retry...*/
            TWCR |= (1<<TWINT)|(1<<TWSTA)|(1<<TWSTO);	
            break;

        case TW_MT_DATA_ACK:							/* Slave Acknowledged data, */
            if(--twi_bytes > 0) 
            {						
                /* more data to send, so send it */
                TWDR = *twi_data;									
                twi_data++;											
                TWCR |= (1<<TWINT);								
            }
            else 
            {
                /* generate the stop condition if needed */
                TWCR |= (1<<TWSTO)|(1<<TWINT);					
                status &= ~(1<<BUSY);								
            }
            break;

        case TW_MT_DATA_NACK:							/* Slave didn't acknowledge data */
            /* send the stop condition */
            TWCR |= (1<<TWINT)|(1<<TWSTO);						
            status &= ~(1<<BUSY);									
            break;

        case TW_MR_SLA_ACK:                             /* Slave acknowledged address */
            if(--twi_bytes > 0) 
            {
                /* if there is more than one byte to read, acknowledge */
                TWCR |= (1<<TWEA)|(1<<TWINT);	
            }
			else
            {
                /* no acknowledge */
                TWCR |= (1<<TWINT);					
            }
            break;

        case TW_MR_DATA_ACK: 							/* Master acknowledged data */
        
            /* grab the received data */
            *twi_data = TWDR;										
            twi_data++;											
            if(--twi_bytes > 0) 
            {
                /* get the next data byte and ack */
                TWCR |= (1<<TWEA)|(1<<TWINT);	
            }
            else
            {
                /* clear out the enable acknowledge bit */
                TWCR &= ~(1<<TWEA);							
            }
            break;

        case TW_MR_DATA_NACK:						/* Master didn't acknowledge data -> end of read process */
            /* read data, and generate the stop condition */
            *twi_data = TWDR;										
            TWCR |= (1<<TWSTO)|(1<<TWINT); 						
            status &= ~(1<<BUSY);											
            break;
	}
}









