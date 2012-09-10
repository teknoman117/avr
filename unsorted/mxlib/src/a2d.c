/*
 *      a2d.c
 *
 *      Copyright 2010 Nathaniel Robert Lewis <linux.robotdude@gmail.com>
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
 *      This header controls the analog comparator on the atmega chips
 *      Example:
 *          initADC();               //init the adc
 *          uint16_t val = getADC(2);  //get the 10 bit output of adc channel 2 and put it in val
 */

#include "a2d.h"
#include <util/delay.h>

void initADC(void) {
    //DIDR0 = 0xFF;             //shut down all digital input on that port
    ADCSRA = ( 1 << ADEN  ) | ( 1 << ADSC  ) | ADC_PRESCALE; //enable the a2d converter
    while ( ADCSRA & ( 1 << ADSC ) );                        //do a dummy conversion
}

uint16_t getADC(uint8_t channel) {
    ADMUX  = ( 1 << REFS0 ) | channel;      //set up the multiplexer to select desired a2d pin
    ADCSRA |=  ( 1 << ADSC  );              //start the conversion
    _delay_us(64);                          //wait 64 microseconds for the flag to register that its not done
    while ( ADCSRA & ( 1 << ADSC ) );       //wait until done
    return ADC;                             //return the 10 bit a2d value
}

uint8_t getADC8 ( uint8_t channel ) {
	uint16_t o = getADC(channel);
	o >>= 2;
	return (o & 0xFF);
}
