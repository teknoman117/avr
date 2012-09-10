/*
 *      timer1.h
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
 *      This header initalizes timer 1 as a timer and allows interrupts to
 *      be enabled for Timer Overflow
 * 
 */


#ifndef _TIMER_1
#define _TIMER_1 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

//the prescalers for timer 1 and their bit values
#define T1_PRESCALER_1 (1 << CS10)                        //fosc / 1
#define T1_PRESCALER_8 (1 << CS11)                        //fosc / 8
#define T1_PRESCALER_64 (1 << CS11) | (1 << CS10)         //fosc / 64
#define T1_PRESCALER_256 (1 << CS12)                      //fosc / 256
#define T1_PRESCALER_1024 (1 << CS12) | (1 << CS10)       //fosc / 1024
#define T1_PRESCALER_EXTERNAL_RISING (1 << CS12) | (1 << CS11) //external clock, clock on rising edge of t1 pin
#define T1_PRESCALER_EXTERNAL_FALLING (1 << CS12) | (1 << CS11) | (1 << CS10) //external clock, clock on falling edge of t1 pin

void init_timer1 ( uint8_t prescaler, uint16_t top ) {
    if(prescaler > 5) {DDRB &= ~(1 << PORTB1);} //if an external clock source is used, set T1 pin as input
	TCNT1 = 0;                           //set the timer to zero
	ICR1 = top;                          //set the maximum timer value
	TCCR1A = 0;                          //TCCR1A needs to be zero
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (prescaler & 0x07);//mask the bits in the prescaler variable that aren't used at set timer to reset at the value in ICR1
}	

void enable_interrupt_t1 ( void ) {
	TIMSK1 |= (1 << TOIE1);             //enable the timer 1 overflow interrupt
    sei();                              //set the global interrupt flag
}
 
void disable_interrupt_t1 ( void ) {
	TIMSK1 &= ~(1 << TOIE1);            //clear the timer 1 overflow interrupt enable bit
} 
 
uint16_t get_timer1 ( void ) {
    return TCNT1;                       //return the value of the Timer Counter 1 register
}

void set_timer1 ( uint16_t value ) {
	TCNT1 = value;                      //set the value of timer counter 1 register to a desired value
}

void reset_timer1 ( void ) {
	TCNT1 = 0;                          //set the timer counter 1 register to zero, reseting the timer
}

#endif
