/*
 *      timer2_test.c
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
 *      This program tests the timer2.h header by setting up timer2 as a
 *      counter.  It sets timer 2 for an external clock source from pin
 *      TOSC1. A push button circuit needs to be plugged into TOSC1, or 
 *      PortC.6. A prescaler of 8 is set so you will notice that it takes
 *      8 pushes of the button for it to count up one.  The top is set at
 *      32, so after 32 clocks, it will reset.  The current value is printed
 *      out of usart0
 * 
 */

#define F_CPU 16000000UL

#include <util/delay.h>
#include <stdusart0.h>
#include <timer2.h>
#include <stdio.h>

int main ( void ) {
	uint16_t c = 0; //create a 16 bit storage variable
	init_stdusart0 (51, DB8 | P_N | SB1); //init usart0 for 19.2kBaud 8N1 (51 is the ubrr value for 19.2kBaud at 16 MHz)
	printf("AVR Timer2 Testing program\r");
	init_timer2 (T2_PRESCALER_8, 32, T2_SOURCE_EXTERNAL); //init timer 2 with a prescaler of 8, a top of 32, and an external clock source on TOSC1
	while (1) {        //repeat forever
	    c = get_timer2();  //store timer value
	    printf("Current Value of Timer 2: %d\r", c); //print current value
	    _delay_ms(250); //wait 1/4 of a second
	}
	return 0;
}
