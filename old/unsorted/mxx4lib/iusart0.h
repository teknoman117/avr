/*
 *      iusart0.c
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
 *      This header controls the usart and buffers incoming data by way of
 *      an interrupt service routine
 * 
 */

#ifndef _IUSART0_H
#define _IUSART0_H 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#ifndef RBUFFER_SIZE0
#warning RBUFFER_SIZE0 not defined, setting to 128
#define RBUFFER_SIZE0 128
#endif

#define RBUFFER_MASK0 ( RBUFFER_SIZE0 - 1 )
#if ( RBUFFER_SIZE0 & RBUFFER_MASK0 )
	#error RX buffer size is not a power of 2
#endif

#define DB8 (UCSZ01 << 1) | (1 << UCSZ00)       //bit combos for 8 data bits
#define DB7 (1 << UCSZ00)                       //bit combos for 7 data bits
#define DB6 (1 << UCSZ00)                       //bit combos for 6 data bits
#define DB5 0                                   //bit combos for 5 data bits

#define P_N 0                                   //bit combos for no parity
#define P_E (1 << UPM01)                        //bit combos for even parity
#define P_O (1 << UPM01) | (1 << UPM00)         //bit combos for odd parity

#define SB1 0                                   //bit combos for 1 stop bit
#define SB2 (1 << USBS0)                        //bit combos for 2 stop bits

uint8_t rbuffer0[RBUFFER_SIZE];       //create a buffer
volatile uint16_t rp0;                //read pointer
volatile uint16_t wp0;                //write pointer
volatile uint16_t bs0;                //bytes received

void flush_buffer0 ( void ) {
	bs0 = wp0 = rp0 = 0;                     //set both pointers to zero, and the byte count, causing the buffer to be "empty"
}

uint8_t bufferEmpty0 ( void ) {
	return (bs0 < 1);               //buffer empty if no bytes received
}

uint8_t bufferFull0 ( void ) {
	uint16_t tmp = (wp0 + 1) & RBUFFER_MASK0;  //get the next write location
	return (tmp == rp0);              //if the next write location is the current read location, the buffer is full 
}

uint16_t bytesInBuf0 ( void ) {
	return bs0;                    //return the bytes in buffer
}

void txbi0 (uint8_t byte) {
	while ( !( UCSR0A & (1<<UDRE0)) );//wait until the tx buffer is empty
	UDR0 = byte;	                  //transmit the byte
}

void writei0 (uint8_t *bytes, uint16_t number) {
	uint16_t c;                       //create a counter variable
	for (c=0; c<number; c++) {        //repeat number times
		txbi0(bytes[c]);              //transmit byte at location c in bytes
	}
}	

uint8_t rxbi0 ( void ) {
	uint16_t tmp = 0;               //initalize tmp 
	if(bytesInBuf0() < 1) {          //if the buffer is empty
	    while (bytesInBuf0() < 1);   //wait until data arrives
	}
	tmp = (rp0 + 1) & RBUFFER_MASK0;   //set tmp to the next read pointer level
	rp0 = tmp;                        //set the read pointer
	bs--;                            //decrement bytes received variable
	return rbuffer0[tmp];             //return the data
}

void readi0 (uint8_t* bytes, uint8_t n) {         //read n number of bytes from buffer
	uint16_t c;            //counter variable
	for(c=0;c<n;c++) {     //read requested num of bytes
		bytes[c] = rxbi0();//write read data to bytes
	}
}

void readi0e (uint8_t* bytes, uint8_t e) {
	uint16_t c = 0;
	uint8_t b = rxbi0();   //initalize b to next byte
    while (b != e) {       //while end byte not received
    	bytes[c++] = b;    //set bytes[c] to b and increase c    	
    	b = rxbi0();       //get the next byte
    }
}

void init_iusart0 (uint16_t ubrr, uint8_t specs) {
	UBRR0 = ubrr;                              //set UBRR0 register
    UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);//enable the usart
    UCSR0C = specs;		                       //set your mode
    flush_buffer0();                            //blank counter variables
	sei();                                     //turn on interrupts
}

ISR (USART0_RX_vect) {         //received a byte ISR
	uint16_t tmp = (wp0 + 1) & RBUFFER_MASK0; //create temporary variable of next location
	uint8_t b = UDR0;                       //store byte into variable
	if (tmp == rp0) {
		  //buffer overflow, doesn't overwrite non read data
	} 
	else {
		bs++;             //increment bytes received    
	    rbuffer0[tmp] = b; //store byte	
	    wp0 = tmp;         //update write pointer
	}
}

#endif
