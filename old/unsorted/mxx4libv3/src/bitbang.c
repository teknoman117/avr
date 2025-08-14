/*
 *      bitbang.c
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
 *      This header controls a bitbanged serial port
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include "bitbang.h"

volatile uint16_t delay = 0;

volatile uint8_t *port = 0;
uint8_t tx = 0;
uint8_t rx = 0;

void init_bitbanging( int baud, volatile uint8_t *portp, uint8_t txpin, uint8_t rxpin ) {
	port = portp;
	tx = txpin;
	rx = rxpin;
	delay = baud;
}

int txb_b ( char byte ) {
	uint8_t cnt;
	*port &= ~(1 << tx);
	_delay_us(delay);
	for(cnt=0;cnt<8;cnt++) {
		if((byte >> cnt) & 0x01) {
			*port |= (1 << tx);
		} else {
			*port &= ~(1 << tx);
		}
		_delay_us(delay);
	}
	*port |= (1 << tx);
	_delay_us(delay);
	return 0;
}

void serout( uint8_t *bytes, uint8_t num ) {
	uint16_t cntr;
	for( cntr=0; cntr<num; cntr++ ) txb_b( bytes[cntr] );
}

void printstr( char *str ) {
	uint16_t cntr;
	for( cntr=0; str[cntr] != '\0'; cntr++ ) txb_b( str[cntr] );
}

int rxb_b( void ) {
    uint8_t cnt;
    uint8_t b = 0;
    while(*port & (1 << rx));
    _delay_us(delay);
    for(cnt=0;cnt<8;cnt++) {
        if(*port & (1 << rx)) b |= (1 << cnt);
        _delay_us(delay);
    }
    return b;
}

void serin( uint8_t *bytes, uint8_t num ) {
    uint16_t counter;
    for(counter=0;counter<num;counter++) bytes[counter] = rxb_b();
}

int getstr( char* str, char term ) {
    uint16_t counter=0;
    char b = -1;
    while((b = rxb_b()) != term) str[counter++] = b;
    return counter;
}

#ifdef _BITBANG_STDIO_

void init_bitbanging_stdio( int baud, volatile uint8_t *portp, uint8_t txpin, uint8_t rxpin ) {
	init_bitbanging(baud, portp, txpin, rxpin);
	fdevopen((void*)txb_b, (void*)rxb_b);
}

#endif
