/*
 *      i2c.h
 *      
 *      Copyright 2009 Nathaniel Robert Lewis <linux.robotdude@gmail.com>
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
 *      This header controls the internal i2c controller (master only)
 * 
 */

#ifndef _I2C_H
#define _I2C_H 1

#include <avr/io.h>
#include <util/twi.h>
#include <stdint.h>

#define I2C_PRESCALER_1 0
#define I2C_PRESCALER_4 1
#define I2C_PRESCALER_16 2
#define I2C_PRESCALER_64 3

void init_i2c (uint8_t bitrate, uint8_t prescaler) {
	TWBR = bitrate;          //set the bit rate register
	TWSR = (prescaler & 0x03);//set the prescaler
}

int8_t i2c_start ( uint8_t address ) {
	uint8_t twst = 0;                          //create a buffer variable
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);//transmit start condition
    while (!(TWCR & (1 << TWINT))) ;           //wait until complete
	twst = TWSR & 0xF8;                        //read status, masking the prescaler bits
	if ((twst != TW_START) && (twst != TW_REP_START)) return -1; //if start not completed, return error
	TWDR = address;                            //set the data register to the address
	TWCR = (1 << TWINT) | (1 << TWEN);         //transmit the address
    while (!(TWCR & (1 << TWINT))) ;           //wait for it to complete
	twst = TW_STATUS & 0xF8;                   //read status, masking the prescaler bits
	if ((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK)) return -1;//if ack not received, return error
	return 0;                                  //return ok
}

void i2c_stop ( void ) {
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);  //transmit stop condition
	while(TWCR & (1<<TWSTO));                    //wait for it to complete
}

int8_t i2c_txb ( uint8_t b ) {                  //transmits a byte, returns 1 if ack received, 0 if nack
	TWDR = b;                                   //set up the data
	TWCR = (1 << TWINT) | (1 << TWEN);          //start the transmission
    while (!(TWCR & (1 << TWINT))) ;            //wait until complete
    uint8_t twst = TWSR & 0xF8;                 //get the status
	if( twst == TW_MT_DATA_ACK) return 0;       //return 0 if ack received
	if( twst == TW_MT_DATA_NACK) return 1;      //return 1 if nack received
	return -1;                                  //return -1 since error occurred
}

uint8_t i2c_rxbAck ( void ) {
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); //start read with Ack
	while (!(TWCR & (1 << TWINT)));             //wait for it to complete
	return TWDR;                                //return data
}

uint8_t i2c_rxbNack ( void ) {
	TWCR = (1 << TWINT) | (1 << TWEN);          //start read with Nack
	while (!(TWCR & (1 << TWINT)));             //wait for it to complete
	return TWDR;                                //return data
}

void i2c_write_reg( uint8_t address, uint8_t reg ) {

}

void i2c_write_reg16( uint8_t address, uint16_t reg16 ) {

}

void i2c_read_reg( uint8_t address, uint8_t reg ) {

}

void i2c_read_reg16( uint8_t address, uint16_t reg16 ) {

}
    
#endif
