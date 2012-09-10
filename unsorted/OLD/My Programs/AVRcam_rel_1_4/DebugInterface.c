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
	Module Name: DebugInterface.c
	Module Date: 04/15/2004
	Module Auth: John Orlando
	
	Description: This module is responsible for providing a
	debug interface to the system.  Currently, the only
	debugging that is available is through the on-board
	UART (which is used by the main application as well) in
	addition to the LED hooked up at PORTD bit 6.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
***********************************************************/

/*	Includes */
#include "CommonDefs.h"
#include "UartInterface.h"
#include "Utility.h"

/*  Local Variables */

/* 	Local Structures and Typedefs */

/*  Extern Variables */

/*  Definitions */

/***********************************************************
	Function Name: DebugInt_init
	Function Description: This function is responsible for
	initializing the debug module.  It sets up the debug LED
	as well as any other debugging that will be done.  The 
    LED blinks four times, which indicates to the user
    that the system is available for re-programming if 
    necessary.  This works because the data lines on the
    OV6620 are still tri-stated at this point, but won't
    be for long after this function returns.
	Inputs:  none
	Outputs: none
***********************************************************/	
void DebugInt_init(void)
{
	/* set PortD pin6 for output */
	DDRD  |= 0x40;
	/* turn on LED */
	PORTD |= 0x40;
    Utility_delay(500);
    PORTD &= 0xBF;
    Utility_delay(500);
    PORTD |= 0x40;
    Utility_delay(500);
    PORTD &= 0xBF;
    Utility_delay(500);
    PORTD |= 0x40;
    Utility_delay(500);
    PORTD &= 0xBF;
    Utility_delay(500);
    PORTD |= 0x40;
}

