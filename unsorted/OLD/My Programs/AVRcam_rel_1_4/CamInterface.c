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
	Module Name: CamInterface.c
	Module Date: 04/12/2004
	Module Auth: John Orlando
	
	Description: This file is responsible for providing an
	interface to the OV6620 camera hardware.  This includes
	an interface to CamInterface.S for certain low-level,
	optimized camera access routines.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
    11/15/2004     1.2     ifdef'd out the resetCam routine, since
                           resetting the cam now causes the OV6620's
                           clock to not be output (we have to start
                           it up after each reset with the external
                           tiny12 processor).
    1/16/2005      1.4     Ensure that the TCCR1B register is set so
                           nothing is clocking timer1 at startup.                             
***********************************************************/

/*	Includes */
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <string.h>
#include "CommonDefs.h"
#include "CamInterface.h"
#include "Utility.h"
#include "UIMgr.h"
#include "Executive.h"
#include "UartInterface.h"

/*  Local Variables */

/* 	Local Structures and Typedefs */

/*  Definitions */
//#define OUTPUT_INITIAL_COLOR_MAP 1

#define FAST_ACQUIRE 1
#define CAM_G_BUS          	PINB
#define CAM_G_BUS_DIR      	DDRB
#define CAM_RB_BUS          PINC
#define CAM_RB_BUS_DIR 		DDRC

#define CAM_CONTROL_PORT     PORTD
#define CAM_CONTROL_PORT_DIR DDRD
#define CAM_RESET_LINE       BIT7
#define CAM_PIXEL_CLK_COUNT  BIT5
#define CAM_HREF             BIT4
#define CAM_PIXEL_CLK_INT    BIT3
#define CAM_VSYNC            BIT2

/*  Global Variables */
/* NOTE: This file MUST appear first in the Makefile for these variables to
be placed properly in RAM */

/* The colorMap[] table provides the membership lookup table to convert 
RGB or YUV pixel values into actual colors.  The membership table contains
16 elements for each color channel, concatenated together.  The Red (or Y)
value is located in the first 16 bytes, the G (or U) value is located in
the second 16 bytes, and the B (or V) value is located in the last 16 bytes:

    ----------------------------------------------------------------------------------
    |red0|red1|red2|...|red15|green0|green1|green2|...|green15|blue0|blue1|...|blue15|
mem:|0x00 0x01 0x02     0x15   0x16   0x17   0x18       0x31   0x32  0x33       0x47 |
    ---------------------------------------------------------------------------------
Thus, the red lookup is accessed at colorMap+0, the green lookup is accessed 
at colorMap+16, and the blue lookup is accessed at colorMap+32.  */	
unsigned char colorMap[NUM_ELEMENTS_IN_COLOR_MAP] __attribute__ ((section (".noinit")));

/*  Extern Variables */
/* These two buffers hold the current and previous lines
of pixel data.  They are sized to the worst case scenario,
where the color changes between every pixel (unrealistic).
The format of each buffer is for all the even bytes to hold
the run-length, and the odd bytes to hold the color data. */

/* In addition, if we are in frameDump mode, we use these buffers
to store the acquired line data...we are actually grabbing ALL of the
pixels in a line (176) instead of the 88 we get normally during tracking.
But since we have enough to hold 88-RLE blocks, we already have the 176
allocated for this... */
unsigned char currentLineBuffer[LENGTH_OF_LINE_BUFFER];
unsigned char previousLineBuffer[LENGTH_OF_LINE_BUFFER];

/*  Extern Functions */
/* These functions are located in assembly files, and thus
must be externed here so they can be referenced in the source below. */
extern void CamIntAsm_waitForNewTrackingFrame(unsigned char *pBuffer, unsigned char *pMemLookup);
extern void CamIntAsm_waitForNewDumpFrame(unsigned char *pCurrBuffer, unsigned char *pPrevBuffer);
extern void CamIntAsm_acquireTrackingLine(unsigned char *pBuffer, unsigned char *pMemLookup);
extern void CamIntAsm_acquireDumpLine(unsigned char *pCurrBuffer, unsigned char *pPrevBuffer);

