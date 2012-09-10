/*
 *      acomp.h
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


#ifndef _ACOMP_H
#define _ACOMP_H

#include <stdint.h>

/**
* This function initalizes the Analog Comparator inside the AVR
* @param multiplex A value of 0 will disable multiplexing and 1 will enable it
* @param channel The default ADC channel to use for the negative line if multiplexing is enabled
*/

void initACOMP (uint8_t multiplex, uint8_t channel);

/**
* This function changes the ADC channel to use as the negative line
* @param channel The channel to use
*/
void switchChannel (uint8_t channel);

/** 
* This function returns the result of the analog to digital converter
* @return 0 - negative line is greater, 1 - positive line is greater
*/
uint8_t getACOMP (void);

#endif
