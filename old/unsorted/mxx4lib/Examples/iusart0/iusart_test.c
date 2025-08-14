/*
 *      iusart_test.c
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
 *      This program tests the interrupt driven buffering usart
 *      How to use:
 * 
 *      every 100 milliseconds the program checks for data if their is none, it waits
 *      if there is, it displays the write pointer and read pointer of the ring buffer,
 *      and the remaining bytes in the buffer along with the next charactein the buffer.
 *      you can also change the size of the receive buffer and see the pointer's
 *      maximums change.
 * 
 *      Another thing you can do is hold the a key down in your terminal interface,
 *      causing the buffer to start to fill up as you will see it register
 */

#define F_CPU 16000000UL
#define RBUFFER_SIZE 256   //set the receive buffer size to 256 bytes

#include <util/delay.h>
#include <iusart0.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <alloca.h>

int main ( void ) {
	char *str = (char *) alloca ( sizeof(char) * 256 );  //create a 256 element string
	init_iusart0 (51, DB8 | P_N | SB1);             //51 is the ubrr value for 19.2 kBaud at 16 MHz
	writei0((uint8_t*)"Test for Interrupt Driven Uart\r", 31);  //write starting message
	writei0((uint8_t*)"As you enter data it is displayed\r", 34);  //write informational message
	while (1) {                                           //repeat forever
		sprintf(str, "Bytes in buffer: %d; Write Pointer: %d, Read Pointer: %d; Last Character: %c\r", bytesInBuf(), wp, rp, (char) rxbi0()); //create this string with the sprinf command in stdio
        writei0((uint8_t*)str, strlen(str));      //write the string
        _delay_ms(100);                           //wait 100 milliseconds before next check
    } 
	return 0;
}
