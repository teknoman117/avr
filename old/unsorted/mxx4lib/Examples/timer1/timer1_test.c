/*
 *      timer1_test.c
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
 * 
 *      This program tests the timer1.h header.  It starts the timer and sets
 *      the top to a value where the timer will be after 4 seconds at 16 MHz
 *    
 *      The program displays the current timer value 4 times a second.  When
 *      the counter reaches the top it resets and the program demontrates that
 * 
 */

#define F_CPU 16000000UL

#include <util/delay.h>
#include <stdusart0.h>
#include <timer1.h>
#include <stdio.h>

int main ( void ) {	
	uint32_t v = 0;                        //create a variable to store 32 bit numbers
	init_stdusart0 (51, DB8 | P_N | SB1);  //initalize usart for 19.2k baud 8N1 for the stdio library
	init_timer1 (T1_PRESCALER_1024, 62500);//initalize timer1 for timer operations with a input of fosc/1024 and a top of 62500 (top is reached every 4 seconds at 16MHz)
	printf("Timer 1 Testing Program\r");   //print welcome message
	while (1) {                            //loop forever
		v = get_timer1();                  //store 16 bit timer value in a 32 bit variable to avoid sign problems
	    printf("Current Timer 1 Value: %ld\r", v);  //print current timer value
	    _delay_ms(250);	                   //wait 1/4 second
	}
	return 0;
}
