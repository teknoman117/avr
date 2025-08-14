/*
 *      timer0ctc_test.c
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
 *      This program tests the timer0ctc header by changing the state of
 *      and led when the a button connected to T0 has been pressed 4 times.
 *      The timer then resets, and waits for another 4 presses
 * 
 */

#define F_CPU 16000000UL

#include <timer0ctc.h>

int main ( void ) {
	init_timer0_ctc(T0_PRESCALER_EXTERNAL_FALLING, PIN_TOGGLE, PIN_DISCONNECT); //set up timer 0 to clock on the falling edge of T0 and to have ocr0a connected, but to disconnect ocr0b
	set_ocr0a(3);                //set ocr0a to 3, so the timer clears and the ocr0a pin toggles every 4 clocks
	                             //this is because it counts 1-2-3-reset
	while (1) ;                  //wait forever, for hardware takes care of everything
	return 0;
}
