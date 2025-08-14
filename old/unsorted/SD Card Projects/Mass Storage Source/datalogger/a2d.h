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

#include <avr/io.h>
#include <stdint.h>

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

void initADC(void);

uint16_t getADC(uint8_t channel);

uint8_t getADC8(uint8_t channel);

#endif
