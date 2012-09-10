/*
 *      stdusart1.c
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

#include "stdusart1.h"
#include <stdio.h>

#ifdef UDR1

int txb1 ( char c ) {
	while ( !( UCSR1A & (1<<UDRE1)) );         //wait for the tx buffer th be ready
	UDR1 = c;                                  //start transmission of c
	return 0;      
}

int rxb1 ( void ) {
	while( !(UCSR1A & (1<<RXC1)));             //wait for there to be data
    return UDR1;                               //return the data
}
	

void init_stdusart0 (uint16_t ubrr, uint8_t specs) {
	UBRR1 = ubrr;                              //set the UBRR register            
    UCSR1B = (1<<RXEN1)|(1<<TXEN1);            //enable transmitting and receiving
    UCSR1C = specs;                            //set the settings to what you wanted
    fdevopen((void*)txb1, (void*)rxb1);	       //open the stdin and stdout connections for stdio	
}

#endif
