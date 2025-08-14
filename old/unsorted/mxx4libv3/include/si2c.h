/*
 *      si2c.h
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
 *      This header controls a software implementation of the I2C bus
 * 
 */

#ifndef _AVR_SI2C_H_
#define _AVR_SI2C_H_

#include <stdint.h>
#include <avr/io.h>

/* Pin Definitions */
#define SCLP      PORTB1
#define SDAP      PORTB0

#define SCLDDR    DDRB
#define SDADDR    DDRB

#define SCLPIN    PINB
#define SDAPIN    PINB

#define SCLPORT   PORTB
#define SDAPORT   PORTB

/* Pin macros */
#define i2c_init() SDAPORT &= ~(1 << SDAP);SCLPORT &= ~(1 << SCLP);
#define SDAON()   SDADDR &= ~(1 << SDAP);
#define SDAOFF()  SDADDR |= (1 << SDAP);
#define SCLON()   SCLDDR &= ~(1 << SCLP);
#define SCLOFF()  SCLDDR |= (1 << SCLP);
#define SDA       ((SDAPIN >> SDAP)&1)
#define SCL       ((SCLPIN >> SCLP)&1)

char i2c_start( uint8_t address );

void i2c_stop();

char i2c_writebyte( uint8_t byte );

uint8_t i2c_readbyte( char ack );

#endif
