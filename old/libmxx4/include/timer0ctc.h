/*
 *      timer0ctc.h
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
 *      This header controls timer0 for ctc
 * 
 */

#ifndef _TIMER_0_CTC_H
#define _TIMER_0_CTC_H 1

#include "timercommon.h"
#include <stdint.h>

void init_timer0_ctc ( uint8_t prescaler, uint8_t pinfunca, uint8_t pinfuncb );

void set_ocr0a ( uint8_t v );

void set_ocr0b ( uint8_t v );

void enable_interrupt_t0a ( void );

void enable_interrupt_t0b ( void );

void disable_interrupt_t0a ( void );

void disable_interrupt_t0b ( void );

#endif
