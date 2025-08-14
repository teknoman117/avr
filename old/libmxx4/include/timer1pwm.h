/*
 *      timer1pwm.h
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
 *      This header controls timer 1 pwm functions
 * 
 */

#ifndef _TIMER_1_PWM
#define _TIMER_1_PWM 1

#include "timercommon.h"
#include <stdio.h>

void init_timer1_pwm ( uint8_t prescaler, uint16_t top, uint8_t invert );

void set_pwm_1a ( uint16_t value );

void set_pwm_1b ( uint16_t value );

#endif
