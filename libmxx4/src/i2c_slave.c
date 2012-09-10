/*
 *      i2c_slave.c
 *      
 *      Copyright 2010 Nathaniel Robert Lewis <linux.robotdude@gmail.com>
 *      
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 * 
 *      This header creates an entirely interrupt driven I2C slave
 *      interface for the AVR.  It has a buffer and a master can read
 *      or write data to it without you having to code the transaction
 *      on the slave processor.  Just start this up and check for 
 *      changes.
 * 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "i2c_slave.h"

volatile uint8_t regAccess;                    //if the next byte is a register
volatile uint8_t status;                       //status of the TWI Interface
volatile uint8_t ack;                          //ack or not to ack

ISR (TWI_vect) {                               //The TWI interface did something
	status = TWSR & 0xF8;                      //get the status, masking prescaler bits
	switch (status) {                          //decide what to do
        case TW_SR_SLA_ACK://slave address received
            regAccess = 1; //next byte will be address
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 <<TWIE ) | (1 << TWEN);  //set up for next transfer
            break;
/*--------------------Slave Receiver Status--------------------------------*/
        case TW_SR_DATA_ACK://received a byte
            if(regAccess) { //byte was register byte
                i2c_counter = TWDR;	 //set the counter
                regAccess = 0;       //clear the register access flag
            }
            else {
            	i2c_buffer[i2c_counter++] = TWDR; //write the data to buffer
            }
            ack = (1 << TWEA);                 //prepare to acknoledge next byte
            if(i2c_counter == I2C_BUFFER_SIZE) ack = 0; //if next byte is buffer's last, clear ack so nack is sent
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | ack;  //set up control register
            break;
        case TW_SR_DATA_NACK://received a byte
            if(regAccess) { //byte was register byte
                i2c_counter = TWDR;	 //set the counter
                regAccess = 0;       //clear the register access flag
            }
            else {
            	i2c_buffer[i2c_counter++] = TWDR; //write the data to buffer
            }
            ack = (1 << TWEA);               //prepare for acknowledgement
            if(i2c_counter == I2C_BUFFER_SIZE) ack = 0;  //if next register is last, set up nack
            if(i2c_counter > I2C_BUFFER_SIZE) {i2c_counter = 0; ack = (1 << TWEA);}  //if current register is last, reset counter
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | ack; //set up control register
            break;
        case TW_SR_STOP://received a stop or repeated start condition
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);  //set up to receive slave address
            break;
/*-----------------------Slave Transmitter Status------------------------*/
        case TW_ST_SLA_ACK:             //slave transmitter mode active
            ack = (1 << TWEA);          //set up for ack
            if(i2c_counter == I2C_BUFFER_SIZE) {ack = 0;} //if next register is last availble byte, set up nack
            TWDR = i2c_buffer[i2c_counter++]; //set up the data register
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | ack; //set up control register
            break;   
        case TW_ST_DATA_ACK:            //byte transmitted, load next byte
            ack = (1 << TWEA);          //set up for ack
            if(i2c_counter == I2C_BUFFER_SIZE) {ack = 0;} //if this byte is last, set up for nack
            TWDR = i2c_buffer[i2c_counter++];    //set up data register
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | ack; //set up control register
            break;         
        case TW_ST_DATA_NACK:           //byte transmitted, sender done
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);  //prepare to receive sla+r/w
        case TW_ST_LAST_DATA:           //byte transmitted, receiver out of data
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);  //prepare to receive sla+r/w
        default: break;		
	}
}

void init_i2c_slave ( uint8_t sla, uint8_t bitrate, uint8_t prescaler ) {
	i2c_counter = 0;    //set counter to zero
	regAccess = 0;      //make sure register access is OFF
	TWAR = sla & 0xfe;  //set the slave address, masking direction bit
	TWBR = bitrate;     //set the bit rate
	TWSR = prescaler & 0x07;//set the i2c prescaler, masking non used bits
    TWCR = (1 << TWEN) | (1 <<TWEA) | (1 << TWIE);  //enable TWI 
    sei();                //and interrupts
}
