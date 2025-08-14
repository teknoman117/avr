/*
 *      spi.c
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

#include "spi.h"

void init_spi ( uint8_t speed, uint8_t mode, uint8_t dtype ) {
	DDRB |= (1 << PORTB5) | (1 << PORTB7);//set MOSI and SCK as outputs
	DDRB &= ~(1 << PORTB6);               //set MISO as an input
	SPSR = (speed >> 2) & 0x01;
	SPCR = (1 << SPE) | (1 << MSTR) | mode | dtype | (speed & 0x03); //setup the spi controller	
}

uint8_t spi_transfer ( uint8_t b ) {
    SPDR = b;                             //set the data byte and start the transfer
	while (!(SPSR & ( 1 << SPIF)));       //wait for the transfer to complete
	return SPDR;                          //return received data
} 
