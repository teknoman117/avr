/*
 *      usii2c.h
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
 *      This header controls a USI I2C Master
 *
 */

#ifndef _AVR_USII2C_H_
#define _AVR_USII2C_H_

#include <avr/io.h>
#include <stdint.h>

#define USISCL PORTB2
#define USISDA PORTB0
#define USIDDR DDRB
#define USIPORT PORTB
#define USIPIN PINB

#define BYTETRANSFER (1<<USISIF) | (1<<USIOIF) | (1<<USIPF) | (1<<USIDC) | (0x0<<USICNT0)
#define BITTRANSFER (1<<USISIF) | (1<<USIOIF) | (1<<USIPF) | (1<<USIDC) | (0xE<<USICNT0)

#define SYS_CLK   (F_CPU/100)
#define T2_TWI    ((SYS_CLK *1300) /1000000) +1 // >1,3us
#define T4_TWI    ((SYS_CLK * 600) /1000000) +1 // >0,6us

void i2c_init( void );

char i2c_start( uint8_t address );

void i2c_stop();

char i2c_writebyte( uint8_t byte );

uint8_t i2c_readbyte( char ack );

#endif

