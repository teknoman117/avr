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
	MCUCR |= (mode & 0x03);
}

void set_mode_int1( uint8_t mode ) {
	MCUCR |= ((mode << 2) & 0x0C);
}


void enable_int0( void ) {
	GICR |= (1 << INT0);
	sei();
}

void enable_int1( void ) {
	GICR |= (1 << INT1);
	sei();
}

void disable_int0( void ) {
	GICR &= ~(1 << INT0);
}

void disable_int1( void ) {
	GICR &= ~(1 << INT1);
}

#ifdef INT2

void set_mode_int2( uint8_t mode ) {
	MCUCSR |= ((mode << 6) & 0x40);
}

void enable_int2( void ) {
	GICR |= (1 << INT2);
	sei();
}

void disable_int2( void ) {
	GICR &= ~(1 << INT2);
}

#endif
