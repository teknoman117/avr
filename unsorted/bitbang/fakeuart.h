/*
 *      fakeuart.h
 *      
 *      Copyright 2011 Nathaniel Robert Lewis <linux.robotdude@gmail.com>
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
 *      This program creates a fake (software) uart with an interrupt pin (INT2) for RX,
 *      a standard IO pin for TX, and Timer/Counter 2 for a time source.  It also
 *      creates a 128 byte TX buffer and a 128 byte RX buffer to store data
 *
 *      Example:
 *          initFakeUart()     // initalizes the fake uart
 *          txbs('F')               // writes fake to the tx buffer
 */
 
 /**
* This function initalizes the fake uart
*/

void initFakeUart (void);

/**
* This function returns the bytes in the software rx buffer
* @return the count of bytes
*/

unsigned char bytesInBufferS (void);

/**
* This function queues a byte in the transmit buffer
* @param b the byte to queue
*/

void txbs(unsigned char b);

/**
* This function blocks until a byte has been received in the rx buffer and returns it
* @return the first byte on the rx  buffer
*/

unsigned char rxbs();


