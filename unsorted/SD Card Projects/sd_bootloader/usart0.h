/*
 *      usart.h
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
 *
 *      This header controls unbuffered byte strings to and from usart0
 */

#ifndef _USART0_H
#define _USART0_H 1

#include <avr/io.h>
#include <stdint.h>

#define DB8 (UCSZ01 << 1) | (1 << UCSZ00)       //bit combos for 8 data bits
#define DB7 (1 << UCSZ00)                       //bit combos for 7 data bits
#define DB6 (1 << UCSZ00)                       //bit combos for 6 data bits
#define DB5 0                                   //bit combos for 5 data bits

#define P_N 0                                   //bit combos for no parity
#define P_E (1 << UPM01)                        //bit combos for even parity
#define P_O (1 << UPM01) | (1 << UPM00)         //bit combos for odd parity

#define SB1 0                                   //bit combos for 1 stop bit
#define SB2 (1 << USBS0)                        //bit combos for 2 stop bits

void init_usart0(uint16_t ubrr, uint8_t specs);

void txb_0 (uint8_t b);

uint8_t rxb_0 ( void );

void write_0 (uint8_t *bytes, uint16_t n);

void write_0s (char *bytes, uint16_t n);

void read_0 (uint8_t *bytes, uint16_t n);

void read_0s (char *chars, uint16_t n, char stopchar);

#endif

