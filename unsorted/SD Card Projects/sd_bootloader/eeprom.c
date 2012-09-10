/*
 *      eeprom.c
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

#include "eeprom.h"
#include <avr/io.h>

void eeprom_write ( uint16_t address, uint8_t byte ) {  //write a byte to the AVR EEPROM
	while (EECR & _BV(EEPE)) ;                 //wait until any write operation is complete
	EEAR = address;                            //set the desired address
	EEDR = byte;                               //set the desired byte
	EECR |= ( 1 << EEMPE );                    //enable write operations
	EECR |= ( 1 << EEPE );                     //start the write operation
}

uint8_t eeprom_read ( uint16_t address ) {     //read a byte from the AVR EEPROM
	while (EECR & _BV(EEPE)) ;                 //wait until any write operation is complete
	EEAR = address;                            //set the desired address
	EECR |= ( 1 << EERE );                     //trigger a read operation
	return EEDR;                               //return the data
}

void eeprom_write_page (uint16_t saddress, uint16_t n, uint8_t* bytes) {  //write a string of bytes to EEPROM
	uint16_t c;                                //counter variable
	for(c=0;c<n;c++) {                         //write n number of bytes
	    eeprom_write(saddress+c, bytes[c]);	   //write the next byte in the string
    }
}

void eeprom_read_page (uint16_t saddress, uint16_t n, uint8_t* bytes) {  //read a string of bytes from EEPROM
	uint16_t c;                                //counter variable
	for(c=0;c<n;c++) {                         //read n number of bytes
	    bytes[c] = eeprom_read(saddress+c);	   //read and store the next byte
    }	
}
