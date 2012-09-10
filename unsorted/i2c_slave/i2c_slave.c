/*
 *      i2c_slave.c
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
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdint.h>
#include <alloca.h>

volatile uint8_t* i2c_buffer;
volatile uint8_t max_size;
volatile uint8_t reg_counter;
volatile uint8_t regAccess;
volatile uint8_t ack;
volatile uint8_t twst;

ISR (TWI_vect) {           //Twi Interrupt Occured
    twst = TWSR & 0xF8;    //Get the status of the TWI unit
    switch (twst) {        //decide what to do about it
        case TW_SR_SLA_ACK://slave address received
            regAccess = 1; //next byte will be address
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 <<TWIE ) | (1 << TWEN);  //set up for next transfer
            break;
/*--------------------Slave Receiver Status--------------------------------*/
        case TW_SR_DATA_ACK://received a byte
            if(regAccess) { //byte was register byte
                reg_counter = TWDR;	 //set the counter
                regAccess = 0;       //clear the register access flag
            }
            else {
            	i2c_buffer[reg_counter++] = TWDR; //write the data
            }
            ack = (1 << TWEA);
            if(reg_counter == max_size) ack = 0;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | ack;
            break;
        case TW_SR_DATA_NACK://received a byte
            if(regAccess) { //byte was register byte
                reg_counter = TWDR;	 //set the counter
                regAccess = 0;       //clear the register access flag
            }
            else {
            	i2c_buffer[reg_counter++] = TWDR; //write the data
            }
            ack = (1 << TWEA);
            if(reg_counter == max_size) ack = 0;
            if(reg_counter > max_size) {reg_counter = 0; ack = (1 << TWEA);}
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | ack;
            break;
        case TW_SR_STOP://received a stop or repeated start condition
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);  //set up to receive slave address
            break;
/*-----------------------Slave Transmitter Status------------------------*/
        case TW_ST_SLA_ACK:             //slave transmitter mode active
            ack = (1 << TWEA);
            if(reg_counter == max_size) {ack = 0;}
            TWDR = i2c_buffer[reg_counter++];
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | ack;
            break;   
        case TW_ST_DATA_ACK:            //byte transmitted, load next byte
            ack = (1 << TWEA);
            if(reg_counter == max_size) {ack = 0;}
            TWDR = i2c_buffer[reg_counter++];
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | ack;
            break;         
        case TW_ST_DATA_NACK:           //byte transmitted, sender done
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
        case TW_ST_LAST_DATA:           //byte transmitted, receiver out of data
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
        default: break;
    }	
}

int main ( void ) {
	//set up i2c buffer
	i2c_buffer = (uint8_t*) alloca ( sizeof(uint8_t) * 128 );//create 128 byte array
	max_size = 127;                                          //max pointer value       
	reg_counter = 0;
	regAccess = 0;    
	ack = 0;
	twst = 0;                              
	//set up IO pins
	DDRB |= (1 << PORTB1) | (1 << PORTB2);
	DDRD = 0xFF;
	PORTB = 0;
	PORTD = 0;
	i2c_buffer[1] = 0;
	i2c_buffer[2] = 0;
	//set up the I2C controller
	TWBR = 128;      //set bit rate
	TWSR = 0x00;     //disable prescaler
	TWAR = 0xca;     //set the slave address for the TWI unit
	TWCR = (1 << TWEN) | (1 <<TWEA) | (1 << TWIE);  //enable TWI
	sei();           //enable interrupts
	while (1) {      //main routine
        //if(i2c_buffer[0]) {PORTB |= (1 << PORTB1);}
        //else {PORTB &= ~(1 << PORTB1);}
        //if(i2c_buffer[1]) {PORTB |= (1 << PORTB2);}
        //else {PORTB &= ~(1 << PORTB2);}
        PORTD = i2c_buffer[1] & 0x07;
        PORTB = i2c_buffer[2] & 0x07;
        _delay_ms(100);
	}
	return 0;
}
