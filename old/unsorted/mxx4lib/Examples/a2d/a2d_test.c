/*
 *      a2d_test.c
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
 *      This program test the a2d converter.  It asks for a channel number
 *      and displays the value.  It uses the stdio library for text output
 *      and input from the usart.
 * 
 */

#define F_CPU 16000000UL

#include <stdusart0.h>
#include <a2d.h>
#include <stdio.h>
#include <stdint.h>

int main ( void ) {
	init_stdusart0(51, DB8 | P_N | SB1);  //init stdio uart at 19.2k baud 8N1 (51 is ubrr value for 19.2k at 16 MHz)
	initADC();                            //initalize the A2D converter
	uint16_t channelnum;                  //create 2 16 bit variables
	uint16_t value;
	printf("AVR ADC Test Program\r:");    //print a welcome message
	while (1) {                           //repeat forever
		scanf("%d", &channelnum);         //get the requested channel number
		value = getADC((uint8_t)(channelnum & 0x07)); //get the value of that channel  channel num is 16 bits, we need the lower three.  mask the non required bits
	    printf("Channel: %d = %d\r:", channelnum, value); //print the result
	}
	return 0;
}
