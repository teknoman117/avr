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

#include "usartcommon.h"
#include <avr/io.h>
#include <stdint.h>

#ifndef RBUFFER_SIZE
#warning RBUFFER_SIZE not defined, setting to 128
#define RBUFFER_SIZE 128
#endif

#define RBUFFER_MASK ( RBUFFER_SIZE - 1 )
#if ( RBUFFER_SIZE & RBUFFER_MASK )
	#error RX buffer size is not a power of 2
#endif

void flush_buffer ( void );

uint8_t bufferEmpty ( void );

uint8_t bufferFull ( void );

uint16_t bytesInBuf ( void );

void txbi (uint8_t byte);

void writei (uint8_t *bytes, uint16_t number);	

uint8_t rxbi ( void );

void readi (uint8_t* bytes, uint8_t n);

void readie (uint8_t* bytes, uint8_t e);

void init_iusart (uint16_t ubrr, uint8_t specs);

#endif
