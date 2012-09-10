/*
 *      timer2pwm.h
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
 *      This header controls pwm functions on timer 2
 * 
 */

#ifndef _TIMER_2_PWM
#define _TIMER_2_PWM 1

#include "timercommon.h"
#include <stdint.h>

void init_timer2_pwm ( uint8_t prescaler, uint8_t invert );

void set_pwm_2a ( uint8_t value );

void set_pwm_2b ( uint8_t value );

#endif
