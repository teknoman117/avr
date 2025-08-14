/*
 *      stdusart0.h
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
 *      This header is a non buffered uart, but it enables you to use the printf
 *      and scanf commands as you do in PC programming from the stdio.h header
 */

#ifndef _STDUART0_H
#define _STDUART0_H 1

#include <avr/io.h>
#include <stdio.h>

#define DB8 (UCSZ01 << 1) | (1 << UCSZ00)       //bit combos for 8 data bits
#define DB7 (1 << UCSZ00)                       //bit combos for 7 data bits
#define DB6 (1 << UCSZ00)                       //bit combos for 6 data bits
#define DB5 0                                   //bit combos for 5 data bits

#define P_N 0                                   //bit combos for no parity
#define P_E (1 << UPM01)                        //bit combos for even parity
#define P_O (1 << UPM01) | (1 << UPM00)         //bit combos for odd parity

#define SB1 0                                   //bit combos for 1 stop bit
#define SB2 (1 << USBS0)                        //bit combos for 2 stop bits

int txb0 ( char c ) {
	while ( !( UCSR0A & (1<<UDRE0)) );         //wait for the tx buffer th be ready
	UDR0 = c;                                  //start transmission of c
	return 0;      
}

int rxb0 ( void ) {
	while( !(UCSR0A & (1<<RXC0)));             //wait for there to be data
    return UDR0;                               //return the data
}
	

void init_stdusart0 (uint16_t ubrr, uint8_t specs) {
	UBRR0 = ubrr;                              //set the UBRR register            
    UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);//enable transmitting and receiving
    UCSR0C = specs;                            //set the settings to what you wanted
    fdevopen((void*)txb0, (void*)rxb0);	       //open the stdin and stdout connections for stdio	
}

#endif
