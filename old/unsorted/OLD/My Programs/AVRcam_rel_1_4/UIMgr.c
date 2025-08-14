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
	Module Name: UIMgr.c
	Module Date: 04/10/2004
	Module Auth: John Orlando
	
	Description: This module is responsible for providing
	the processing to manage the user interface of the
	system.  This user interface is provided via the UART.
	This module handles the incoming serial commands, and
	performs the needed functionality.  It is then
	responsible for generating any needed response to
	the external entity.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
    11/15/2004     1.2     Updated version string to 1.2.
    1/16/2005      1.4     Added code to write the colorMap
                           to EEPROM one byte at a time, 
                           ensuring that the EEPROM is only
                           written when the data is different
                           than the current value (thus saving
                           EEPROM writes).  Updated version
                           string to 1.4.
***********************************************************/

/*	Includes */
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>
#include "CommonDefs.h"
#include "UIMgr.h"
#include "UartInterface.h"
#include "CamConfig.h"
#include "Utility.h"
#include "Executive.h"
#include "CamInterface.h"

/* 	Local Structures and Typedefs */

typedef enum
{
	getVersionCmd,
	pingCmd,
	setCameraRegsCmd,
	dumpFrameCmd,
	enableTrackingCmd,
	disableTrackingCmd,
	setColorMapCmd,
	resetCameraCmd,
	noCmd,
	invalidCmd
} UIMgr_Cmd_t;

typedef enum
{
	setRed,
	setGreen,
	setBlue
} setColorState_t;
	

/*  Local Variables */
static unsigned char charCount = 0; 
static unsigned char charIndex = 0;
static unsigned char asciiTokenBuffer[MAX_TOKEN_LENGTH+1]; /* +1 to ensure NULL at end */
static unsigned char tokenCount = 0;
static unsigned char tokenBuffer[MAX_TOKEN_COUNT];
static UIMgr_Cmd_t receivedCmd = noCmd;
static unsigned char AVRcamVersion[] = "AVRcam v1.4\r";

/*  Local Function Declaration */
static unsigned char UIMgr_readRxFifo(void);
static unsigned char UIMgr_readTxFifo(void);
static unsigned char UIMgr_readRxFifo(void);
static void UIMgr_sendNck(void);
static void UIMgr_sendAck(void);
static void UIMgr_convertTokenToCmd(void);
static void UIMgr_convertTokenToValue(void);
static void UIMgr_executeCmd(void);

/*  Extern Variables */
unsigned char UIMgr_rxFifo[UI_MGR_RX_FIFO_SIZE];
unsigned char UIMgr_rxFifoHead=0;
unsigned char UIMgr_rxFifoTail=0;

unsigned char UIMgr_txFifo[UI_MGR_TX_FIFO_SIZE];
unsigned char UIMgr_txFifoHead=0;
unsigned char UIMgr_txFifoTail=0;

/*  Definitions */
#define IS_DATA_IN_TX_FIFO() (!(UIMgr_txFifoHead == UIMgr_txFifoTail))
#define IS_DATA_IN_RX_FIFO() (!(UIMgr_rxFifoHead == UIMgr_rxFifoTail))

/* MAX_EEPROM_WRITE_ATTEMPTS limits the number of writes that can be
done to a particular EEPROM cell, so that it can't possible just 
write to the same cell over and over */
#define MAX_EEPROM_WRITE_ATTEMPTS 3

/***********************************************************
	Function Name: UIMgr_init
	Function Description: This function is responsible for
	initializing the UIMgr module.  It sets up the fifo
	used to hold incoming data, etc.
	Inputs:  none 
	Outputs: none
***********************************************************/	
void UIMgr_init(void)
{
	memset(asciiTokenBuffer,0x00,MAX_TOKEN_LENGTH+1);
	memset(tokenBuffer,0x00,MAX_TOKEN_COUNT);
	memset(UIMgr_txFifo,0x00,UI_MGR_TX_FIFO_SIZE);
	memset(UIMgr_rxFifo,0x00,UI_MGR_RX_FIFO_SIZE);
}

