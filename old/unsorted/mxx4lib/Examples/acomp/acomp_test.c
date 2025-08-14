/*
 *      acomp_test.c
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
 *      This program tests the Analog comparator in the AVR.  It prints 
 *      out the value of the analog comparator every 1/4 of a second.
 *      The comparator is initalized with the positive input on AIN0
 *      and the negative input on AIN1.
 * 
 */

#define F_CPU 16000000UL

#include <util/delay.h>
#include <stdusart0.h>
#include <acomp.h>
#include <stdio.h>
#include <stdint.h>

int main ( void ) {
	init_stdusart0(51, DB8 | P_N | SB1);  //init stdio uart at 19.2k baud 8N1 (51 is ubrr value for 19.2k at 16 MHz)	
	initACOMP(0, 0);                      //initalize the analog comparator without multiplexing
	uint8_t result;                       //create a variable to store the result
	printf("AVR Analog comparator program\r"); //welcome message
	while (1) {                           //repeat forever
		result = getACOMP();
		if(result & 1) {                  //if output is 1
		    printf("AIN0 > AIN1\r");      //print message
	    }
	    else {                            //if output is 0
	    	printf("AIN1 > AIN0\r");      //print message
		}
		_delay_ms(250);                   //wait 1/4 of a second
    }
	return 0;
}
