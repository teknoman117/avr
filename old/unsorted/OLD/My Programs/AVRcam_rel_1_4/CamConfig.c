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
/**********************************************************
	Module Name: CamConfig.c
	Module Date: 04/10/2004
    Module Auth: John Orlando 
	
	Description: This module is responsible for the 
	high-level configuration activities of the OV6620
	camera module.  This module interfaces with the
	I2CInterface module to perform this configuration.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
    11/15/2004     1.2     Added code to un-tri-state the
                           OV6620's pixel data busses at
                           startup after four seconds.  
                           This was added in to 
                           allow the user to re-program the
                           mega8 at startup if needed.
***********************************************************/

/*  Includes */
#include <avr/io.h>
#include "CamConfig.h"
#include "I2CInterface.h"
#include "CommonDefs.h"
#include "Utility.h"

/**********************************************************/
/*  Definitions */
/* The length of an I2C command is made up of a register address
plus the actual value of the register */
#define SIZE_OF_I2C_CMD 2
#define MAX_NUM_CONFIG_CMDS 8
#define CAM_CONFIG_TX_FIFO_SIZE MAX_NUM_CONFIG_CMDS 
#define CAM_CONFIG_TX_FIFO_MASK CAM_CONFIG_TX_FIFO_SIZE-1

/*  Local Variables */

/*  Local Structures and Typedefs */

/*  Local Function Prototypes */
static i2cCmd_t CamConfig_readTxFifo(void);

/*  Extern Variables */
i2cCmd_t 		CamConfig_txFifo[CAM_CONFIG_TX_FIFO_SIZE];
unsigned char CamConfig_txFifoHead=0;
unsigned char CamConfig_txFifoTail=0;

/***********************************************************
	Function Name: CamConfig_init
	Function Description: This function is responsible for
	performing the initial configuration of the camera.
	Inputs:  none
	Outputs: none
***********************************************************/	
void CamConfig_init(void)
{
	CamConfig_setCamReg(0x14,0x20);  /* reduce frame size */
	CamConfig_setCamReg(0x39,0x40);  /* gate PCLK with HREF */
	CamConfig_setCamReg(0x12,0x28);  /* set RGB mode, with no AWB */
	CamConfig_setCamReg(0x28,0x05);  /* set color sequencer */
    CamConfig_setCamReg(0x13,0x01);  /* un-tri-state the Y/UV lines */
	
	/* send the first four cmds in the I2C fifo */
	CamConfig_sendFifoCmds();	
}


/***********************************************************
	Function Name: CamConfig_setCamReg
	Function Description: This function is responsible for
	creating an I2C cmd structure and placing it into the
	cmd fifo.
	Inputs:  reg - the register to modify
	         val - the new value of the register
	Outputs: none
***********************************************************/	
void CamConfig_setCamReg(unsigned char reg, unsigned char val)
{
	i2cCmd_t cmd;
	
	cmd.configReg = reg;
	cmd.data = val;
#ifndef SIMULATION	
	CamConfig_writeTxFifo(cmd);
#endif	
}
/***********************************************************
	Function Name: CamConfig_sendFifoCmds
	Function Description: This function is responsible for
	sending the entire contents of the config fifo.  This
	function won't return until the configuration process
	is complete (or an error is encountered).
	Inputs:  none
	Outputs: none
	Note: Since this function is written to use the TWI
	interrupt in the I2CInterface module, there will be 
	some busy-waiting here...no big deal, since we end up
	having to trash the frame that we are executing this
	slave write in anyway (since we can't meet the strict
	timing requirements and write i2c at the same time).
***********************************************************/	
void CamConfig_sendFifoCmds(void)
{
	i2cCmd_t cmd;
	
	while (CamConfig_txFifoHead != CamConfig_txFifoTail)
	{
		cmd = CamConfig_readTxFifo();
		I2CInt_writeData(CAM_ADDRESS,&cmd.configReg,SIZE_OF_I2C_CMD);
		Utility_delay(100);		
		/* wait for the I2C transaction to complete */
		while(I2CInt_isI2cBusy() == TRUE);
	} 
}

/***********************************************************
	Function Name: CamConfig_writeTxFifo
	Function Description: This function is responsible for
	adding a new command to the tx fifo.  It adjusts all
	needed pointers.
	Inputs:  cmd - the i2cCmd_t to add to the fifo
	Outputs: bool_t - indicating if writing to the fifo
	         causes it to wrap
***********************************************************/	
bool_t CamConfig_writeTxFifo(i2cCmd_t cmd)
{
	unsigned char tmpHead;
	bool_t retVal = TRUE;
 	
	CamConfig_txFifo[CamConfig_txFifoHead] = cmd;
		
	/* see if we need to wrap */
	tmpHead = (CamConfig_txFifoHead+1) & (CAM_CONFIG_TX_FIFO_MASK);
	CamConfig_txFifoHead = tmpHead;
	
	/* check to see if we have filled up the queue */
	if (CamConfig_txFifoHead == CamConfig_txFifoTail)
	{
		/* we wrapped the fifo...return false */
		retVal = FALSE;
	}
	return(retVal);
}

/***********************************************************
	Function Name: CamConfig_readTxFifo
	Function Description: This function is responsible for
	reading a cmd out of the tx fifo.
	Inputs:  none
	Outputs: i2cCmd_t - the cmd read from the fifo
***********************************************************/	
static i2cCmd_t CamConfig_readTxFifo(void)
{
	i2cCmd_t cmd;
	unsigned char tmpTail;
	
	/* just return the current tail from the rx fifo */
	cmd = CamConfig_txFifo[CamConfig_txFifoTail];	
	tmpTail = (CamConfig_txFifoTail+1) & (CAM_CONFIG_TX_FIFO_MASK);
	CamConfig_txFifoTail = tmpTail;
	
	return(cmd);
}
