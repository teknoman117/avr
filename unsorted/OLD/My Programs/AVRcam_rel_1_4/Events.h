#ifndef EVENTS_H
#define EVENTS_H

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
	Module Name: Events.h
	Module Date: 05/23/2004
	Module Auth: John Orlando
	
	Description: This file provides the external interface
	to the events that can be published/processed in the
	system.  It is specifically by itself (and nothing
	else should be defined in here) so that both .c and
	.S (assembly) files can include this file without
	a problem.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
    *******************************************************/

/* Definitions */
/* Originally, all events were passed in a bitmask...however,
an event FIFO was finally used, but the coding of the event
definitions were never translated back....doesn't make a
difference, but looks a little weird */
#define EV_SERIAL_DATA_RECEIVED		0x01
#define EV_DUMP_FRAME				0x02
#define EV_PROCESS_FRAME_COMPLETE	0x04
#define EV_PROCESS_LINE_COMPLETE    0x08
#define EV_ACQUIRE_LINE_COMPLETE	0x10
#define EV_ACQUIRE_FRAME_COMPLETE	0x20
#define EV_CONFIGURE_CAMERA			0x40
#define EV_ENABLE_TRACKING			0x80
#define EV_DISABLE_TRACKING			0x81
#define EV_SERIAL_DATA_PENDING_TX	0x90
#define EV_RED_COLOR_MAP_RECEIVED	0x91
#define EV_BLUE_COLOR_MAP_RECEIVED	0x92
#define EV_GREEN_COLOR_MAP_RECEIVED	0x93

/* This is used to pass fast events through the system
so there is a minimum of processing time needed
between lines of tracking data */
#define FEV_ACQUIRE_LINE_COMPLETE 	0x01
#define FEV_PROCESS_LINE_COMPLETE 	0x02

/* This is needed for the event fifo */
#define EXEC_EVENT_FIFO_SIZE 8
#define EXEC_EVENT_FIFO_MASK EXEC_EVENT_FIFO_SIZE-1 

#endif