/***********************************************************
	Function Name: UIMgr_dispatchEvent
	Function Description: This function is responsible for
	processing events that pertain to the UIMgr.
	Inputs:  event - the generated event
	Outputs: none
***********************************************************/	
void UIMgr_dispatchEvent(unsigned char event)
{
	switch(event)
	{
		case EV_ACQUIRE_LINE_COMPLETE:
			UIMgr_transmitPendingData();
			break;
			
		case EV_SERIAL_DATA_RECEIVED:		
			UIMgr_processReceivedData();
			break;
			
		case EV_SERIAL_DATA_PENDING_TX:
			UIMgr_flushTxBuffer();
			break;
	}
}
/***********************************************************
	Function Name: UIMgr_transmitPendingData
	Function Description: This function is responsible for
	transmitting a single byte of data if data is waiting
	to be sent.  Otherwise, if nothing is waiting, the
	function just returns.
	Inputs:  none 
	Outputs: none
***********************************************************/
void UIMgr_transmitPendingData(void)
{
	if (IS_DATA_IN_TX_FIFO() == TRUE)
	{
		/* data is waiting...send a single byte */
		UartInt_txByte( UIMgr_readTxFifo() );
	}
}
/***********************************************************
	Function Name: UIMgr_processReceivedData
	Function Description: This function is responsible for
	parsing any serial data waiting in the rx fifo
	Inputs:  none 
	Outputs: none
***********************************************************/
void UIMgr_processReceivedData(void)
{
	unsigned char tmpData = 0;

	/* still need to add a mechanism to handle token counts 
	that are excessive!!! FIX ME!!! */
    
	while(IS_DATA_IN_RX_FIFO() == TRUE)
	{
		tmpData = UIMgr_readRxFifo();
		if (tmpData == '\r') 
		{
			/* we have reached a token separator */
			if (tokenCount == 0)
			{
				/* convert the command */
				UIMgr_convertTokenToCmd();				
			}
			else
			{
				/* convert a value */
				UIMgr_convertTokenToValue();
				tokenCount++;
			}
			/* either way, it is time to try to process the received
			token list since we have reached the end of the cmd. */
			Utility_delay(100);
			if (receivedCmd == invalidCmd ||
			     receivedCmd == noCmd )
			{
				UIMgr_sendNck();
				PUBLISH_EVENT(EV_SERIAL_DATA_PENDING_TX);
			}
			else
			{
				UIMgr_sendAck();
				/* publish the serial data pending event, so it
				will push the ACK out before we execute the cmd */
				PUBLISH_EVENT(EV_SERIAL_DATA_PENDING_TX);
				UIMgr_executeCmd();
			}
			
			/* reset any necessary data */
			tokenCount = 0;
			memset(tokenBuffer,0x00,MAX_TOKEN_COUNT);
		}
		else if (tmpData == ' ')  /* space char */
		{
			/* the end of a token has been reached */
			if (tokenCount == 0)
			{
				UIMgr_convertTokenToCmd();
				tokenCount++;   /* check this...why is this being incremented here??? This
                means we have received a token, with tokenCount == 0, which means it is a
                command...why is this contributing to tokenCount?
                This might cause the set color map command to include too much data, since
                it sets the color map based on tokenCount...CHECK*/
			}
			else
			{
				/* check to see if this token is going to push
				us over the limit...if so, abort the transaction */
				if (tokenCount+1 >= MAX_TOKEN_COUNT)
				{
					/* we received too many tokens, and 
					need to NCK this request, since its too
					large...reset everything...*/
					charCount=0;
					charIndex=0;
					tokenCount=0;
					receivedCmd = invalidCmd;
				}
				else
				{
					/* tokenCount is still in range...*/
					UIMgr_convertTokenToValue();
					tokenCount++;
				}
			}
		}
		else if ( (tmpData >= 'A' && tmpData <= 'Z') ||
				   (tmpData >= '0' && tmpData <= '9') )
		{
			/* a valid range of token was received */
			asciiTokenBuffer[charIndex] = tmpData;
			charCount++;
			charIndex++;
			if (charCount > MAX_TOKEN_LENGTH)
			{
				/* we have received a token that cannot be handled...
				set the received cmd to an invalid cmd, and wait
				for the \r to process it */
				receivedCmd = invalidCmd;
				charIndex = 0;  /* ...so we won't overwrite memory */
			}
		}
		else
		{
			/* an invalid character was received */
			receivedCmd = invalidCmd;
		}
	}  /* end while */
	
	asm volatile("clt"::);  /* clear out the T flag in case it wasn't
								 cleared already */
}						

