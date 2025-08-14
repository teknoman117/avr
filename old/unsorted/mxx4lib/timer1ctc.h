/*
 *      timer1ctc.h
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
 *      This header controls timer 1 under clear timer on compare match 
 *      conditions.  The OCR1A register is the top of the timer, and when
 *      the timer reaches that value, the timer clears, and throws the
 *      compare match interrupt and updates the pin if its enabled
 * 
 */


#ifndef _TIMER_1_CTC
#define _TIMER_1_CTC 1

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

//pin functions for ctc on timer 1
#define PIN_DISCONNECT 0                                  //disconnect pin from timer
#define PIN_TOGGLE 1                                      //toggle pin on compare match
#define PIN_CLEAR 2                                       //clear (set to 0) pin on compare match 
#define PIN_SET 3                                         //set (set to 1) pin on compare match

void init_timer1_ctc ( uint8_t prescaler, uint8_t pinfunca, uint8_t pinfuncb ) {
	if(pinfunca > 0) {DDRD |= (1 << PORTD5);}       //if ocr1a is used, set pin as output
	if(pinfuncb > 0) {DDRD |= (1 << PORTD4);}       //if ocr1b is used, set pin as output
	if(prescaler > 5) {DDRB &= ~(1 << PORTB1);} //if an external clock source is used, set T1 pin as input
	OCR1A = 0;                                            //clear the OCR1x registers
	OCR1B = 0;
    TCCR1A = ((pinfunca << 6) & 0xC0) | ((pinfuncb << 4) & 0x30);  //TCCR1A contains pin function bits, so set them
    TCCR1B = (1 << WGM12) | (prescaler & 0x07);                    //set the prescaler and mode for timer 1 ctc
}

void set_ocr1a ( uint16_t v ) {                           //set output compare register 1a
	OCR1A = v;
}

void set_ocr1b ( uint16_t v ) {                           //set output compare register 1b
	OCR1B = v;
}

void enable_interrupt_t1a ( void ) {                      //enable output compare register 1a interrupt
	TIMSK1 |= (1 << OCIE1A);                              //set enable bit for output compare register 1a interrupt 
	sei();                                                //set global interrupt flag
}

void enable_interrupt_t1b ( void ) {                      //enable output compare register 1b interrupt
	TIMSK1 |= (1 << OCIE1B);                              //set enable bit for output compare register 1b interrupt 
	sei();                                                //set global interrupt flag
}

void disable_interrupt_t1a ( void ) {
	TIMSK1 &= ~(1 << OCIE1A);
}

void disable_interrupt_t1b ( void ) {
	TIMSK1 &= ~(1 << OCIE1B);
}

#endif
