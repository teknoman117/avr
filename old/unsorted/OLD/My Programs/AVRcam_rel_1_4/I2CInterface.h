#ifndef I2CINTERFACE_H
#define I2CINTERFACE_H

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
	Module Name: I2CInterface.h
	Module Date: 04/10/2004                         
	Module Auth: John Orlando
	
	Description: This module provides the external interface 
	to the I2C hardware available on the mega8.  It also 
	contains defines and declarations for how the I2C 
	interface will be used in this system.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
***********************************************************/
	
#include "CommonDefs.h"	
	
/*  Structures and Typedefs */
typedef struct
{
	unsigned char configReg;
	unsigned char data;
} i2cCmd_t;
	
/*  Defines   */
/*  The I2C interface will run at 100 KHz (the OV6620 can
	support up to 400 KHz, but this hasn't been tested) */
#define I2C_SPEED 100000


/*  External Function Prototypes */
extern void  I2CInt_init(void);
extern void  I2CInt_writeData(unsigned char address, unsigned char *data, unsigned char bytes);
extern void  I2CInt_readData(unsigned char address, unsigned char *data, unsigned char bytes);
extern bool_t I2CInt_isI2cBusy(void);



#endif	

