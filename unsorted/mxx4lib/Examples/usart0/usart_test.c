/*
 *      usart_test.c
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
 *      This program tests the usart0.h header
 */

#define F_CPU 16000000UL      //16 MHz clock

#include <avr/io.h>
#include <usart0.h>
#include <stdint.h>
#include <string.h>
#include <alloca.h>

int main ( void ) {
	char *rstr = (char*) alloca(sizeof(char) * 128);  //allocate an array of 128 bytes called rstr
	uint8_t cnt=0;                        //counter variable
	for(cnt=0;cnt<128;cnt++) {
		rstr[cnt] = 0;                    //clear out variable 
	}
	init_usart0(51, DB8 | P_N | SB1);     //51 is the ubrr value for 19.2kBaud at 16MHz and data format is 8N1
	write_0s("This is a string of data!\r", 26);  //Transmit this 26 byte string over the usart
	write_0s("Enter a string: ", 16);       //Transmit this 16 byte string
	read_0s((uint8_t*)rstr, 128, '\r');    //read 128 bytes or until a '\r' is received (the enter button) ans store in str
	write_0s("You entered: ", 13);         //write a string
    write_0s(rstr, strlen(rstr));          //write the received message
	while (1) ; 
	return 0;
}
