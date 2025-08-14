/*
 *      usartcommon.h
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
 *      This header contains standard settings for the AVR's USART
 * 
 */

#ifndef _USART_COMMON_H_
#define _USART_COMMON_H_

#include <avr/io.h>

#ifndef F_CPU
#warning F_CPU not defined so setting F_CPU to 1 MHz
#define F_CPU 1000000
#endif

#define BAUD(b) ((F_CPU / (16 * b)) - 1)

#define DB8 (UCSZ1 << 1) | (1 << UCSZ0)       //bit combos for 8 data bits
#define DB7 (1 << UCSZ1)                       //bit combos for 7 data bits
#define DB6 (1 << UCSZ0)                       //bit combos for 6 data bits
#define DB5 0                                   //bit combos for 5 data bits

#define P_N 0                                   //bit combos for no parity
#define P_E (1 << UPM1)                        //bit combos for even parity
#define P_O (1 << UPM1) | (1 << UPM0)         //bit combos for odd parity

#define SB1 0                                   //bit combos for 1 stop bit
#define SB2 (1 << USBS)                        //bit combos for 2 stop bits

#endif
