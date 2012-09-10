/*
 *      a2d.h
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
 *      This header controls the analog comparator on the atmega chips  
 *      Example:
 *          initADC();               //init the adc
 *          uint16_t val = getADC(2);  //get the 10 bit output of adc channel 2 and put it in val
 */

#ifndef _A2D_H
#define _A2D_H 1

#ifndef F_CPU
#warning F_CPU not defined setting to 1mHz
#define F_CPU 1000000UL
#endif

//defines the max frequency of the ADC CLK input in hertz
#define ADC_MAX_CLK 200000                 //max adc clock frequency for 10 bit resolution as defined by datasheet

//prescalers used to drop F_CPU to =>200KHz for ADC CLK
#define ADC_PRESCALE_2 ( 1 << ADPS0 )
#define ADC_PRESCALE_4 ( 1 << ADPS1 )
#define ADC_PRESCALE_8 (( 1 << ADPS0 ) | ( 1 << ADPS1 ))
#define ADC_PRESCALE_16 ( 1 << ADPS2 )
#define ADC_PRESCALE_32 (( 1 << ADPS0 ) | ( 1 << ADPS2 ))
#define ADC_PRESCALE_64 (( 1 << ADPS2 ) | ( 1 << ADPS1 ))
#define ADC_PRESCALE_128 (( 1 << ADPS0 ) | ( 1 << ADPS1 ) | ( 1 << ADPS2 ))

//a macro to select the proper adc prescaler
#if ((F_CPU / 2) <= ADC_MAX_CLK )
#define ADC_PRESCALE ADC_PRESCALE_2
#elif ((F_CPU / 4) <= ADC_MAX_CLK )
#define ADC_PRESCALE ADC_PRESCALE_4
#elif ((F_CPU / 8) <= ADC_MAX_CLK )
#define ADC_PRESCALE ADC_PRESCALE_8
#elif ((F_CPU / 16) <= ADC_MAX_CLK )
#define ADC_PRESCALE ADC_PRESCALE_16
#elif ((F_CPU / 32) <= ADC_MAX_CLK )
#define ADC_PRESCALE ADC_PRESCALE_32
#elif ((F_CPU / 64) <= ADC_MAX_CLK )
#define ADC_PRESCALE ADC_PRESCALE_64
#elif ((F_CPU / 128) <= ADC_MAX_CLK )
#define ADC_PRESCALE ADC_PRESCALE_128
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void initADC(void) {
    DIDR0 = 0xFF;             //shut down all digital input on that port
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

#endif
