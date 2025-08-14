/*
 *      usart.h
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
 *      This header controls unbuffered byte strings to and from usart0
 */

#ifndef _USART0_H
#define _USART0_H 1

#include <avr/io.h>
#include <stdint.h>

#define DB8 (UCSZ01 << 1) | (1 << UCSZ00)       //bit combos for 8 data bits
#define DB7 (1 << UCSZ00)                       //bit combos for 7 data bits
#define DB6 (1 << UCSZ00)                       //bit combos for 6 data bits
#define DB5 0                                   //bit combos for 5 data bits

#define P_N 0                                   //bit combos for no parity
#define P_E (1 << UPM01)                        //bit combos for even parity
#define P_O (1 << UPM01) | (1 << UPM00)         //bit combos for odd parity

#define SB1 0                                   //bit combos for 1 stop bit
#define SB2 (1 << USBS0)                        //bit combos for 2 stop bits

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
 
uint8_t rxb_0 ( void ) {                           //read a byte                             
    while( !(UCSR0A & (1<<RXC0)));          //wait for a byte
    return UDR0;                             //return the byte
}

void write_0 (uint8_t *bytes, uint16_t n) {  //write a string
	for(x=0;x<n;x++) {                      //transmit n bytes
	    txb_0(bytes[x]);                    //transmit one byte
	}
}

void write_0s (char *bytes, uint16_t n) {  //write a string
	for(x=0;x<n;x++) {                      //transmit n bytes
	    txb_0(bytes[x]);                    //transmit one byte
	}
}

void read_0 (uint8_t *bytes, uint16_t n) { //need to use alloca to allocate data for the pointer
    for(x=0;x<n;x++) {                     //for n bytes
    	*bytes++=rxb_0();                  //get the byte and put it in bytes
    }
}

void read_0s (char *chars, uint16_t n, char stopchar) {  //get n bytes or until stopchar is received
	for(x=0;x<n;x++) {                 //for n bytes
		*chars=(char)rxb_0();          //get the next byte
		if(*chars==stopchar) {break;}  //if the next byte is the stopping character, quit the loop 
		chars++;                       //increment the pointer
	}	
} 

#endif