/***********************************************************
	Function Name: UIMgr_executeCmd
	Function Description: This function is responsible for
	executing whatever cmd is stored in the receivedCmd
	object.
	Inputs:  none 
	Outputs: none
***********************************************************/
static void UIMgr_executeCmd(void)
{
	unsigned char i,eepromData, num_writes=0;
	unsigned char *pData;
    unsigned char eeprom_write_succeeded = FALSE;
#if	DEBUG_COLOR_MAP	
	unsigned char asciiBuffer[5];
#endif

	if (receivedCmd == pingCmd) 
	{
	}
	else if (receivedCmd == getVersionCmd)
	{
		pData = AVRcamVersion;
		while(*pData != 0)
		{		
			UIMgr_writeTxFifo(*pData++);
		}
	}		
	else if (receivedCmd == resetCameraCmd)
	{
		CamInt_resetCam();
	}
	else if (receivedCmd == dumpFrameCmd)
	{
		/* publish the event that will indicate that
		a request has come to dump a frame...this will
		be received by the FrameMgr, which will begin
		dumping the frame...a short delay is needed
		here to keep the Java demo app happy (sometimes
		it wouldn't be able to receive the serial data
		as quickly as AVRcam can provide it). */
		Utility_delay(100);
		PUBLISH_EVENT(EV_DUMP_FRAME);
	}
	else if (receivedCmd == setCameraRegsCmd)
	{
		/* we need to gather the tokens and
		build config cmds to be sent to the camera */
		for (i=1; i<tokenCount; i+=2)  /* starts at 1 since first token
											is the CR cmd */
		{
			CamConfig_setCamReg(tokenBuffer[i],tokenBuffer[i+1]);
		}
		CamConfig_sendFifoCmds();
	}
	else if (receivedCmd == enableTrackingCmd)
	{
		/* publish the event...again with a short delay */
		Utility_delay(100);
		PUBLISH_EVENT(EV_ENABLE_TRACKING);
	}
	else if (receivedCmd == disableTrackingCmd)
	{
		PUBLISH_EVENT(EV_DISABLE_TRACKING);
	}
	else if (receivedCmd == setColorMapCmd)
	{
		/* copy the received tokens into the color map */
		for (i=0; i<tokenCount; i++)
		{
			colorMap[i] = tokenBuffer[i+1];
            
            /* write each colorMap byte to EEPROM, but only those
            that changed...this will help reduce wear on the EEPROM */
            eepromData = eeprom_read_byte( (unsigned char*)(i+1));
            if (eepromData != colorMap[i])
            {
                /* need to actually perform the write because the
                data in eeprom is different than the current colorMap */
                eeprom_write_succeeded = FALSE;
                while(eeprom_write_succeeded == FALSE && num_writes < MAX_EEPROM_WRITE_ATTEMPTS)
                {
                    eeprom_write_byte((unsigned char*)(i+1),colorMap[i]);
                    num_writes++;
                    eepromData = eeprom_read_byte( (unsigned char*)(i+1));
                    if (eepromData == colorMap[i])
                    {
                        eeprom_write_succeeded = TRUE;
                    }
                }
                num_writes = 0;
            }
		}

#if	DEBUG_COLOR_MAP			
            			/* for debugging...send out the entire color map */
        UIMgr_txBuffer("\r\n",2);
		for (i=0; i<NUM_ELEMENTS_IN_COLOR_MAP; i++)
		{
			memset(asciiBuffer,0x00,5);
			itoa(colorMap[i],asciiBuffer,10);
			UIMgr_txBuffer(asciiBuffer,3);
			UIMgr_txBuffer(" ",1);
			if (i==15 || i == 31)
			{
				/* break up the output */
				UIMgr_txBuffer("\r\n",2);
			}
		}
#endif			
	}
}

