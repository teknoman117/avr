/*
 *      mmc.c
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
 */

#include <avr/io.h> 
#include "mmc.h"
#include "spi.h"

char init_mmc ( void ) {
	char i;
	PORTB |= (1 << PORTB4);
	for(i=0; i < 10; i++) spi_transfer(0xFF); // send 10*8=80 clock pulses
	PORTB &= ~(1 << PORTB4);          // enable MMC
	if (mmc_command(0x40,0x95,0,0) != 1) return 0; // reset MMC
	while( mmc_command(0x41,0xFF,0,0) != 0 );
	return 1;
}

uint8_t mmc_command( uint8_t befF, uint8_t befH, uint16_t AdrH, uint16_t AdrL ) {
	spi_transfer(0xFF);
	spi_transfer(befF);
	spi_transfer((uint8_t)(AdrH >> 8));
	spi_transfer((uint8_t)AdrH);
	spi_transfer((uint8_t)(AdrL >> 8));
	spi_transfer((uint8_t)AdrL);
	spi_transfer(befH);
	spi_transfer(0xFF);
	return spi_transfer(0xFF);	// return the last received character
}

char mmc_readblock( uint32_t address, uint8_t *sector ) {
	uint16_t al = address & 0x0000FFFF;
	uint16_t ah = (address >> 16) & 0x0000FFFF;
	uint8_t r1 =  mmc_command(0x51, 0xFF, ah, al);
	for (al = 0; al < 50000; al++) {
		if (r1 == 0) break;
		r1 = spi_transfer(0xFF);
	}
	if (r1 != 0) return 1;
	while(spi_transfer(0xFF) != 0xFE);
	for(al=0; al < 512; al++) sector[al] = spi_transfer(0xFF);
	spi_transfer(0xFF);
	spi_transfer(0xFF);
	return 0;
}

char mmc_writeblock( uint32_t address, uint8_t *sector ) {
	uint8_t c;
	uint16_t al = address & 0x0000FFFF;
	uint16_t ah = (address >> 16) & 0x0000FFFF;
	uint8_t r1 =  mmc_command(0x58, 0xFF, ah, al);
	for (al = 0; al < 50000; al++) {
		if (r1 == 0) break;
		r1 = spi_transfer(0xFF);
	}
	if (r1 != 0) return 1;
	spi_transfer(0xFF);
	spi_transfer(0xFF);
	spi_transfer(0xFE);
	for (al=0;al<512;al++) spi_transfer( sector[al] );
	spi_transfer(0xFF);
	spi_transfer(0xFF);
	c = spi_transfer(0xFF);
	c &= 0x1F; 
	if (c != 0x05) return 2;
	while(spi_transfer(0xFF) != 0xFF);
	return 0;
}
