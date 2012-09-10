#ifndef UIMGR_H
#define UIMGR_H

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
	Module Name: UIMgr.h
	Module Date: 04/10/2004
	Module Auth: John Orlando
	
	Description: This file provides the external interface
	to the UI Mgr.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
***********************************************************/


/* Defines */
#define UI_MGR_RX_FIFO_SIZE 32 
#define UI_MGR_RX_FIFO_MASK UI_MGR_RX_FIFO_SIZE-1 

#define UI_MGR_TX_FIFO_SIZE 64
#define UI_MGR_TX_FIFO_MASK UI_MGR_TX_FIFO_SIZE-1

#define LENGTH_OF_UI_CMD 		2
#define UI_MGR_MAX_CMD_LENGTH 	32
#define MAX_TOKEN_LENGTH		3   /* needs to hold 0 - 255 in ASCII format */
#define MAX_TOKEN_COUNT			64  /* for setting the color maps */


/* External Variable Reference */
extern unsigned char UIMgr_rxFifo[];
extern unsigned char UIMgr_rxFifoHead;
extern unsigned char UIMgr_rxFifoTail;

/* External Function Declaration */
extern void UIMgr_init(void);
extern void UIMgr_writeBufferToTxFifo(unsigned char *pData, unsigned char length);
extern void UIMgr_txBuffer(unsigned char *pData, unsigned char length);
extern void UIMgr_processReceivedData(void);
extern void UIMgr_transmitPendingData(void);
extern void UIMgr_dispatchEvent(unsigned char event);
extern void UIMgr_flushTxBuffer(void);
extern void UIMgr_writeTxFifo(unsigned char data);
	
#endif	

