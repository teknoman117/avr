/*
 *      timer1pwm.c
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
 *      This header controls timer 1 pwm functions
 * 
 */

#include "timer1pwm.h"

void init_timer1_pwm ( uint8_t prescaler, uint16_t top, uint8_t invert ) {
	DDRD |= (1 << PORTD4) | (1 << PORTD5); //set the proper pins as outputs
	ICR1 = top;                            //set the top value of the timer
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); //set up the timer for standard mode
	if (invert) {                          //if inversion requested
		TCCR1A |= (1 << COM1A0) | (1 << COM1B0);//set the extra bits required for inverted output
	}
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (prescaler & 0x07); //set the other mode bits and the prescaler (masks unused bits)
}

void set_pwm_1a ( uint16_t value ) {
	OCR1A = value;
}

void set_pwm_1b ( uint16_t value ) {
	OCR1B = value;
}
