/*
 *      timer1pwm_test.c
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
 *      This program tests the timer1pwm.h header by raising and lowering
 *      the intensity of an led hooked to ocr1a
 * 
 */

#define F_CPU 16000000UL

#include <util/delay.h>
#include <timer1pwm.h>
#include <stdint.h>

int main ( void ) {
	uint16_t counter = 0;     //create a 16 bit counter variable
	init_timer1_pwm (T1_PRESCALER_1, T1_PWM_10BIT, 0); //set up timer 1 pwm for no prescaler, 10 bit, and non-inverting
	while (1) {                   //repeat forever
		counter = 0;                //clear counter variable
		while (counter++ < 1023) {  //count to 1023
			set_pwm_1a(counter);    //set pwm channel to counter variable
			_delay_ms(1);           //by delaying 1 millisecond every count, the time taken is about 1 seconds
		}
		counter = 1023;             //set counter variable to highest value
		while (counter-- > 0) {    //count to 0
		    set_pwm_1a(counter);    //set pwm channel to counter variable
		    _delay_ms(1);
		}
	}
	return 0;
}
