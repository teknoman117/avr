/*
 *      timer1sound.h
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
 
#ifndef _TIMER_1_SOUND_
#define _TIMER_1_SOUND_ 1
 
#include <avr/io.h>
#include "timer1ctc.h"

#ifndef F_CPU
#warning F_CPU not defined, defining for 1 MHz
#define F_CPU 1000000UL
#endif

#define BASECLK (F_CPU / 8)

void init_timer1_sound ( void ) {
	init_timer1_ctc( T1_PRESCALER_8, PIN_TOGGLE, PIN_TOGGLE );
}

void setToneA( uint16_t hertz ) {
	set_ocr1a( BASECLK / (hertz * 2) );
}

void setToneB( uint16_t hertz ) {
	set_ocr1b( BASECLK / (hertz * 2) );
}	
 
#endif