#ifndef CAMCONFIG_H
#define CAMCONFIG_H

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
	Module Name: CamConfig.h
	Module Date: 04/12/2004
	Module Auth: John Orlando
	
	Description: This file provides the external interface
	to the CamConfig module.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
***********************************************************/

#include "I2CInterface.h"
	
/* This is the I2C slave address of the OV6620 module.
NOTE: The actual address of the camera is 0xC0.  However,
the TWI interface on the mega8 needs to have the address
shifted one bit to the right, which yields 0x60.  The
LSB is used to set whether the operation is read or 
write. */
#define CAM_ADDRESS 0x60	
	
extern i2cCmd_t CamConfig_txFifo[];
extern unsigned char 	CamConfig_txFifoHead;
extern unsigned char 	CamConfig_txFifoTail;
extern bool_t 			CamConfig_txComplete;

extern void  	CamConfig_init(void);
extern bool_t 	CamConfig_writeTxFifo(i2cCmd_t cmd);
extern void  	CamConfig_setCamReg(unsigned char reg, unsigned char val);
extern void  	CamConfig_sendFifoCmds(void);
	
#endif	