/***********************************************************
	Function Name: UIMgr_convertTokenToValue
	Function Description: This function is responsible for
	converting a received token to a hex value It will
	access the asciiTokenBuffer directly, and store the
	result in the appropriate token buffer.
	Inputs:  none 
	Outputs: none
***********************************************************/	
static void UIMgr_convertTokenToValue(void)
{
	unsigned int newValue;
	
	newValue = atoi(asciiTokenBuffer);
	if (newValue > 255)
	{
		/* the value is too large */
		receivedCmd = invalidCmd;
		tokenBuffer[tokenCount] = 0xFF;  /* to indicate an error */
	}
	else
	{
		/* copy the value into the tokenBuffer */
		tokenBuffer[tokenCount] = newValue;
	}
	memset(asciiTokenBuffer,0x00,MAX_TOKEN_LENGTH);
	charIndex = 0;
	charCount = 0;
}
/***********************************************************
	Function Name: UIMgr_convertTokenToCmd
	Function Description: This function is responsible for
	parsing a received 2-character command.  It will
	access the asciiTokenBuffer directly.
	Inputs:  none 
	Outputs: none
***********************************************************/	
static void UIMgr_convertTokenToCmd(void)
{
	if ( (asciiTokenBuffer[0] == 'P') &&
		 (asciiTokenBuffer[1] == 'G') )
	{
		/* we got a "ping" command...but we still need to see
		if we are going to get the \r */
		receivedCmd = pingCmd;
	}
	else if ( (asciiTokenBuffer[0] == 'G') &&
			   (asciiTokenBuffer[1] == 'V') )
	{
		/* we got the "get version" command */
		receivedCmd = getVersionCmd;
	}
	else if ( (asciiTokenBuffer[0] == 'D') &&
			   (asciiTokenBuffer[1] == 'F') )
	{
		/* we should go into frame dump mode */
		receivedCmd = dumpFrameCmd;	
	}
	else if ( (asciiTokenBuffer[0] == 'C') &&
	           (asciiTokenBuffer[1] == 'R') )
	{
		/* the user wants to set registers in the OV6620 */
		receivedCmd = setCameraRegsCmd;
	}
	else if ( (asciiTokenBuffer[0] == 'E') &&
			   (asciiTokenBuffer[1] == 'T') )
	{
		/* the user wants to enable tracking */
		receivedCmd = enableTrackingCmd;
	}
	else if ( (asciiTokenBuffer[0] == 'S') &&
			   (asciiTokenBuffer[1] == 'M') )
	{
		/* the user wants to set the color map */
		receivedCmd = setColorMapCmd;
	}
	else if ( (asciiTokenBuffer[0] == 'D') &&
			   (asciiTokenBuffer[1] == 'T') )
	{
		receivedCmd = disableTrackingCmd;
	}
	else if ( (asciiTokenBuffer[0] == 'R') &&
			   (asciiTokenBuffer[1] == 'S') )
	{
		receivedCmd = resetCameraCmd;
	}
	else
	{
		/* don't recognize the cmd */
		receivedCmd = invalidCmd;
	}
	memset(asciiTokenBuffer,0x00,MAX_TOKEN_LENGTH);
	charIndex = 0;
	charCount = 0;
}
/***********************************************************
	Function Name: UIMgr_sendAck
	Function Description: This function is responsible for
	queuing up an ACK to be sent to the user.
	Inputs:  none 
	Outputs: none
***********************************************************/	
static void UIMgr_sendAck(void)
{
	UIMgr_writeTxFifo('A');
	UIMgr_writeTxFifo('C');
	UIMgr_writeTxFifo('K');
	UIMgr_writeTxFifo('\r');
}

/***********************************************************
	Function Name: UIMgr_sendNck
	Function Description: This function is responsible for
	queueing up an NCK to be sent to the user.
	Inputs:  none 
	Outputs: none
***********************************************************/	
static void UIMgr_sendNck(void)
{
		UIMgr_writeTxFifo('N');
		UIMgr_writeTxFifo('C');
		UIMgr_writeTxFifo('K');
		UIMgr_writeTxFifo('\r');
}


