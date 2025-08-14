/*
 *      t1servotest.c
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
 *      This program tests servo control using timer1 to generate the pulses
 *      so the AVR is under no load keeping the servos in position
 * 
 */

#define F_CPU 11059200UL
#include <util/delay.h>
#include <timer1servo.h>

int main ( void ) {
    DDRC = DDRD = 0xFF;
	init_timer1_servos();
	int servoc;
	while (1) {
		PORTC = 255;
		for(servoc=750;servoc<2250;servoc++) {
			set_servo_1a(servoc*ticksperus);
			set_servo_1b(servoc*ticksperus);
		    _delay_ms(1);
		}
		PORTC = 0;
		for(servoc=2250;servoc>750;servoc--) {
			set_servo_1a(servoc*ticksperus);
			set_servo_1b(servoc*ticksperus);
		    _delay_ms(1);
		}
    }
	return 0;
}