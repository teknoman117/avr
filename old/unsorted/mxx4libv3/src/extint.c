/*
 *      extint.c
 *      
 *      Copyright 2010 Nathaniel Robert Lewis <linux.robotdude@gmail.com>
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
 */
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "extint.h"
 
void set_mode_int0( uint8_t mode ) {
	EICRA |= (mode & 0x03);
}
 
void set_mode_int1( uint8_t mode ) {
	EICRA |= ((mode << 2) & 0x0C);
}
 
void set_mode_int2( uint8_t mode ) {
	EICRA |= ((mode << 4) & 0x30);
}
 
void set_mask_pcint0( uint8_t mask ) {
	PCMSK0 = mask;
}
 
void set_mask_pcint1( uint8_t mask ) {
	PCMSK1 = mask;
}
 
void set_mask_pcint2( uint8_t mask ) {
	PCMSK2 = mask;
}

void set_mask_pcint3( uint8_t mask ) {
	PCMSK3 = mask;
}
 
void enable_int0( void ) {
	EIMSK |= (1 << INT0);
	sei();
}

void enable_int1( void ) {
	EIMSK |= (1 << INT1);
	sei();
}

void enable_int2( void ) {
	EIMSK |= (1 << INT2);
	sei();
}
 
void enable_pcint0( void ) {
	PCICR |= (1 << PCIE0);
	sei();
}

void enable_pcint1( void ) {
	PCICR |= (1 << PCIE1);
	sei();
}

void enable_pcint2( void ) {
	PCICR |= (1 << PCIE2);
	sei();
}

void enable_pcint3( void ) {
	PCICR |= (1 << PCIE3);
	sei();
}
 
void disable_int0( void ) {
	EIMSK &= ~(1 << INT0);
}

void disable_int1( void ) {
	EIMSK &= ~(1 << INT1);
}

void disable_int2( void ) {
	EIMSK &= ~(1 << INT2);
}
 
void disable_pcint0( void ) {
	PCICR |= ~(1 << PCIE0);
}

void disable_pcint1( void ) {
	PCICR |= ~(1 << PCIE1);
}

void disable_pcint2( void ) {
	PCICR &= ~(1 << PCIE2);
}

void disable_pcint3( void ) {
	PCICR &= ~(1 << PCIE3);
}
