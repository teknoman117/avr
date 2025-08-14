;
;    Copyright (C) 2004    John Orlando
;    
;   AVRcam: a small real-time image processing engine.

;    This program is free software; you can redistribute it and/or
;    modify it under the terms of the GNU General Public
;    License as published by the Free Software Foundation; either
;    version 2 of the License, or (at your option) any later version.

;    This program is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;    General Public License for more details.

;    You should have received a copy of the GNU General Public
;    License along with this program; if not, write to the Free Software
;    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

;   For more information on the AVRcam, please contact:

;   john@jrobot.net

;   or go to www.jrobot.net for more details regarding the system.
;**********************************************************************
;       Module Name: CanInterfaceAsm.S
;       Module Date: 04/14/2004
;       Module Auth: John Orlando
;
;       Description: This module provides the low-level interface
;       to the OV6620 camera hardware.  It is responsible for
;   	acquiring each pixel block (R,G,B), performing the mapping
;       into an actual color (orange, purple, etc), run-length
;       encoding the data, and storing the info off to the appropriate
;       line buffer.  This routine is synchronized with the pixel data
;       so that no polling of the camera data needs to be done (the
;       OV6620 is clocked off of the same crystal source as the mega8,
;       thus providing inherent synchronization between the two).
;
;       Revision History:
;       Date        Rel Ver.    Notes
;       4/10/2004      0.1     Module created
;       6/30/2004      1.0     Initial release for Circuit Cellar
;                              contest.
;       1/16/2005      1.4     Fixed issue with the TCCR1B register
;                              where PCLK was getting routed to the
;                              timer1 even when it wasn't needed.
;                              This caused excessive counter overflow
;                              interrupts, and caused problems.  Now,
;                              the "PCLK" pipe feeds timer1 when needed,
;                              and is turned off when it isn't needed.

#include <avr/io.h>
#include "Events.h"
		
		.extern fastEventBitmask    ; This is the flag used to indicate to the rest
									; of the system that the line is complete
								
#define HREF_INTERRUPT_ENABLE_MASK   0x80
#define HREF_INTERRUPT_DISABLE_MASK  0x7F
#define ENABLE_PCLK_TIMER1_OVERFLOW_BITMASK  0x04
#define DISABLE_PCLK_TIMER1_OVERFLOW_BITMASK 0xFB
#define G_PORT						_SFR_IO_ADDR(PINC)  
#define RB_PORT						_SFR_IO_ADDR(PINB)  
#define PIXEL_RUN_START_INITIAL     0x50     	; This value causes our pixel counter (TCNT1)
												; to overflow after 176 (horizontal) pixels

#define RED_MEM_OFFSET				0x00
#define GREEN_MEM_OFFSET			0x10
#define BLUE_MEM_OFFSET				0x20

; A pixelBlock is defined as a contiguous group of 4 pixels that are combined 
; together to form a specific color.  Typically, this is formed by sampling a
; a green value, followed by a red and blue value (since we are dealing
; with Bayer color data).  We could optionally sample a second green with
; the red and average the greens, because the eye is more sensitive to
; green, but for speed we don't do this.  These three values (RGB) are then
; used as indices into the color membership lookup table (memLookup) to
; determine which color the pixelBlock maps into.  The memLookup table is
; manually generated for now (though it will hopefully be modified over
; the serial interface eventually).
;
; Here is a pixel block:
; ...G  G  G  G...  (row x)
; ...B  R  B  R...  (row x+1)
;    |  |  |  |--this is skipped 
;    |  |  |--this is skipped
;    |  |--this is sampled
;    |--this is sampled

; As pixel blocks are sampled, the red, green, and blue values are
; used to index into their respective color maps.  The color maps
; return values that can be logically ANDed together so that a 
; particular RGB triplet will result in a single bit being set
; after the AND operation.  This single bit indicates which color
; the RGB triplet represents.  It is also possible for no bits to
; be set after the AND process, indicating that the RGB triplet
; does not map to any of the colors configured in the color map.
; This isn't quite as fast as a pure RGB lookup table, but
; it then again it doesn't require 2^12 (4-bits for each color
; channel) bytes to store the lookup table.  It takes just a few
; more cycles, and only requires 48 bytes of precious RAM (16
; per color channel, since our resolution on each color channel
; is only 4-bits).  Not bad....for more information, see:
; http://www.cs.cmu.edu/~trb/papers/wirevision00.pdf for more
; information on this color segmentation technique.

