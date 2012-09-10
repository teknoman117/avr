/*
 *      stdusart0.h
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

#ifndef _STDUART0_H
#define _STDUART0_H 1

#include "usartcommon.h"

int txb0 ( char c );

int rxb0 ( void );

void init_stdusart0 (uint16_t ubrr, uint8_t specs);

#endif
