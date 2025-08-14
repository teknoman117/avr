/*
 *      iusart1.c
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
 
#include "iusart1.h"
#include <avr/interrupt.h>

#ifdef UDR1

uint8_t rbuffer[RBUFFER_SIZE1];       // RX Buffer
uint8_t tbuffer[TBUFFER_SIZE1];       // TX Buffer

volatile uint16_t rp;                //read pointer
volatile uint16_t wp;                //write pointer
volatile uint16_t bs;                //bytes received
volatile uint16_t rp_tx;             // read pointer (tx)
volatile uint16_t wp_tx;             // write pointer (tx)
volatile uint16_t bs_tx;             // bytes received (tx)

void flush_buffer1 ( void ) {
	bs = wp = rp = 0;                     //set both pointers to zero, and the byte count, causing the buffer to be "empty"
	bs_tx = wp_tx = rp_tx = 0;
}

uint8_t bufferEmpty1 ( void ) {
	return (bs < 1);               //buffer empty if no bytes received
}

uint8_t bufferFull1 ( void ) {
	uint16_t tmp = (wp + 1) & RBUFFER_MASK1;  //get the next write location
	return (tmp == rp);              //if the next write location is the current read location, the buffer is full 
}

uint16_t bytesInBuf1 ( void ) {
	return bs;                    //return the bytes in buffer
}

void txbi1 (uint8_t byte) {
	uint16_t tmp = (wp_tx + 1) & TBUFFER_MASK1;
	while (tmp == rp_tx);
	bs_tx++;               
	tbuffer[tmp] = byte; 	
	wp_tx = tmp;     
	UCSR1B |= _BV(UDRIE1); 
}

void writei1 (uint8_t *bytes, uint16_t number) {
	uint16_t c;                       //create a counter variable
	for (c=0; c<number; c++) txbi1(bytes[c]); //transmit byte at location c in bytes
}	

void writeStringi1( char *string ) {
	uint8_t counter = 0;
	for(counter = 0; string[counter] != '\0'; counter++) txbi1(string[counter]);
}

uint8_t rxbi1 ( void ) {
	uint16_t tmp = 0;               //initalize tmp 
	while (bytesInBuf1() < 1);       //wait until data arrives
	tmp = (rp + 1) & RBUFFER_MASK1;   //set tmp to the next read pointer level
	rp = tmp;                        //set the read pointer
	bs--;                            //decrement bytes received variable
	return rbuffer[tmp];             //return the data
}

void readi1 (uint8_t* bytes, uint8_t n) {         //read n number of bytes from buffer
	uint16_t c;            //counter variable
	for(c=0;c<n;c++) bytes[c] = rxbi1();//write read data to bytes
}

void readi1e (uint8_t* bytes, uint8_t e) {
	uint16_t c = 0;
	uint8_t b = rxbi1();   //initalize b to next byte
    while (b != e) {       //while end byte not received
    	bytes[c++] = b;    //set bytes[c] to b and increase c    	
    	b = rxbi1();       //get the next byte
    }
}

void init_iusart1 (uint16_t ubrr, uint8_t specs) {
	UBRR1 = ubrr;                              //set UBRR1 register
    UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1);//enable the usart
    UCSR1C = specs;		                       //set your mode
    flush_buffer1();                            //blank counter variables
	sei();                                     //turn on interrupts
}

ISR (USART1_RX_vect) {         //received a byte ISR
	uint16_t tmp = (wp + 1) & RBUFFER_MASK1; //create temporary variable of next location
	uint8_t b = UDR1;                       //store byte into variable
	if (tmp == rp) {/* Buffer overflow, doesn't overwrite non read data */} 
	else {
		bs++;             //increment bytes received    
	    rbuffer[tmp] = b; //store byte	
	    wp = tmp;         //update write pointer
	}
}

ISR (USART1_UDRE_vect, ISR_NOBLOCK) {
	uint16_t tmp = 0;               
	if(bs_tx < 1) UCSR0B &= ~_BV(UDRIE1);
	else {
		tmp = (rp_tx + 1) & TBUFFER_MASK1;   
		rp_tx = tmp;                       
		bs_tx--;                           
		UDR1 = tbuffer[tmp];  
	}           
}

#endif
