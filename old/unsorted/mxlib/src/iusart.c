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
 
#include <iusart.h>
#include <avr/interrupt.h>

uint8_t rbuffer[RBUFFER_SIZE];       //create a buffer
volatile uint16_t rp;                //read pointer
volatile uint16_t wp;                //write pointer
volatile uint16_t bs;                //bytes received

void flush_buffer ( void ) {
	bs = wp = rp = 0;                     //set both pointers to zero, and the byte count, causing the buffer to be "empty"
}

uint8_t bufferEmpty ( void ) {
	return (bs < 1);               //buffer empty if no bytes received
}

uint8_t bufferFull ( void ) {
	uint16_t tmp = (wp + 1) & RBUFFER_MASK;  //get the next write location
	return (tmp == rp);              //if the next write location is the current read location, the buffer is full 
}

uint16_t bytesInBuf ( void ) {
	return bs;                    //return the bytes in buffer
}

void txbi (uint8_t byte) {
	while ( !( UCSRA & (1<<UDRE)) );//wait until the tx buffer is empty
	UDR = byte;	                  //transmit the byte
}

void writei (uint8_t *bytes, uint16_t number) {
	uint16_t c;                       //create a counter variable
	for (c=0; c<number; c++) {        //repeat number times
		txbi(bytes[c]);              //transmit byte at location c in bytes
	}
}	

uint8_t rxbi ( void ) {
	uint16_t tmp = 0;               //initalize tmp 
	if(bytesInBuf() < 1) {          //if the buffer is empty
	    while (bytesInBuf() < 1);   //wait until data arrives
	}
	tmp = (rp + 1) & RBUFFER_MASK;   //set tmp to the next read pointer level
	rp = tmp;                        //set the read pointer
	bs--;                            //decrement bytes received variable
	return rbuffer[tmp];             //return the data
}

void readi (uint8_t* bytes, uint8_t n) {         //read n number of bytes from buffer
	uint16_t c;            //counter variable
	for(c=0;c<n;c++) {     //read requested num of bytes
		bytes[c] = rxbi();//write read data to bytes
	}
}

void readie (uint8_t* bytes, uint8_t e) {
	uint16_t c = 0;
	uint8_t b = rxbi();   //initalize b to next byte
    while (b != e) {       //while end byte not received
    	bytes[c++] = b;    //set bytes[c] to b and increase c    	
    	b = rxbi();       //get the next byte
    }
}

void init_iusart (uint16_t ubrr, uint8_t specs) {
	UBRRL = (ubrr & 0xFF);                  //set UBRR register
	UBRRH = ((ubrr >> 8) & 0x0F);
    UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);//enable the usart
    UCSRC = (1<<URSEL) | specs;               //set your mode
    flush_buffer();                            //blank counter variables
	sei();                                     //turn on interrupts
}

ISR (USART_RXC_vect) {         //received a byte ISR
	uint16_t tmp = (wp + 1) & RBUFFER_MASK; //create temporary variable of next location
	uint8_t b = UDR;                       //store byte into variable
	if (tmp == rp) {
		  //buffer overflow, doesn't overwrite non read data
	} 
	else {
		bs++;             //increment bytes received    
	    rbuffer[tmp] = b; //store byte	
	    wp = tmp;         //update write pointer
	}
}
