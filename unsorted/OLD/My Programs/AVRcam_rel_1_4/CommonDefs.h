#ifndef COMMONDEFS_H
#define COMMONDEFS_H

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
	Module Name: CommonDefs.h
	Module Date: 04/10/2004
	Module Auth: John Orlando
	
	Description: This module provides some of the generic
	definitions and declarations needed by the AVRcam 
	system.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
    11/15/2004     1.2     Added the NO_CRYSTAL definition
                           for when the OV6620 is providing
                           the mega8's clock source.
***********************************************************/
 
#include <avr/interrupt.h>
	
/*  Defines  */
/* 	FOSC defines the frequency of the crystal connected to
	the mega8, supplying the instruction clock cycle */
#define FOSC 16000000

/* For defining boolean operations */
typedef unsigned char bool_t;
#define TRUE  1
#define FALSE 0 

#define BIT0 0
#define BIT1 1
#define BIT2 2 
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7

#define DISABLE_INTS() cli()
#define ENABLE_INTS()  sei()

//#define SIMULATION 	1
//#define SLOW_FRAMES	1
//#define DEBUG_FRAME_DATA 1
//#define DEBUG_TRACKED_LINE 1
//#define DEBUG_COLOR_MAP 1
#define COMPLETE_COLOR_MAP 1
#define NO_CRYSTAL 1
	
#endif	

