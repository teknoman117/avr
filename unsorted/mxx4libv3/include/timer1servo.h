/*
 *      timer1servo.h
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
 */

#ifndef _T1_SERVO
#define _T1_SERVO 1

#include <timercommon.h>
#include <stdint.h>

#ifndef F_CPU
#warning cpu frequency not defined so a default of 1 MHz is selected
#define F_CPU 1000000UL
#endif

#define ticksperus ((F_CPU/8.0)/1000000.0)  //calculate timer ticks that occur in 1us

/**
* This function initalizes timer 1 to control servos
*/

void init_timer1_servos ( void );

/**
* This function sets the pulse length for channel a
* @param us the microseconds of the servo pulse
*/

void set_servo_1a ( uint16_t us );

/**
* This function sets the pulse length for channel b
* @param us the microseconds of the servo pulse
*/

void set_servo_1b ( uint16_t us );

#endif
