/*
 *      dio_test.c
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
 *      The purpose of this program is to test the digital IO functions.
 *      the program reads portc.1 and writes it to portc.0
 * 
 *      DDR registers are directional registers
 *      PORT registers store the desired state if an output
 *      PIN registers store the value of the port currently (in input, is input, as output, sends back whats outputed)
 *  
 */

#define F_CPU 16000000UL

#include <avr/io.h>

int main ( void ) {
	uint8_t r = 0;                        //create a byte sized variable
	DDRC |= (1 << PORTC0);                //set portc0 in ddr (set as output)
	DDRC &= ~(1 << PORTC1);               //clear portc1 in ddrc (set as input)
	while (1) {                           //loop forever
	    r = (PINC >> PORTC1) & 1;         //store value of portc1
	    if(r) {PORTC |= (1 << PORTC0);}   //if r > 0, set PORTC0
	    else {PORTC &= ~(1 << PORTC0);}   //else, clear PORTC0
	}
	return 0;
}
