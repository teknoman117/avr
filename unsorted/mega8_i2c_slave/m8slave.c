/*
 *      m8slave.c
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

#include <util/delay.h>
#include <util/twi.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

volatile uint8_t sla = 0;
volatile uint8_t twst = 0;
volatile uint8_t reg = 0;
volatile uint8_t data = 0;

ISR (TWI_vect) {
    cli();          //disable interrupts
    twst = TWSR & 0xF8; //get status
    if(twst == TW_SR_SLA_ACK) {  //if addressed properly
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);  //get
        while (!(TWCR & (1 << TWINT)));
        reg = TWDR;	
        twst = TWSR & 0xF8;
        if(twst != TW_SR_DATA_ACK) {PORTD = 3;}
        TWCR = (1 << TWINT) |(1 << TWEN) | (1 << TWEA);
        while (!(TWCR & (1 << TWINT)));
        data = TWDR;
        twst = TWSR & 0xF8;
        if(twst != TW_SR_DATA_ACK) {PORTD = 7;}
        if(reg == 1) {
            if(data) PORTB |= (1 << PORTB1);
            else {PORTB &= ~(1 << PORTB1);}
        }
        if(reg == 2) {
            if(data) PORTB |= (1 << PORTB2);
            else {PORTB &= ~(1 << PORTB2);}
        }	
    }
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 <<TWEA) | (1 << TWIE); 
    sei();
}

int main ( void ) {
	DDRB |= (1 << PORTB1) | (1 << PORTB2);
	DDRD = 0xFF;
	PORTB = 0;
	PORTD = 0;
	TWBR = 128;
	TWSR = 0x00;
	TWAR = 0xca;     //set the slave address for the TWI unit
	TWCR = (1 << TWEN) | (1 <<TWEA) | (1 << TWIE);  //enable TWI
	sei();           //enable interrupts
	while (1);
	return 0;
}
