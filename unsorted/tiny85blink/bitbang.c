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
#include <string.h>
#include "bitbang.h"

#define delay (1000000 / BAUD)+2

void txb_b ( volatile uint8_t *port, uint8_t pin, uint8_t byte ) {
	uint8_t cnt;
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

void serout( volatile uint8_t *port, uint8_t pin, uint8_t *bytes, uint8_t num ) {
	uint16_t cntr;
	for( cntr=0; cntr<num; cntr++ ) {
		txb_b( port, pin, bytes[cntr] );
	}
}

void printstr( volatile uint8_t *port, uint8_t pin, char *str ) {
	uint16_t cntr;
	for( cntr=0; cntr<strlen(str); cntr++ ) {
		txb_b( port, pin, (char)str[cntr] );
	}
}

uint8_t rxb_b( volatile uint8_t *port, uint8_t pin ) {
    uint8_t cnt;
    uint8_t b = 0;
    while(*port & (1 << pin));
    _delay_us(delay);
    for(cnt=0;cnt<8;cnt++) {
        if(*port & (1 << pin)) b |= (1 << cnt);
        _delay_us(delay);
    }
    return b;
}

void serin( volatile uint8_t *port, uint8_t pin, uint8_t *bytes, uint8_t num ) {
    uint16_t counter;
    for(counter=0;counter<num;counter++) {
        bytes[counter] = rxb_b( port, pin );
    }
}

int getstr( volatile uint8_t *port, uint8_t pin, char* str, char term ) {
    uint16_t counter=0;
    char b = -1;
    while((b = rxb_b(port,pin)) != term) {
        str[counter++] = b;
    }
    return counter;
}
