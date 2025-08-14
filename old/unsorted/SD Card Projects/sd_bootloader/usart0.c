/*
 *      usart.c
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
 *      This header controls unbuffered byte strings to and from usart0
 */

#include "usart0.h"

uint16_t x;

void init_usart0(uint16_t ubrr, uint8_t specs) {
	UBRR0 = ubrr;                              //set the UBRR register
    UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);//enable transmitting and receiving
    UCSR0C = specs;                            //set the settings to what you wanted
}
void txb_0 (uint8_t b) {                      //send a byte
	while ( !( UCSR0A & (1<<UDRE0)) );      //wait for the transmit register to empty
	UDR0 = b;	                             //send the byte
}

/*void write_0 (uint8_t *bytes, uint16_t n) {  //write a string
	for(x=0;x<n;x++) {                      //transmit n bytes
	    txb_0(bytes[x]);                    //transmit one byte
	}
}

void write_0s (char *bytes, uint16_t n) {  //write a string
	for(x=0;x<n;x++) {                      //transmit n bytes
	    txb_0(bytes[x]);                    //transmit one byte
	}
}*/