/***********************************************************
	Function Name: UIMgr_writeBufferToTxFifo
	Function Description: This function is responsible for
	placing "length" bytes into the tx FIFO.
	Inputs:  pData -  a pointer to the data to send
	         length - the number of bytes to send
	Outputs: none
***********************************************************/	
void UIMgr_writeBufferToTxFifo(unsigned char *pData, unsigned char length)
{
	unsigned char tmpHead;
	if (length == 0)
	{
		return;
	}
	
	DISABLE_INTS();
	while(length-- != 0)
	{
		UIMgr_txFifo[UIMgr_txFifoHead] = *pData++;
	
		/* now move the head up */
		tmpHead = (UIMgr_txFifoHead + 1) & (UI_MGR_TX_FIFO_MASK);
		UIMgr_txFifoHead = tmpHead;
	}
	ENABLE_INTS();
}

/***********************************************************
	Function Name: UIMgr_txBuffer
	Function Description: This function is responsible for
	sending 'length' bytes out using the UartInterface 
	module.
	Inputs:  pData -  a pointer to the data to send
	         length - the number of bytes to send
	Outputs: none
***********************************************************/	
void UIMgr_txBuffer(unsigned char *pData, unsigned char length)
{
	while(length-- != 0)
	{
		UartInt_txByte(*pData++); 
	}
}

/***********************************************************
	Function Name: UIMgr_flushTxBuffer
	Function Description: This function is responsible for
	sending all data currently in the serial tx buffer
	to the user.
	Inputs:  none
	Outputs: none
***********************************************************/	
void UIMgr_flushTxBuffer(void)
{
	while(IS_DATA_IN_TX_FIFO() == TRUE)
	{
		UartInt_txByte(UIMgr_readTxFifo() );
	}
}

/***********************************************************
	Function Name: UIMgr_readRxFifo
	Function Description: This function is responsible for
	reading a single byte of data from the rx fifo, and
	updating the appropriate pointers.
	Inputs:  none 
	Outputs: unsigned char-the data read
***********************************************************/	
static unsigned char UIMgr_readRxFifo(void)
{
	unsigned char dataByte, tmpTail;
	
	/* just return the current tail from the rx fifo */
	DISABLE_INTS();
	dataByte = UIMgr_rxFifo[UIMgr_rxFifoTail];	
	tmpTail = (UIMgr_rxFifoTail+1) & (UI_MGR_RX_FIFO_MASK);
	UIMgr_rxFifoTail = tmpTail;
	ENABLE_INTS();
	
	return(dataByte);
}

/***********************************************************
	Function Name: UIMgr_readTxFifo
	Function Description: This function is responsible for
	reading a single byte of data from the tx fifo, and
	updating the appropriate pointers.
	Inputs:  none 
	Outputs: unsigned char-the data read
***********************************************************/	
static unsigned char UIMgr_readTxFifo(void)
{
	unsigned char dataByte, tmpTail;
	
	/* just return the current tail from the tx fifo */
	DISABLE_INTS();
	dataByte = UIMgr_txFifo[UIMgr_txFifoTail];	
	tmpTail = (UIMgr_txFifoTail+1) & (UI_MGR_TX_FIFO_MASK);
	UIMgr_txFifoTail = tmpTail;
	ENABLE_INTS();
	
	return(dataByte);
}

/***********************************************************
	Function Name: UIMgr_writeTxFifo
	Function Description: This function is responsible for
	writing a single byte to the TxFifo and
	updating the appropriate pointers.
	Inputs:  data - the byte to write to the Fifo 
	Outputs: none
***********************************************************/	
void UIMgr_writeTxFifo(unsigned char data)
{
	unsigned char tmpHead;

	DISABLE_INTS();
	UIMgr_txFifo[UIMgr_txFifoHead] = data;

    /* now move the head up */
    tmpHead = (UIMgr_txFifoHead + 1) & (UI_MGR_TX_FIFO_MASK);
    UIMgr_txFifoHead = tmpHead;
	ENABLE_INTS();
	
}

