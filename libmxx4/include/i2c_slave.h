/*
 *      i2c_slave.h
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
 *      This header creates an entirely interrupt driven I2C slave
 *      interface for the AVR.  It has a buffer and a master can read
 *      or write data to it without you having to code the transaction
 *      on the slave processor.  Just start this up and check for 
 *      changes.
 * 
 */


#ifndef _I2C_SLAVE_H
#define _I2C_SLAVE_H 1

#include <stdint.h>

#ifndef I2C_BUFFER_SIZE
#define I2C_BUFFER_SIZE 32
#warning I2C_BUFFER_SIZE not defined, setting to 32 bytes
#endif

#define I2C_PRESCALER_1 0
#define I2C_PRESCALER_4 1
#define I2C_PRESCALER_16 2
#define I2C_PRESCALER_64 3

volatile uint8_t i2c_buffer[I2C_BUFFER_SIZE];  //the buffer for I2C
volatile uint8_t i2c_counter;                  //the address counter

void init_i2c_slave ( uint8_t sla, uint8_t bitrate, uint8_t prescaler );

#endif
