/*
 *      timer1.h
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
 *      This header initalizes timer 1 as a timer and allows interrupts to
 *      be enabled for Timer Overflow
 * 
 */


#ifndef _TIMER_1
#define _TIMER_1 1

#include <stdint.h>
#include "timercommon.h"

void init_timer1 ( uint8_t prescaler, uint16_t top );

void enable_interrupt_t1 ( void );
 
void disable_interrupt_t1 ( void );
 
uint16_t get_timer1 ( void );

void set_timer1 ( uint16_t value );

void reset_timer1 ( void );

#endif
