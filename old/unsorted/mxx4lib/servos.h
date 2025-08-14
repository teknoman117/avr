/*
 *      servos.h
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
 *      This header controls up to 8 servos via interrupts
 * 
 */
 
#ifndef _ISERVOS_H
#define _ISERVOS_H 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "timer0ctc.h"

#define NUMSERVOS 3

#ifndef F_CPU 
#warning F_CPU is not defined and is being defaulted to 1 MHz
#define F_CPU 1000000UL
#endif

volatile uint8_t servopins[NUMSERVOS];
volatile uint8_t *servoregs[NUMSERVOS];

volatile uint16_t servo_counter;
volatile uint8_t scntr;
volatile uint16_t servo_positions[NUMSERVOS]; 

ISR( TIMER0_COMPA_vect ) {
	servo_counter++;
	if(servo_counter > speriod) {
		disable_interrupt_t0a();
		servo_counter = 0;
		for(scntr=0;scntr<NUMSERVOS;scntr++) *servoregs[scntr] |= (1 << servopins[scntr]);
		enable_interrupt_t0a();
	}
	
}
 
#endif