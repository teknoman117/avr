/*
 *      timer0_test.c
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
 *      This program tests the timer0.h header
 *      This program uses timer0 as a counter.  A push button circuit needs
 *      to be plugged into the T0 pin (PortB.0) for this program to work the
 *      current timer 0 value is then displayed 4 times per second
 * 
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdusart0.h>
#include <timer0.h>
#include <stdio.h>

int main ( void ) {
	uint16_t v;                  //create a 16 bit variable for storage
	init_stdusart0(51, DB8 | P_N | SB1); //init usart for 19.2k baud 8N1  (51 is ubrr value for 19.2k baud at 16 MHz)
	init_timer0(T0_PRESCALER_EXTERNAL_FALLING, 255); //init timer0 to clock on the falling edge of the T0 pin and to top the timer at 255
	printf("AVR Timer 0 Testing Program\r");//print welcome message
	while (1) {                         //repeat forever
	    v = get_timer0();               //store value
	    printf("Current Timer0 Value: %d\r", v);  //print current value
	    _delay_ms(250);                           //wait 1/4 of a second
	}
	return 0;
}
