#ifndef CAMINTERFACE_H
#define CAMINTERFACE_H

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
	Module Name: CamInterface.h
	Module Date: 04/14/2004
	Module Auth: John Orlando
	
	Description: This file is responsible for providing the
	external interface to the CamInterface module.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
***********************************************************/

#define NUM_PIXELS_IN_A_TRACKED_LINE 	88
#define NUM_PIXELS_IN_A_DUMP_LINE    	176
#define ACTUAL_NUM_PIXELS_IN_A_LINE		176
#define ACTUAL_NUM_LINES_IN_A_FRAME		144
#define LENGTH_OF_RUN_LENGTH_BLOCK 2
#define LENGTH_OF_LINE_BUFFER NUM_PIXELS_IN_A_DUMP_LINE

#define WAIT_FOR_VSYNC_LOW() 	while( (PIND & 0x04) != 0)
#define WAIT_FOR_VSYNC_HIGH()	while( (PIND & 0x04) == 0)

#define WAIT_FOR_HREF_LOW()		while( (PIND & 0x10) != 0)
#define WAIT_FOR_HREF_HIGH()	while( (PIND & 0x10) == 0)

#define WAIT_FOR_PCLK_LOW()		while( (PIND & 0x20) != 0)
#define WAIT_FOR_PCLK_HIGH()	while( (PIND & 0x20) == 0)

#define NUM_COLOR_STEPS     16
#define NUM_COLOR_CHANNELS	3
#define NUM_ELEMENTS_IN_COLOR_MAP (NUM_COLOR_STEPS * NUM_COLOR_CHANNELS)

#define RED_OFFSET		0
#define GREEN_OFFSET	16
#define BLUE_OFFSET		32

extern unsigned char currentLineBuffer[];
extern unsigned char previousLineBuffer[];
extern unsigned char colorMap[];
extern unsigned char frameCount;
	
/* Extern functions */
extern void CamInt_init(void);
extern void CamInt_resetCam(void);
extern void CamInt_waitForNewDumpFrame(void);
extern void CamInt_waitForNewTrackingFrame(void);
extern void CamInt_acquireTrackingLine(void);
extern void CamInt_acquireDumpLine(void);
	
#endif	

