/*
 *      spi.h
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
 */

#ifndef _SPI_H
#define _SPI_H 1

#include <avr/io.h>
#include <stdint.h>

//data modes
#define MSBFIRST 0
#define LSBFIRST (1 << DORD)

//modes (detailed on page 160, table 16-2 in the atmega644 datasheet)
#define MODE0 0
#define MODE1 (1 << CHPA)
#define MODE2 (1 << CPOL)
#define MODE3 (1 << CHPA) | (1 << CPOL)

//clock speeds
#define FOSC_2 4
#define FOSC_4 0
#define FOSC_8 5
#define FOSC_16 1
#define FOSC_32 6
#define FOSC_64 2
#define FOSC_128 3

void init_spi ( uint8_t speed, uint8_t mode, uint8_t dtype );

uint8_t spi_transfer ( uint8_t b );

#endif 
