/*
 *      bitbang.h
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
 *      This header provides a bitbanged serial port
 * 
 */
 
#ifndef _BITBANG_H
#define _BITBANG_H 1

#ifndef F_CPU 
#warning F_CPU is not defined and is being defaulted to 1 MHz
#define F_CPU 1000000UL
#endif


#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>

void tx_byte( uint8_t* port, uint8_t pin, uint16_t baud, uint8_t byte ) {
	uint8_t cnt;
	uint16_t delay = 1000000/(baud-10);
	*port &= ~(1 << pin);
	_delay_us(delay);
	for(cnt=0;cnt<8;cnt++) {
		if((byte >> cnt) & 0x01) {
			*port |= (1 << pin);
		} else {
			*port &= ~(1 << pin);
		}
		_delay_us(delay);
	}
	*port |= (1 << pin);
	_delay_us(delay);
}

uint8_t rx_byte( uint8_t* pinreg, uint8_t pin, uint16_t baud ) {
	uint8_t cnt, byte=0;
	uint16_t delay = 1000000/(baud-10);
	while((*pinreg >> pin) & 0x01);
	_delay_us(delay);
	for(cnt=0;cnt<8;cnt++) {
		byte |= (((*pinreg >> pin) & 0x01) << cnt);
		_delay_us(delay);
	}
	while(!(*pinreg >> pin) & 0x01);
	_delay_us(delay);
	return byte;
}

void tx_bytes( uint8_t* port, uint8_t pin, uint16_t baud, uint8_t* arr, uint16_t num ) {
	uint16_t cntr;
	for(cntr=0;cntr<num;cntr++) {
		tx_byte(port, pin, baud, arr[cntr]);
	}
}

void rx_bytes( uint8_t* pinreg, uint8_t pin, uint16_t baud, uint8_t* arr ) {

}

void rx_str( uint8_t* pinreg, uint8_t pin, uint16_t baud, char* str ) {

}
 
#endif