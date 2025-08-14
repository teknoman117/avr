/*
####################################################
####                kitt.cpp                    ####
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


/** Kitt Class
**  Represents a once-famous car with a series of 
**  LEDs on the hood.
**  You can tell the car is alive when it cycles 
**  it's LEDs, lighting each in turn, from left 
**  to right and then back again.
*/
class Kitt {
	public:
		Kitt();
		void cycleLEDs();
	private:
		void busywait(uint16_t numLoops);
		
		uint16_t waitLoops;
		enum {UP, DOWN} currentDirection;
		unsigned char currentValue;
};


Kitt::Kitt()
{
	waitLoops = (uint16_t) SCALERVAL;
	currentDirection = UP;
	currentValue = 1;
	
	DDRB = 0xFF;
	PORTB = 0xFF;
}


/* cycleLEDs -- performs a single back and forth, 
** illuminating each LED in turn twice
*/
void Kitt::cycleLEDs()
{
	while (currentDirection == UP 
		|| currentValue > 1)
	{
		busywait(waitLoops);
		if (currentDirection == UP)
		{

			if (currentValue < 128)
			{
				currentValue = 
				currentValue<<1;
			} else {
				currentDirection 
				             = DOWN;
			}
		} else {
			if (currentValue > 1)
			{
				currentValue = 
				    currentValue>>1;
			}
		}
		
		PORTB = ~currentValue;
	}
	
	/* when we reach this point, direction is 
	   DOWN and value is 1 */
	   
	/* change direction for next cycle */
	currentDirection = UP; 
	
	busywait(waitLoops);

	return;
}

void Kitt::busywait(uint16_t numLoops)
{
	uint16_t innerCounter;
	while(numLoops-- > 0)
	{
		innerCounter = 16633;
		while (--innerCounter > 0) {}
	}
	return;
}

/* Main driver.  Simply cycles LEDs without end */
int main (void)
{
	Kitt myCar;
	
	for(;;)
	{
		myCar.cycleLEDs();
	}
	
	return 0;
}
		
