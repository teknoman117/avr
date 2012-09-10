/*
 *      timer1_ctc_test.c
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
 *      This program requires that an led be connected to the ocr1a physical
 *      pin.  This program sets up compare match to toggle an led every 
 *      second.  The pin is set to toggle on compare match and the ocr1a
 *      register is set to such a value so that the timer reaches it in 1
 *      second
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <timer1ctc.h>

int main ( void ) {
	init_timer1_ctc (T1_PRESCALER_1024, PIN_TOGGLE, PIN_DISCONNECT);  //set up the timer for an input of fosc/1024 and the ocr1a pin toggles on compare match and the ocr1b pin is disabled
	set_ocr1a (15625);         //this value is how many counter ticks occur in 1 second
	while (1) ;                //wait forever, for the hardware takes care of the led operation
	return 0;
}
