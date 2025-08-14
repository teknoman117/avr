/*
    Copyright (C) 2004    John Orlando
    
   AVRcam: a small real-time image processing engine.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

   For more information on the AVRcam, please contact:

   john@jrobot.net

   or go to www.jrobot.net for more details regarding the system.
*/
/***********************************************************
	Module Name: Utility.c
	Module Date: 04/13/2004
	Module Auth: John Orlando
	
	Description: This module provides a basic set of
	general purpose utilities that can be used by any
	module needing them.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
***********************************************************/

/*	Includes */
#include "CommonDefs.h"

/*  Local Variables */

/* 	Local Structures and Typedefs */

/*  Extern Variables */

/*  Definitions */

/***********************************************************
	Function Name: Utility_delay
	Function Description: This function provides a busy-wait
	delay for a specified number of milliseconds.
	Inputs:  numMs - the number of milliseconds to delay
	Outputs: none
	NOTES: The delay_loop_1 and delay_loop_2 functions found
	in avr/delay.h provide accurate 3 and 4 cycle delay loops
	if needed...this isn't really a millisecond, so DON'T
    depend on it for exact timing...
***********************************************************/	
void Utility_delay(unsigned short numMs)
{
	volatile unsigned short i=0,j=0;
#ifndef SIMULATION
	for (i=0; i<numMs; i++)
	{
		for (j=0; j<1000; j++)
		{
			asm volatile("nop"::);
		}
	}
#endif	
}