; One other note: this code does depend on the colorMap residing
; at a well-defined position in memory; specifically, it mus
; start at a 256-byte boundary so that the lowest byte in the
; map is set to 0x00.  Currently, the colorMap is forced to
; start at RAM location 0x300.  This could potentially be changed
; by the developer if needed, but offsets would have to be added
; in to the colorMap look-up code below to make it work.


; These are the registers that will be used throughout this
; module for acquiring each line of pixel data
pixelCount			= 16
pixelRunStart		= 17
lastColor     		= 18
tmp1				= 19	; be sure to not use tmp1 and color simultaneously
tmp2				= 20
color           	= 19
greenData       	= 20
blueData        	= 21
colorMapLow	  		= 22
colorMapHigh		= 23
prevLineBuffLow  	= 22  	; overlaps with memLookupLow (but orthogonal)
prevLineBuffHigh	= 23	; overlaps with memLookupHigh (but orthogonal)
currLineBuffLow     = 24
currLineBuffHigh  	= 25

        .section .text

; These are the global assembly function names that are accessed via other
; C functions
        .global CamIntAsm_waitForNewTrackingFrame
		.global CamIntAsm_waitForNewDumpFrame
		.global CamIntAsm_acquireDumpLine
		.global CamIntAsm_acquireTrackingLine
		.global SIG_INTERRUPT0
		.global SIG_INTERRUPT1
		.global SIG_OVERFLOW0
		.global SIG_OVERFLOW1
		
