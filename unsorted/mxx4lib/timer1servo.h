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

#include <avr/io.h>
#include <stdint.h>

#ifndef F_CPU
#warning cpu frequency not defined so a default of 1 MHz is selected
#define F_CPU 1000000UL
#endif

#define ticksperus ((F_CPU/8.0)/1000000.0)  //calculate timer ticks that occur in 1us

void init_timer1_servos ( void ) {
    DDRD |= (1 << PORTD4) | (1 << PORTD5);                //set up output pins
    TCNT1 = 0;                                            //set the timer to zero
    ICR1 = ticksperus * 20000;                            //set the maximum timer value for 20ms
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);//set up ocr pins and timer settings
    TCCR1B = (1 << WGM13) | (1 << WGM12) | ( 1 << CS11 ); //set a prescaler of 8 and some timer config
}

void set_servo_1a ( uint16_t us ) {
    OCR1A = us * ticksperus;
}


void set_servo_1b ( uint16_t us ) {
    OCR1B = us * ticksperus;
}

#endif