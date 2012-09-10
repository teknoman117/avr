/*
 *      timer2ctc_test.c
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
 *      This program tests the timer2ctc header by changing the state of
 *      and led when the clocks in TCNT2 = 255. It uses then internal 
 *      clock and has a prescaler of 1024.  The led will be blinking VERY
 *      fast, for it changes this many times:
 *      16000000Hz clock / 1024 prescaler / 256 clocks = ~61 changes per
 *      second
 * 
 */

#define F_CPU 16000000UL

#include <timer2ctc.h>

int main ( void ) {
	init_timer2_ctc(T2_PRESCALER_1024, PIN_TOGGLE, PIN_DISCONNECT, T2_SOURCE_INTERNAL); //set up timer 2 with a prescaler of 1024, to use the internal clock, and to have ocr0a connected, but to disconnect ocr2b
	set_ocr2a(255);              //set ocr2a to 255, so the timer clears and the ocr0a pin toggles
	while (1) ;                  //wait forever, for hardware takes care of everything
	return 0;
}
