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
	Module Name: Main.c
	Module Date: 04/10/2004
	Module Auth: John Orlando
	 
	Description:  This module is responsible for providing 
	the entry point to the code through the "main" function.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
***********************************************************/

/*	Includes */
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "UIMgr.h"
#include "UartInterface.h" 
#include "I2CInterface.h"
#include "CamInterface.h"
#include "DebugInterface.h"
#include "FrameMgr.h"
#include "CommonDefs.h" 
#include "CamConfig.h" 
#include "Executive.h" 
#include "Utility.h"

/* 	Local Structures and Typedefs */

/*  Extern Variables */

/*  Definitions */

/***********************************************************
	Function Name: main
	Function Description: This function provides the entry
	point into AVRcam application.
	Inputs:  none
	Outputs: int
***********************************************************/	
int main(void)
{
	/* initialize all of the interface modules */
	DebugInt_init();
	UartInt_init();
	I2CInt_init();
	CamInt_init();
	
	/* initialize the remaining modules that will process
	data...interrupts need to be on for these */
	ENABLE_INTS();
	CamConfig_init(); 
	UIMgr_init();
	FrameMgr_init();
    
	/* provide a short delay for the camera to stabilize before
	we let the executive start up */
	Utility_delay(1000);
	
	/* the rest of the application will be under the
	control of the Executive.  */
	Exec_run();	
	
	/* this should never be reached */
	return(0);
}

