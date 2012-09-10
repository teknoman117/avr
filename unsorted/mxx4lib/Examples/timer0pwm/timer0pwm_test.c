/*
 *      timer0pwm_test.c
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
 *      This program tests the timer0pwm.h header by raising and lowering
 *      the intensity of an led connected to ocr0a is cycles of 1 seconds
 * 
 */

#define F_CPU 16000000UL

#include <util/delay.h>
#include <timer0pwm.h>
#include <stdint.h>

int main ( void ) {
	uint8_t c = 0;   //create an 8 bit counter variable
	init_timer0_pwm (T0_PRESCALER_1, 0); //set up pwm on timer 0 with no prescaler and non-inverted output
	while (1) {      //repeat forever
	    c = 0;               //clear counter variable
	    while (c++ < 255) {  //count to 255
	    	set_pwm_0a(c);   //set pwm channel
	    	_delay_ms(2);    //by waiting 2 milliseconds every cycle, the total delay is about 1/2 second
	    }
	    c = 255;             //max out the counter variable
	    while (c-- > 0) {    //count to 0
	    	set_pwm_0a(c);   //set pwm channel
	    	_delay_ms(2);    //by waiting 2 milliseconds every cycle, the total delay is about 1/2 second
	    }
	}	
	return 0;
}
