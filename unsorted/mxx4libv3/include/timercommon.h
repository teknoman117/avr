/*
 *      timercommon.h
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
 *      This header contains common things about timers
 * 
 */

#ifndef _TIMER_COMMON_H_
#define _TIMER_COMMON_H_

#include <avr/io.h>

/* Prescalers for Timer 0 */
#define T0_PRESCALER_1 (1 << CS00)                        //fosc / 1
#define T0_PRESCALER_8 (1 << CS01)                        //fosc / 8
#define T0_PRESCALER_64 (1 << CS01) | (1 << CS00)         //fosc / 64
#define T0_PRESCALER_256 (1 << CS02)                      //fosc / 256
#define T0_PRESCALER_1024 (1 << CS02) | (1 << CS00)       //fosc / 1024
#define T0_PRESCALER_EXTERNAL_RISING (1 << CS02) | (1 << CS01) //external clock, clock on rising edge of t0 pin
#define T0_PRESCALER_EXTERNAL_FALLING (1 << CS02) | (1 << CS01) | (1 << CS00) //external clock, clock on falling edge of t0 pin

/* Prescalers for Timer 1 */
#define T1_PRESCALER_1 (1 << CS10)                        //fosc / 1
#define T1_PRESCALER_8 (1 << CS11)                        //fosc / 8
#define T1_PRESCALER_64 (1 << CS11) | (1 << CS10)         //fosc / 64
#define T1_PRESCALER_256 (1 << CS12)                      //fosc / 256
#define T1_PRESCALER_1024 (1 << CS12) | (1 << CS10)       //fosc / 1024
#define T1_PRESCALER_EXTERNAL_RISING (1 << CS12) | (1 << CS11) //external clock, clock on rising edge of t1 pin
#define T1_PRESCALER_EXTERNAL_FALLING (1 << CS12) | (1 << CS11) | (1 << CS10) //external clock, clock on falling edge of t1 pin

/* the predefined PWM types for Timer 1 */
#define T1_PWM_8BIT 255
#define T1_PWM_9BIT 511
#define T1_PWM_10BIT 1023
#define T1_PWM_11BIT 2047
#define T1_PWM_12BIT 4095

/* Prescalers for Timer 2 */
#define T2_PRESCALER_1 (1 << CS20)   // t2clock/1
#define T2_PRESCALER_8 (1 << CS21)   // t2clock/8
#define T2_PRESCALER_32 (1 << CS21) | (1 << CS20) // t2clock/32
#define T2_PRESCALER_64 (1 << CS22)  // t2clock/64
#define T2_PRESCALER_128 (1 << CS22) | (1 << CS20)// t2clock/128
#define T2_PRESCALER_256 (1 << CS22) | (1 << CS21)// t2clock/256
#define T2_PRESCALER_1024 (1 << CS22) | (1 << CS21) | (1 << CS20) // t2clock/1024

/* Timer 2 clock sources */
#define T2_SOURCE_INTERNAL 0
#define T2_SOURCE_EXTERNAL 1

/* Pin functions for CTC */
#define PIN_DISCONNECT 0                              //disconnect pin from timer
#define PIN_TOGGLE 1                                  //toggle pin on compare match
#define PIN_CLEAR 2                                   //clear (set to 0) pin on compare match 
#define PIN_SET 3                                     //set (set to 1) pin on compare match

#endif