/***********************************************************
	Function Name: CamInt_init
	Function Description: This function is responsible
	for initializing the camera interface.  This includes
	setting up the i/o ports that are used to read the
	camera busses, as well as resetting the camera.
	Inputs:  none
	Outputs: none
***********************************************************/	
void CamInt_init(void)
{
#if OUTPUT_INITIAL_COLOR_MAP
	unsigned char asciiBuffer[5];
    unsigned char i;
#endif    
    
	/* set up the mega8 ports that will be interfacing
	with the camera */	
	CAM_CONTROL_PORT_DIR |= (1<<CAM_RESET_LINE); /* cam reset is output */
	CAM_CONTROL_PORT_DIR |= 0x80;   /* set just the MSB as an output */
	CAM_CONTROL_PORT_DIR &= 0xFB;   /* make sure bit2 is clear (input) */
	CAM_CONTROL_PORT &= 0x7F;   /* set reset line low */
	CAM_G_BUS_DIR &= 0xF0;  /* 4-bit G bus all inputs */
    CAM_G_BUS_DIR |= 0xF0;  /* disable the pull-up on PB4 and PB5 */
	CAM_RB_BUS_DIR &= 0xF0;  /* 4-bit RB bus all inputs */
	
    /* ensure that timer1 is disabled to start...eventually, when PCLK needs
    to feed timer1 through the external counter, it will be enabled on an
    "as needed" basis...*/
	TCCR1B &= ~( (1<<CS12)|(1<<CS11)|(1<<CS10) );
	
	/* we'll turn on the interrupt after we assign the initial TCNT value */
	
	/* set up External Interrupt1 to interrupt us on rising edges (HREF)...
	this is needed to indicate when the first pixel of each line is about to start, so
	we can synch up with it...this interrupt will be disabled once HREF goes high */
	
	MCUCR |= (1<<ISC11) | (1<<ISC10);  /* rising edge interrupt */
	/* the interrupt will be enabled when we are ready to detect the rising edge of
	HREF...its now primed and ready to go */
	
	/* set up External Interrupt0 to interrupt us on rising edges (VSYNC) */
	MCUCR |= (1<<ISC01) | (1<<ISC00);	/* rising edge interrupt */ 
	GICR  |= (1<<INT0);    /* interrupt request enabled */
	
	/* set up TimerO to count and be clocked from an external pulse source
	(HREF) on falling edges...eventually, we need to enable the interrupt
	for this!  FIX THIS */
	TCCR0 = (1<<CS02)|(1<<CS01)|(0<<CS00);
	
	/* setting up the PCLK counter with Timer1 will be done right after
	we start receiving pixels in each line...we sacrifice the first pixel
	in each line, but we'll account for it...*/
	
	/* set up the mega8 so that its sleep mode puts it in an IDLE sleep
	mode, where it can wake up as fast as possible */
	set_sleep_mode(SLEEP_MODE_IDLE);
	/* umm....we need to actually enable the sleep mode...*/
	MCUCR |= 0x80;
	
	/* initialize the memLookup table */
	memset(colorMap,0x00,NUM_ELEMENTS_IN_COLOR_MAP);   
	
	/* read the color map out of EEPROM */
	eeprom_read_block(colorMap, (unsigned char*)0x01,NUM_ELEMENTS_IN_COLOR_MAP);

#if OUTPUT_INITIAL_COLOR_MAP    
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

#ifndef NO_CRYSTAL
	CamInt_resetCam();	
#endif    
}

/***********************************************************
	Function Name: CamInt_resetCam
	Function Description: This function is responsible
	for resetting the camera.  This is accomplished by
	toggling the reset line on the OV6620 for ~100 mS.
	Inputs:  none
	Outputs: none
    IMPORTANT NOTE: This function has effectively been removed
    since resetting the camera now causes the camera to not
    output the clock signal.  Thus, if we reset the cam, the
    AVR has no clock, and thus doesn't run...
***********************************************************/	
void CamInt_resetCam(void)
{

#if 0
	CAM_CONTROL_PORT |= (1<<CAM_RESET_LINE); /* cam reset line high */
	Utility_delay(500);
	CAM_CONTROL_PORT &= (0<<CAM_RESET_LINE); /* cam reset line low */
	Utility_delay(100);
#endif    
}


