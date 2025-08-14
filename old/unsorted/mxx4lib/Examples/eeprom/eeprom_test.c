/*
 *      eeprom_test.c
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
 *      This program tests the internal eeprom of the AVR.  It creates a
 *      serial terminal with which to control it and uses the stdio library
 *      for text processing.  It can write single bytes to the eeprom a read
 *      from a specified location.  Try it out!
 * 
 */

#define F_CPU 16000000UL

#include <stdusart0.h>       //functions to interface stdio lib to AVR
#include <eeprom.h>          //avrlib eeprom control
#include <stdio.h>           //stdio library
#include <stdint.h>          //stdint library (uint8_t, uint16_t, etc. types)

int main( void ) {
	init_stdusart0(51, DB8 | P_N | SB1);   //initalize the UART and stdio lib at 19.2 kBaud 8N1 (51 is ubrr for 19.2k at 16MHz
	char cmd;                              //a command variable where commands are stored
	uint16_t addr;                         //a 16 bit variable to store the desired address
	uint16_t byte;                         //a 16 bit variable to store desired data
	printf("AVR Internal EEPROM Test\r");  //print a welcome message
	printf("------Commands------\r");      //print commands
	printf("w - write a byte\r");
	printf("r - read a byte\r");
	printf("--------------------\r:");
	while (1) {		                      //repeat forever
		scanf("%c", &cmd);                 //wait for a command char to be entered
		if( cmd == 'w' ) {                //if write command received
			printf("\rLocation? ");        //ask for location
			scanf("%d", &addr);           //store it
			printf("Data? ");             //request the data
			scanf("%d", &byte);           //store it
			eeprom_write(addr, (uint8_t)(byte & 0xFF)); //write the byte to the eeprom.  since the eeprom is 8 bit, we need to mask the upper 8 bits of the 16 bit variable
			putchar(':');
	    }
	    if( cmd == 'r' ) {            //if read command received
	    	printf("\rLocation? ");   //request location
	    	scanf("%d", &addr);       //store it
	    	byte = eeprom_read(addr); //read the byte
	    	printf("Data at %d is %d\r:", addr, byte);  //display results
	    }
    }
	return 0;
}
