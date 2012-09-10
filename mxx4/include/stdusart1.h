/*
 *      stdusart1.h
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
 *      This header is a non buffered uart, but it enables you to use the printf
 *      and scanf commands as you do in PC programming from the stdio.h header
 */

#ifndef _STDUART1_H
#define _STDUART1_H 1

#include "usartcommon.h"

#ifdef UDR1

int txb1 ( char c );

int rxb1 ( void );

void init_stdusart1 (uint16_t ubrr, uint8_t specs);

#else
#warning No second USART on this device disabling stdusart1
#endif

#endif
