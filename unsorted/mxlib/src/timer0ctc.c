/*
 *      timer0ctc.c
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
 *      This header controls timer0 for ctc
 *
 */

#include "timer0ctc.h"
#include <avr/interrupt.h>

#ifdef OCR0

void init_timer0_ctc ( uint8_t prescaler, uint8_t pinfunc ) {
	if(pinfunc > 0) {DDRB |= (1 << PORTB3);} //id ocr0a is used, set pin as ouput
	if(prescaler > 5) {DDRB &= ~(1 << PORTB0);} //if an external clock source is used, set T0 pin as input
	OCR0 = 0;                         //clear the ocr0 register
	TCCR0 = ((pinfunc << 4) & 0x30) | (1 << WGM01) | (prescaler & 0x07); //set up timer0
}

void set_ocr0 ( uint8_t v ) {
	OCR0 = v;                       //set the output compare register 0a
}

void enable_interrupt_t0 ( void ) {
	TIMSK |= (1 << OCIE0);            //set the timer output compare interrupt enable 0a bit
	sei();                            //set the global interrupt flag
}

void disable_interrupt_t0 ( void ) {
	TIMSK &= ~(1 << OCIE0);           //clear the timer output compare interrupt enable 0a bit
}

#endif