;*****************************************************************		
;   	Function Name: CamIntAsm_waitForNewTrackingFrame
;       Function Description: This function is responsible for
;       going to sleep until a new frame begins (indicated by
;    	VSYNC transitioning from low to high.  This will wake
;       the "VSYNC sleep" up and allow it to continue with 
;       the acquireLine function, where the system waits for
;       an "HREF sleep" that we use to synchronize with the
;       data.  
;       Inputs:  r25 - MSB of currentLineBuffer
;                r24 - LSB of currentLineBuffer
;				 r23 - MSB of colorMap
; 				 r22 - LSB of colorMap
;       Outputs: none
;       NOTES: This function doesn't really return...it sorta just
;       floats into the acquireLine function after the "VSYNC sleep"
;       is awoken, then begins processing the line data.  Once
;		176 pixels are sampled (and the counter overflows), then
;		an interrupt will occur, the 'T' bit in the SREG will be
;		set, and the function will return.
;*****************************************************************
		
CamIntAsm_waitForNewTrackingFrame:
		sbi		_SFR_IO_ADDR(PORTD),PD6  ; For testing...
		cbi		_SFR_IO_ADDR(PORTD),PD6		
		sleep

;*****************************************************************
; REMEMBER...everything from here on out is critically timed to be
; synchronized with the flow of pixel data from the camera...
;*****************************************************************

CamIntAsm_acquireTrackingLine:
		brts	_cleanUp
		;sbi		_SFR_IO_ADDR(PORTD),PD6 ; For testing...
		;cbi		_SFR_IO_ADDR(PORTD),PD6
        
        in      tmp1,_SFR_IO_ADDR(TCCR1B) ; Enable the PCLK line to actually
        ori     tmp1, 0x07                 ; feed Timer1
        out     _SFR_IO_ADDR(TCCR1B),tmp1 
										; The line is about to start...		
		ldi     pixelCount,0			; Initialize the RLE stats...
		ldi		pixelRunStart,PIXEL_RUN_START_INITIAL  	; Remember, we always calculate
														; the pixel run length as
														; TCNT1L - pixelRunStart
		
		ldi		lastColor,0x00				; clear out the last color before we start
		
		mov   	XH,currLineBuffHigh    	; Load the pointer to the current line
		mov		XL,currLineBuffLow		; buffer into the X pointer regs		 
		
		mov   	ZH,colorMapHigh      	; Load the pointers to the membership
		mov		ZL,colorMapLow			; lookup tables (ZL and YL will be overwritten
		mov 	YH,colorMapHigh	 		; as soon as we start reading data) to Z and Y
		
		in		tmp1, _SFR_IO_ADDR(TIMSK)			; enable TIMER1 to start counting
		ori		tmp1, ENABLE_PCLK_TIMER1_OVERFLOW_BITMASK 	; external PCLK pulses and interrupt on 
		out		_SFR_IO_ADDR(TIMSK),tmp1			; overflow
		
		ldi 	tmp1,PIXEL_RUN_START_INITIAL	; set up the TCNT1 to overflow (and
		ldi 	tmp2,0xFF 						; interrupts) after 176 pixels		
		out 	_SFR_IO_ADDR(TCNT1H),tmp2		
		out 	_SFR_IO_ADDR(TCNT1L),tmp1				
		
		mov		YL,colorMapLow		
		
		in 		tmp1, _SFR_IO_ADDR(GICR)	; enable the HREF interrupt...remember, we
											; only use this interrupt to synchronize
											; the beginning of the line
		ori 	tmp1, HREF_INTERRUPT_ENABLE_MASK
		out		_SFR_IO_ADDR(GICR), tmp1
		
;*******************************************************************************************
;   Track Frame handler 
;*******************************************************************************************		
		
_trackFrame:		
		sbi		_SFR_IO_ADDR(PORTD),PD6
		sleep   ; ...And we wait...
		
	; Returning from the interrupt/sleep wakeup will consume
	; 14 clock cycles (7 to wakeup from idle sleep, 3 to vector, and 4 to return)	

	; Disable the HREF interrupt
		cbi		_SFR_IO_ADDR(PORTD),PD6
		in 		tmp1, _SFR_IO_ADDR(GICR)
		andi 	tmp1, HREF_INTERRUPT_DISABLE_MASK
		out		_SFR_IO_ADDR(GICR), tmp1
		
	; A couple of NOPs are needed here to sync up the pixel data...the number (2)
	; of NOPs was determined emperically by trial and error.
		nop
		nop
_acquirePixelBlock:							;							Clock Cycle Count
		in		ZL,RB_PORT         			; sample the red value (PINB)		(1)
		in		YL,G_PORT         			; sample the green value (PINC)		(1)
		andi	YL,0x0F            			; clear the high nibble				(1)
		ldd		color,Z+RED_MEM_OFFSET  	; lookup the red membership			(2)
		in		ZL,RB_PORT         			; sample the blue value (PINB)		(1)
		ldd		greenData,Y+GREEN_MEM_OFFSET; lookup the green membership		(2)
		ldd		blueData,Z+BLUE_MEM_OFFSET	; lookup the blue membership		(2)
		and		color,greenData 			; mask memberships together			(1)
		and		color,blueData  			; to produce the final color		(1)
		brts    _cleanUpTrackingLine		; if some interrupt routine has		(1...not set)
											; come in and set our T flag in 
											; SREG, then we need to hop out
											; and blow away this frames data (common cleanup)									
		cp		color,lastColor     		; check to see if the run continues	(1)
		breq    _acquirePixelBlock  		;									(2...equal)
											;									___________
											;								16 clock cycles 		
											; (16 clock cycles = 1 uS = 1 pixelBlock time)
		
		; Toggle the debug line to indicate a color change
		sbi     _SFR_IO_ADDR(PORTD),PD6
		nop
		cbi		_SFR_IO_ADDR(PORTD),PD6
		
		mov		tmp2,pixelRunStart				; get the count value of the
												; current pixel run
		in		pixelCount,_SFR_IO_ADDR(TCNT1L)	; get the current TCNT1 value 
		mov   	pixelRunStart,pixelCount		; reload pixelRunStart for the
												; next run
		sub		pixelCount,tmp2     			; pixelCount = TCNT1L - pixelRunStart
										
		st		X+,lastColor			; record the color run in the current line buffer
		st		X+,pixelCount			; with its length
		mov		lastColor,color			; set lastColor so we can figure out when it changes
		
		nop								; waste one more cycle for a total of 16
		rjmp	_acquirePixelBlock	
		
; _cleanUpTrackingLine is used to write the last run length block off to the currentLineBuffer so
; that all 176 pixels in the line are accounted for.
_cleanUpTrackingLine:		
		ldi		pixelCount,0xFF		; the length of the last run is ALWAYS 0xFF minus the last
		sub		pixelCount,pixelRunStart  	; pixelRunStart
		
		inc		pixelCount				; increment pixelCount since we actually need to account
										; for the overflow of TCNT1
										
		st		X+,color				; record the color run in the current line buffer
		st		X,pixelCount		
		rjmp	_cleanUp
		
_cleanUpDumpLine:		
		; NOTE: If serial data is received, to interrupt the tracking of a line, we'll
		; get a EV_SERIAL_DATA_RECEIVED event, and the T bit set so we will end the
		; line's processing...however, the PCLK will keep on ticking for the rest of
		; the frame/line, which will cause the TCNT to eventually overflow and
		; interrupt us, generating a EV_ACQUIRE_LINE_COMPLETE event.  We don't want
		; this, so we need to actually turn off the PCLK counting each time we exit
		; this loop, and only turn it on when we begin acquiring lines....
        ; NOT NEEDED FOR NOW...
		;in		tmp1, _SFR_IO_ADDR(TIMSK)			; disable TIMER1 to stop counting
		;andi	tmp1, DISABLE_PCLK_TIMER1_OVERFLOW_BITMASK 	; external PCLK pulses
		;out		_SFR_IO_ADDR(TIMSK),tmp1

_cleanUp:
        ; Disable the external clocking of the Timer1 counter 
        in      tmp1, _SFR_IO_ADDR(TCCR1B)
        andi    tmp1, 0xF8
        out     _SFR_IO_ADDR(TCCR1B),tmp1
		
		; Toggle the debug line to indicate the line is complete
		sbi     _SFR_IO_ADDR(PORTD),PD6
		cbi		_SFR_IO_ADDR(PORTD),PD6
		clt				; clear out the T bit since we have detected
						; the interruption and are exiting to handle it
_exit:
		ret
		
;*****************************************************************		
;   	Function Name: CamIntAsm_waitForNewDumpFrame
;       Function Description: This function is responsible for
;       going to sleep until a new frame begins (indicated by
;    	VSYNC transitioning from low to high.  This will wake
;       the "VSYNC sleep" up and allow it to continue with 
;       acquiring a line of pixel data to dump out to the UI.
;       Inputs:  r25 - MSB of currentLineBuffer
;                r24 - LSB of currentLineBuffer
;				 r23 - MSB of prevLineBuffer
;				 r22 - LSB of prevLineBuffer
;       Outputs: none
;       NOTES: This function doesn't really return...it sorta just
;       floats into the acquireDumpLine function after the "VSYNC sleep"
;       is awoken.
;*****************************************************************		
CamIntAsm_waitForNewDumpFrame:
		sbi		_SFR_IO_ADDR(PORTD),PD6  ; For testing...
		cbi		_SFR_IO_ADDR(PORTD),PD6
		sleep

;*****************************************************************
; REMEMBER...everything from here on out is critically timed to be
; synchronized with the flow of pixel data from the camera...
;*****************************************************************

CamIntAsm_acquireDumpLine:
		brts	_cleanUp
		;sbi		_SFR_IO_ADDR(PORTD),PD6 ; For testing...
		;cbi		_SFR_IO_ADDR(PORTD),PD6
		
		mov   	XH,currLineBuffHigh    	; Load the pointer to the current line
		mov		XL,currLineBuffLow		; buffer into the X pointer regs

		mov		YH,prevLineBuffHigh		; Load the pointer to the previous line
		mov		YL,prevLineBuffLow  	; buffer into the Y pointer regs
		
		ldi 	tmp1,PIXEL_RUN_START_INITIAL	; set up the TCNT1 to overflow (and
		ldi 	tmp2,0xFF 						; interrupts) after 176 pixels		
		out 	_SFR_IO_ADDR(TCNT1H),tmp2		
		out 	_SFR_IO_ADDR(TCNT1L),tmp1		
		
        in      tmp1, _SFR_IO_ADDR(TCCR1B) ; Enable the PCLK line to actually
        ori     tmp1, 0x07                 ; feed Timer1
        out     _SFR_IO_ADDR(TCCR1B),tmp1
        nop
        
		in		tmp1, _SFR_IO_ADDR(TIMSK)			; enable TIMER1 to start counting
		ori		tmp1, ENABLE_PCLK_TIMER1_OVERFLOW_BITMASK 	; external PCLK pulses and interrupt on 
		out		_SFR_IO_ADDR(TIMSK),tmp1			; overflow			
		
		in 		tmp1, _SFR_IO_ADDR(GICR)	; enable the HREF interrupt...remember, we
											; only use this interrupt to synchronize
											; the beginning of the line
		ori 	tmp1, HREF_INTERRUPT_ENABLE_MASK
		out		_SFR_IO_ADDR(GICR), tmp1
		
;*******************************************************************************************
;   Dump Frame handler 
;*******************************************************************************************		
		
_dumpFrame:		
		sbi		_SFR_IO_ADDR(PORTD),PD6
		sleep   ; ...And we wait...

		cbi		_SFR_IO_ADDR(PORTD),PD6
		in 		tmp1, _SFR_IO_ADDR(GICR)			; disable the HREF interrupt
		andi 	tmp1, HREF_INTERRUPT_DISABLE_MASK  	; so we don't get interrupted
		out		_SFR_IO_ADDR(GICR), tmp1			; while dumping the line
	
		nop		; Remember...if we ever remove the "cbi" instruction above,
				; we need to add two more NOPs to cover this
    
; Ok...the following loop needs to run in 8 clock cycles, so we can get every
; pixel in the line...this shouldn't be a problem, since the PCLK timing was
; reduced by a factor of 2 whenever we go to dump a line (this is to give us
; enough time to do the sampling and storing of the pixel data).  In addition,
; it is assumed that we will have to do some minor processing on the data right
; before we send it out, like mask off the top 4-bits of each, and then pack both
; low nibbles into a single byte for transmission...we just don't have time to
; do that here (only 8 instruction cycles :-)  )
_sampleDumpPixel:
		in		tmp1,G_PORT				; sample the G value					(1)
		in		tmp2,RB_PORT			; sample the R/B value					(1)
		st		X+,tmp1					; store to the currLineBuff and inc ptrs(2)
		st		Y+,tmp2					; store to the prevLineBuff and inc ptrs(2)
		brtc	_sampleDumpPixel		; loop back unless flag is set			(2...if not set)
										;									___________
										;									8 cycles normally
																			
		; if we make it here, it means the T flag is set, and we must have been interrupted
		; so we need to exit (what if we were interrupted for serial? should we disable it?)
		rjmp	_cleanUpDumpLine

;***********************************************************
;	Function Name: <interrupt handler for External Interrupt0> 
;	Function Description: This function is responsible
;	for handling a rising edge on the Ext Interrupt 0.  This
;	routine simply returns, since we just want to wake up
;	whenever the VSYNC transitions (meaning the start of a new
;	frame).
;	Inputs:  none
;	Outputs: none
;***********************************************************
SIG_INTERRUPT0:
; This will wake us up when VSYNC transitions high...we just want to return
		reti
		
;***********************************************************
;	Function Name: <interrupt handler for External Interrupt1> 
;	Function Description: This function is responsible
;	for handling a falling edge on the Ext Interrupt 1.  This
;	routine simply returns, since we just want to wake up
;	whenever the HREF transitions (meaning the pixels 
;	are starting after VSYNC transitioned, and we need to
; 	start acquiring the pixel blocks
;	Inputs:  none
;	Outputs: none
;***********************************************************	
SIG_INTERRUPT1:
; This will wake us up when HREF transitions high...we just want to return
		reti
		
;***********************************************************
;	Function Name: <interrupt handler for Timer0 overflow>
;	Function Description: This function is responsible
;	for handling the Timer0 overflow (hooked up to indicate
;	when we have reached the number of HREFs required in a
;	single frame).  We set the T flag in the SREG to
;	indicate to the _acquirePixelBlock routine that it needs
;	to exit, and then set the appropriate action to take in
;	the eventList of the Executive module.
;	Inputs:  none
;	Outputs: none
;   Note: Originally, the HREF pulses were also going to
;   be counted by a hardware counter, but it didn't end up
;   being necessary
;***********************************************************
;SIG_OVERFLOW0:
;		set				; set the T bit in SREG
;		lds		tmp1,eventBitmask
;		ori		tmp1,EV_ACQUIRE_FRAME_COMPLETE
;		sts		eventBitmask,tmp1
;		reti
		
;***********************************************************
;	Function Name: <interrupt handler for Timer1 overflow>
;	Function Description: This function is responsible
;	for handling the Timer1 overflow (hooked up to indicate
;	when we have reached the end of a line of pixel data,
;	since PCLK is hooked up to overflow TCNT1 after 176 
;	pixels).  This routine generates an acquire line complete
;	event in the fastEventBitmask, which is streamlined for
;	efficiency reasons.
;***********************************************************
SIG_OVERFLOW1:				
		lds		tmp1,fastEventBitmask   		; set a flag indicating
		ori		tmp1,FEV_ACQUIRE_LINE_COMPLETE	; a line is complete
		sts		fastEventBitmask,tmp1
		set		; set the T bit in SREG 
		;sbi		_SFR_IO_ADDR(PORTD),PD6 ; For testing...
		;cbi		_SFR_IO_ADDR(PORTD),PD6 ; For testing...

		reti

; This is the default handler for all interrupts that don't
; have handler routines specified for them.
        .global __vector_default              
__vector_default:
        reti

        .end
