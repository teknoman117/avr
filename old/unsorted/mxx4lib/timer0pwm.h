/*
 *      timer0pwm.h
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
 *      This header controls timer0 as a pwm timer.  This timer's pwm
 *      is strictly 8 bit
 * 
 */


#ifndef _TIMER_0_PWM
#define _TIMER_0_PWM 1

#include <avr/io.h>
#include <stdint.h>

//timer 0 prescalers
#define T0_PRESCALER_1 (1 << CS00)                        //fosc / 1
#define T0_PRESCALER_8 (1 << CS01)                        //fosc / 8
#define T0_PRESCALER_64 (1 << CS01) | (1 << CS00)         //fosc / 64
#define T0_PRESCALER_256 (1 << CS02)                      //fosc / 256
#define T0_PRESCALER_1024 (1 << CS02) | (1 << CS00)       //fosc / 1024

void init_timer0_pwm ( uint8_t prescaler, uint8_t invert ) {
	DDRB |= (1 << PORTB3) | (1 << PORTB4); //set the proper pins as outputs
	TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); //set up the timer for standard mode
	if (invert) {                          //if inversion requested
		TCCR0A |= (1 << COM0A0) | (1 << COM0B0);//set the extra bits required for inverted output
	}
	TCCR0B = (prescaler & 0x07); //set the other mode bits and the prescaler (masks unused bits)
}

void set_pwm_0a ( uint8_t value ) {
	OCR0A = value;
}

void set_pwm_0b ( uint8_t value ) {
	OCR0B = value;
}

#endif
