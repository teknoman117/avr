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
	Module Name: Executive.c
	Module Date: 04/12/2004
	Module Auth: John Orlando
	
	Description: This file is responsible for implementing a
	minimalist event dispatcher.  It keeps track of an event
	fifo that waits for new events to come in, and dispatches
	them to any entities that care about them.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
    1/16/2005      1.4     Fixed issue where the interrupts weren't
                           being turned off when the fastEventBitmask
                           was being accessed.  Also removed redundant
                           interrupt masking when accessing the
                           main event fifo.  Also fixed issue where
                           the main event fifo wasn't being checked
                           for events if an event was pending in
                           the fast event fifo.
***********************************************************/

/*	Includes */
#include <stdlib.h>
#include "CommonDefs.h"
#include "Executive.h"
#include "FrameMgr.h"
#include "CamInterface.h"
#include "UIMgr.h"
#include "UartInterface.h"
#include "CamConfig.h"
#include "Utility.h"

/*  Local Variables */
unsigned char Exec_eventFifo[EXEC_EVENT_FIFO_SIZE];
unsigned char Exec_eventFifoHead=0;
unsigned char Exec_eventFifoTail=0;

/*  Local Function Definitions */
static unsigned char Exec_readEventFifo(void);

/* 	Local Structures and Typedefs */

/*  Extern Variables */
/* This bitmask holds events that need to be processed as fast as possible */
unsigned char fastEventBitmask = 0x00;

/*  Definitions */
#define IS_DATA_IN_EVENT_FIFO() (!(Exec_eventFifoHead == Exec_eventFifoTail))
/***********************************************************
	Function Name: Exec_run
	Function Description: This function is responsible for
	running the main control loop.  The control loop is 
	based on checking both the fast-event bitmask (for high
    priority events) and the event FIFO to determine if an
    event needs to be handled.  The event is then dispatched
    to the appropriate handler.
	Inputs:  none
	Outputs: none
***********************************************************/	
void Exec_run(void)
{
	unsigned char eventGenerated;
	
	while(1)
	{
		if (fastEventBitmask)
		{
			/* an event needing fast processing has been received */
			/* a received line needs to be processed...this
			needs to be processed as quickly as possible */
			if (fastEventBitmask & FEV_ACQUIRE_LINE_COMPLETE)
			{
                DISABLE_INTS();
				fastEventBitmask &= ~FEV_ACQUIRE_LINE_COMPLETE;	
                ENABLE_INTS();
				FrameMgr_processLine();				
			
				/* also check if serial data needs to be sent
				out through UIMgr */
				UIMgr_transmitPendingData();	

				/* we can't just call acquire line again here,
				since we don't know if we need to acquire another
				line or not (it depends on the FrameMgr to figure
				this out) */
			}
			if (fastEventBitmask & FEV_PROCESS_LINE_COMPLETE)
			{
                DISABLE_INTS();
				fastEventBitmask &= ~FEV_PROCESS_LINE_COMPLETE;
                ENABLE_INTS();
				FrameMgr_acquireLine();
			}
		}		
		
        if (IS_DATA_IN_EVENT_FIFO() == TRUE)		
		{			
            eventGenerated = Exec_readEventFifo();
			switch(eventGenerated)
			{
				case (EV_DUMP_FRAME):
					FrameMgr_dispatchEvent(eventGenerated);
					break;
					
				case (EV_ENABLE_TRACKING):
					FrameMgr_dispatchEvent(eventGenerated);
					break;
					
				case (EV_DISABLE_TRACKING):
					FrameMgr_dispatchEvent(eventGenerated);
					break;
					
				case (EV_ACQUIRE_LINE_COMPLETE):
					FrameMgr_dispatchEvent(eventGenerated);
					UIMgr_dispatchEvent(eventGenerated);
					break;
					
				case (EV_ACQUIRE_FRAME_COMPLETE):				
					FrameMgr_dispatchEvent(eventGenerated);
					break;
					
				case (EV_PROCESS_LINE_COMPLETE):
					FrameMgr_dispatchEvent(eventGenerated);
					break;
				
				case (EV_PROCESS_FRAME_COMPLETE):
					FrameMgr_dispatchEvent(eventGenerated);
					break;
					
				case (EV_SERIAL_DATA_RECEIVED):
					UIMgr_dispatchEvent(eventGenerated);
					FrameMgr_dispatchEvent(eventGenerated);
					break;																

				case (EV_SERIAL_DATA_PENDING_TX):
					UIMgr_dispatchEvent(eventGenerated);
					break;
								
				default:		
					break;
			}			
		}
        
        /* toggle the debug line */

	}
}

/***********************************************************
	Function Name: Exec_readEventFifo
	Function Description: This function is responsible for
	reading a single event out of the event fifo.
	Inputs:  none 
	Outputs: unsigned char-the data read
***********************************************************/	
static unsigned char Exec_readEventFifo(void)
{
	unsigned char dataByte, tmpTail;
	
	DISABLE_INTS();
	/* just return the current tail from the tx fifo */
	dataByte = Exec_eventFifo[Exec_eventFifoTail];	
	tmpTail = (Exec_eventFifoTail+1) & (EXEC_EVENT_FIFO_MASK);
	Exec_eventFifoTail = tmpTail;
	ENABLE_INTS();
	
	return(dataByte);
}

/***********************************************************
	Function Name: Exec_writeEventFifo
	Function Description: This function is responsible for
	writing a single event to the event fifo and
	updating the appropriate pointers.
	Inputs:  data - the byte to write to the Fifo 
	Outputs: none
***********************************************************/	
void Exec_writeEventFifo(unsigned char event)
{
	unsigned char tmpHead;

	DISABLE_INTS();
	Exec_eventFifo[Exec_eventFifoHead] = event;

    /* now move the head up */
    tmpHead = (Exec_eventFifoHead + 1) & (EXEC_EVENT_FIFO_MASK);
    Exec_eventFifoHead = tmpHead;
	ENABLE_INTS();
}

