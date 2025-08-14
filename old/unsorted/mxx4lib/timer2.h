/*
 *      timer2.h
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
 *      This header controls timer 2.  It supports prescaling, external
 *      clock, user set top, and the timer overflow 2 interrupt.  For 
 *      external clock input, a clock source has to be connected to the
 *      TOSC1 pin (PortC.6).  The t2clock is either clkIO (system clock)
 *      or the input from TOSC1
 * 
 */


#ifndef _TIMER_2
#define _TIMER_2 1

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

void init_timer2 ( uint8_t prescaler, uint8_t top, uint8_t clksource ) {
	if(clksource) {          //if an external clock is used:
		DDRC &= ~(1 << PORTC6);//input the clock pin
		ASSR |= (1 << EXCLK); //tell timer 2 to use an external clock not external crystal
		ASSR |= (1 << AS2);   //tell timer 2 to use an external clock source
	}
    TCNT2 = 0;               //clear the timer register
    OCR2A = top;             //set the top    	
    TCCR2A = (1 << WGM21);   //set the mode for the timer
    TCCR2B = (prescaler & 0x07); //set up the prescaler, mask the non prescaler bits
}

uint8_t get_timer2 ( void ) {
	return TCNT2;           //return the Timer Counter 2 Register
}

void set_timer2 ( uint8_t v ) {
	TCNT2 = v;             //set the Timer Counter 2 Register
}

void reset_timer2 ( void ) {
	TCNT2 = 0;             //clear the Timer Counter 2 Register, reseting the timer
}

void enable_interrupt_t2 ( void ) {
	TIMSK2 |= (1 << TOIE2);//set the Timer Overflow Interrupt Enable 2 bit
	sei();                 //set the global interrupt flag
}

void disable_interrupt_t2 ( void ) {
	TIMSK2 &= ~(1 << TOIE2);//clear the Timer Overflow Interrupt Enable 2 bit
}

#endif
