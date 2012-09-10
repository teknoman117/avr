/*
 *      bitbang.h
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
 *      This header controls a bitbanged serial port
 *
 */

#ifndef _AVR_BITBANG_H_
#define _AVR_BITBANG_H_

#include <stdint.h>

void init_bitbanging( int delay, volatile uint8_t *portp, uint8_t txpin, uint8_t rxpin );

int txb_b ( char c );
int rxb_b( void );

void serout( uint8_t *bytes, uint8_t num );
void serin( uint8_t *bytes, uint8_t num );

void printstr( char *str );
int getstr( char* str, char term );

#ifdef _BITBANG_STDIO_
#include <stdio.h>
void init_bitbanging_stdio( int baud, volatile uint8_t *port, uint8_t tx, uint8_t rx );
#endif

#endif
