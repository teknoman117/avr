/*
 *      timer0.h
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
 *      This header controls the timer counter 0
 * 
 */

#ifndef _TIMER_0_H
#define _TIMER_0_H 1

#include "timercommon.h"
#include <stdint.h>

void init_timer0 ( uint8_t prescaler, uint8_t top );

uint8_t get_timer0 ( void );

void set_timer0 ( uint8_t v );

void reset_timer0 ( void );

void enable_interrupt_t0 ( void );

void disable_interrupt_t0 ( void );

#endif
