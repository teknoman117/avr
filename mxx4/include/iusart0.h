/*
 *      iusart0.h
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
 *      This header controls the usart and buffers incoming data by way of
 *      an interrupt service routine
 * 
 */

#ifndef _IUSART0_H
#define _IUSART0_H 1

#include <avr/io.h>
#include <stdint.h>

#include "usartcommon.h"

#ifndef RBUFFER_SIZE0
#warning RBUFFER_SIZE0 not defined, setting to 128
#define RBUFFER_SIZE0 128
#endif

#define RBUFFER_MASK0 ( RBUFFER_SIZE0 - 1 )
#if ( RBUFFER_SIZE0 & RBUFFER_MASK0 )
	#error RX buffer0 size is not a power of 2
#endif

#ifndef TBUFFER_SIZE0
#warning TBUFFER_SIZE0 not defined, setting to 128
#define TBUFFER_SIZE0 128
#endif

#define TBUFFER_MASK0 ( TBUFFER_SIZE0 - 1 )
#if ( TBUFFER_SIZE0 & TBUFFER_MASK0 )
	#error TX buffer0 size is not a power of 2
#endif

void flush_buffer0 ( void );

uint8_t bufferEmpty0 ( void );

uint8_t bufferFull0 ( void );

uint16_t bytesInBuf0 ( void );

void txbi0 (uint8_t byte);

void writei0 (uint8_t *bytes, uint16_t number);	

void writeStringi0 (char *string);

uint8_t rxbi0 ( void );

void readi0 (uint8_t* bytes, uint8_t n);

void readi0e (uint8_t* bytes, uint8_t e);

int16_t peeki0 (uint8_t depth);   

void init_iusart0 (uint16_t ubrr, uint8_t specs);

#endif
