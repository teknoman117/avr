/*
 *      timer0.h
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
 *      This header controls the timer counter 0
 * 
 */

#ifndef _TIMER_0_H
#define _TIMER_0_H 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

//timer 0 prescalers
#define T0_PRESCALER_1 (1 << CS00)                        //fosc / 1
#define T0_PRESCALER_8 (1 << CS01)                        //fosc / 8
#define T0_PRESCALER_64 (1 << CS01) | (1 << CS00)         //fosc / 64
#define T0_PRESCALER_256 (1 << CS02)                      //fosc / 256
#define T0_PRESCALER_1024 (1 << CS02) | (1 << CS00)       //fosc / 1024
#define T0_PRESCALER_EXTERNAL_RISING (1 << CS02) | (1 << CS01) //external clock, clock on rising edge of t0 pin
#define T0_PRESCALER_EXTERNAL_FALLING (1 << CS02) | (1 << CS01) | (1 << CS00) //external clock, clock on falling edge of t0 pin

void init_timer0 ( uint8_t prescaler, uint8_t top ) {
	if(prescaler > 5) {DDRB &= ~(1 << PORTB0);} //if an external clock source is used, set T0 pin as input
	TCNT0 = 0;                        //clear the tcnt0 register
	OCR0A = top;                      //set the top
	TCCR0A = (1 << WGM01);            //set the mode of the timer
	TCCR0B = prescaler & 0x07;        //set the prescaler and mask bits that aren't the prescaler
}

uint8_t get_timer0 ( void ) {
	return TCNT0;                     //return the timer counter 0 register value
}

void set_timer0 ( uint8_t v ) {
	TCNT0 = v;                       //set the timer0 register
}

void reset_timer0 ( void ) {
	TCNT0 = 0;                       //set the timer0 register to 0, clearing the timer
}

void enable_interrupt_t0 ( void ) {
	TIMSK0 = (1 << TOIE0);            //set the timer overflow interrupt enable 0 bit
	sei();                            //set the global interrupt flag
}

void disable_interrupt_t0 ( void ) {
	TIMSK0 &= ~(1 << TOIE0);           //clear the timer overflow interrupt enable 0 bit
}

#endif
