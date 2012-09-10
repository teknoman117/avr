/*
 *      stdusart_test.c
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
 *      This program demonstrates the stdusart0.h header
 * 
 */

#define F_CPU 16000000UL       //16 MHz clock

#include <stdusart0.h>
#include <stdio.h>

int main ( void ) {
	char name [64];        //create an array of 64 chars called name
	char *n = (char*) name;  //create a pointer to the name array
	init_stdusart0 (51, DB8 | P_N | SB1);  //51 is the ubrr value for 19.2 kBaud at 16 MHz and init for 8N1
	printf("What is your name? ");     //write this string over the usart
	scanf("%s", n);                //wait for a string to be returned
	printf("Hello %s!\r", name);       //write this and insert the string the avr received
	while (1) ;       //loop forever for it is bad practice for an avr to reach the end of a program
	return 0;                          
}
