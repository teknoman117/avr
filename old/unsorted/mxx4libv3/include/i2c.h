/*
 *      i2c.h
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
 *      This header controls the internal i2c controller (master only)
 * 
 */

#ifndef _I2C_H
#define _I2C_H 1

#include <stdint.h>

#define I2C_PRESCALER_1 0
#define I2C_PRESCALER_4 1
#define I2C_PRESCALER_16 2
#define I2C_PRESCALER_64 3

void init_i2c (uint8_t bitrate, uint8_t prescaler);

int8_t i2c_start ( uint8_t address );

void i2c_stop ( void );

int8_t i2c_txb ( uint8_t b );

uint8_t i2c_rxbAck ( void );

uint8_t i2c_rxbNack ( void );

#endif
