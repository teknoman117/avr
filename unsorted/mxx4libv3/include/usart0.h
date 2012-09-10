/*
 *      usart0.h
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

#include "usartcommon.h"
#include <stdint.h>

void init_usart0(uint16_t ubrr, uint8_t specs);

void txb_0 (uint8_t b);

uint8_t rxb_0 ( void );

void write_0 (uint8_t *bytes, uint16_t n);

void write_0s (char *bytes, uint16_t n);

void read_0 (uint8_t *bytes, uint16_t n);

void read_0s (char *chars, uint16_t n, char stopchar);

#endif

