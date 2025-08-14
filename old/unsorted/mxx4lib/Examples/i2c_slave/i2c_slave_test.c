/*
 *      i2c_slave_test.c
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
 *      This program emulates an i2c eeprom and can be used with the Example
 *      program for the i2c header on a second AVR.  If the value in register
 *      0 is greater than 1, an led on portb.0 lights up.
 * 
 */

#define I2C_BUFFER_SIZE 256  //define the size of the i2c buffer

#include <i2c_slave.h>

int main ( void ) {
	DDRB = (1 << PORTB0);                        //set portb.0 as an output
	init_i2c_slave(0xa0, 255, I2C_PRESCALER_1);  //initalize the i2c slave
	while (1) {                                  //the i2c slave is completely interrupt driven, but use values
	    if(i2c_buffer[0] > 0) { PORTB = (1 << PORTB0); } //if register 0 greater than 1, turn on portb.0
	    else { PORTB = 0; }                      //otherwise, clear portb.0
	}
	return 0;
}
