/*
 *      timer2ctc.h
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
 *      This header controls timer 2 under ctc settings
 * 
 */

#ifndef _TIMER_CTC_2
#define _TIMER_CTC_2 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

//timer 2 prescalers
#define T2_PRESCALER_1 (1 << CS20)   // t2clock/1
#define T2_PRESCALER_8 (1 << CS21)   // t2clock/8
#define T2_PRESCALER_32 (1 << CS21) | (1 << CS20) // t2clock/32
#define T2_PRESCALER_64 (1 << CS22)  // t2clock/64
#define T2_PRESCALER_128 (1 << CS22) | (1 << CS20)// t2clock/128
#define T2_PRESCALER_256 (1 << CS22) | (1 << CS21)// t2clock/256
#define T2_PRESCALER_1024 (1 << CS22) | (1 << CS21) | (1 << CS20) // t2clock/1024

//timer 2 clock sources
#define T2_SOURCE_INTERNAL 0
#define T2_SOURCE_EXTERNAL 1

//pin functions for ctc on timer 2
#define PIN_DISCONNECT 0                                  //disconnect pin from timer
#define PIN_TOGGLE 1                                      //toggle pin on compare match
#define PIN_CLEAR 2                                       //clear (set to 0) pin on compare match 
#define PIN_SET 3                                         //set (set to 1) pin on compare match

void init_timer2_ctc ( uint8_t prescaler, uint8_t pinfunca, uint8_t pinfuncb, uint8_t clksource ) {
	if(clksource) {          //if an external clock is used:
		DDRC &= ~(1 << PORTC6);//input the clock pin
		ASSR |= (1 << EXCLK); //tell timer 2 to use an external clock not external crystal
		ASSR |= (1 << AS2);   //tell timer 2 to use an external clock source
	}
	if(pinfunca > 0) {DDRD |= (1 << PORTD7);} //if ocr2a is used, set its pin as an output
	if(pinfuncb > 0) {DDRD |= (1 << PORTD6);} //if ocr2b is used, set its pin as an output
	OCR2A = 0;           //clear the output compare registers for timer 2
	OCR2B = 0;
	TCCR2A = ((pinfunca << 6) & 0xC0) | ((pinfuncb << 4) & 0x30) | (1 << WGM21);//set the mode of the timer and the pin modes
	TCCR2B = prescaler & 0x07; //set the prescaler and mask non prescaler bits
}

void set_ocr2a ( uint8_t v ) {
	OCR2A = v;                       //set the output compare register 2a
}

void set_ocr2b ( uint8_t v ) {
	OCR2B = v;                       //set the output compare register 2b
}

void enable_interrupt_t2a ( void ) {
	TIMSK2 = (1 << OCIE2A);            //set the timer output compare interrupt enable 2a bit
	sei();                            //set the global interrupt flag
}

void enable_interrupt_t2b ( void ) {
	TIMSK2 = (1 << OCIE2B);            //set the timer output compare interrupt enable 2b bit
	sei();                            //set the global interrupt flag
}

void disable_interrupt_t2a ( void ) {
	TIMSK2 &= ~(1 << OCIE2A);           //clear the timer output compare interrupt enable 2a bit
}

void disable_interrupt_t2b ( void ) {
	TIMSK2 &= ~(1 << OCIE2B);           //clear the timer output compare interrupt enable 2b bit
}
	
#endif
