/*
####################################################
####                   kr.c                     ####
####    Copyright (C) 2004 Patrick Deegan       ####
####              Psychogenic Inc               ####
####            All Rights Reserved             ####
####                                            ####
####                                            ####
#### This program is free software; you can     ####
#### redistribute it and/or modify it under the ####
#### terms of the GNU General Public License as ####
#### published by the Free Software Foundation; ####
#### either version 2 of the License, or (at    ####
#### your option) any later version.            ####
####                                            ####
#### This program is distributed in the hope    ####
#### that it will be useful,but WITHOUT ANY     ####
#### WARRANTY; without even the implied         ####
#### warranty of MERCHANTABILITY or FITNESS     ####
#### FOR A PARTICULAR PURPOSE.  See the GNU     ####
#### General Public License for more details.   ####
####                                            ####
####################################################
*/
#include <avr/io.h>

#define MYCLOCKSPEED	4000000
#define MYCYCLESPERSECOND 	0.5

#define SCALERVAL ( (MYCLOCKSPEED) / \
		    (33267*8* MYCYCLESPERSECOND));

void busywait(uint16_t numLoops);

void main (void)
{
	volatile enum {UP, DOWN} direction = UP;	
	unsigned char currentValue = 0x01;
	
	uint16_t waitLoops = (uint16_t) SCALERVAL;
	
	/**** I/O Init ****/
	/* use all pins on port B for output */
	DDRB = 0xFF;

	PORTB = 0xFF; /* all lights off */
	
	/* loop forever, main never exits*/
	for (;;) {
		busywait(waitLoops);
		
		if (direction == UP)
		{
			if (currentValue < 128)
			{
				currentValue = 
				    currentValue<<1;
			} else {
				direction = DOWN;
			}
		} else {
			if (currentValue < 2)
			{
				direction = UP;
			} else {
				currentValue = 
				    currentValue>>1;
			}
		}
		
		PORTB = ~currentValue;
	}
	
}

/* busy wait, inneficient but simple */
void busywait(uint16_t numLoops)
{
	uint16_t innerCounter;
	while(numLoops-- > 0)
	{
		innerCounter = 16633;
		while (--innerCounter > 0) {}
	}
	return;
}
