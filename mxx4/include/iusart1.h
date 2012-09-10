/*
 *      iusart1.h
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

#ifndef _IUSART1_H
#define _IUSART1_H 1

#include <avr/io.h>
#include <stdint.h>

#include "usartcommon.h"

#ifdef UDR1

#ifndef RBUFFER_SIZE1
#warning RBUFFER_SIZE1 not defined, setting to 128
#define RBUFFER_SIZE1 128
#endif

#define RBUFFER_MASK1 ( RBUFFER_SIZE1 - 1 )
#if ( RBUFFER_SIZE1 & RBUFFER_MASK1 )
	#error RX buffer1 size is not a power of 2
#endif

#ifndef TBUFFER_SIZE1
#warning TBUFFER_SIZE1 not defined, setting to 128
#define TBUFFER_SIZE1 128
#endif

#define TBUFFER_MASK1 ( TBUFFER_SIZE1 - 1 )
#if ( TBUFFER_SIZE1 & TBUFFER_MASK1 )
	#error TX buffer1 size is not a power of 2
#endif

void flush_buffer1 ( void );

uint8_t bufferEmpty1 ( void );

uint8_t bufferFull1 ( void );

uint16_t bytesInBuf1 ( void );

void txbi1 (uint8_t byte);

void writei1 (uint8_t *bytes, uint16_t number);	

void writeStringi1 (char *string);

uint8_t rxbi1 ( void );

void readi1 (uint8_t* bytes, uint8_t n);

void readi1e (uint8_t* bytes, uint8_t e);

void init_iusart1 (uint16_t ubrr, uint8_t specs);

#else 
#warning no second usart on this device disabling usart1 code
#endif

#endif
