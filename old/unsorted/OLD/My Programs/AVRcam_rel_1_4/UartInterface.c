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
	Module Name: UartInterface.c
	Module Date: 04/10/2004
	Module Auth: John Orlando
	
	Description: This module is responsible for providing an
	interface to the UART hardware available on the mega8.
	This interface is an interrupt-driven interface.
    
    Revision History:
    Date        Rel Ver.    Notes
    4/10/2004      0.1     Module created
    6/30/2004      1.0     Initial release for Circuit Cellar
                           contest.
    11/15/2004     1.2     Updated UART baud rate regs so that
                           it runs at 115.2 kbps when the input
                           crystal is at 17.7 MHz (which is the
                           speed of the OV6620's crystal).
    1/16/2005      1.4     Moved the serial received ISR to
                           this file, instead of having it
                           in its own UartInterfaceAsm.S file
                           written in assembly.
***********************************************************/

/*	Includes */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include "CommonDefs.h"
#include "UartInterface.h" 
#include "UIMgr.h"
#include "Executive.h"

/*  Local Variables */

/* 	Local Structures and Typedefs */

/*  Extern Variables */

/*  Definitions */

/***********************************************************
	Function Name: UartInt_init
	Function Description: This function is responsible for
	initializing the UART interface on the mega8.  This 
	interface is set to communicate at 115.2 Kbps, with an
	8N1 protocol.
	Inputs:  none
	Outputs: none
***********************************************************/	
void UartInt_init(void)
{	
	/* set up the baud rate registers so the UART will operate
	at 115.2 Kbps */
	UBRRH = 0x00;

#ifdef NO_CRYSTAL    
    UBRRL = 18;  /* 18 for double clocking at 115.2 kbps */
#else    
	UBRRL = 0x08;  /* for 16 MHz crystal at 115.2 kbps */
#endif    
	
	/* enable the tx and rx capabilities of the UART...as well 
		as the receive complete interrupt */
	UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);	
	
	/* set up the control registers so the UART works at 8N1 */
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
    
#ifdef NO_CRYSTAL     
    /* set the baud rate to use the double-speed */
    UCSRA = (1<<U2X);
#endif    
	
}

/***********************************************************
	Function Name: UartInt_txByte
	Function Description: This function is responsible for
	transmitting a single byte on the uart.  
	Inputs:  txByte - the byte to send
	Outputs: none
	NOTES: When the TX UDRE (data register empty) is set, there
	is puposefully no interrupt...thus, to send a string of
	data out, the calling routine needs to hold up the entire
	application while this takes place (or just send one
	byte at a time at strtegically timed intervals, like
	the stats data is sent out :-)
***********************************************************/
void UartInt_txByte(unsigned char txByte)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = txByte;
}

/***********************************************************
	Function Name: SIG_UART_RECV ISR
	Function Description: This function is responsible for
	handling the interrupt caused when a data byte is 
    received by the UART.
	Inputs:  none
	Outputs: none
	NOTES: This function was originally written in assembly,
    but moved over to C when the setting of the "T" bit at
    the end of the routine was no longer necessary (this
    theoretically allowed the AVRcam to respond to serial
    bytes in the middle of tracking or dumping a frame.
    But it wasn't really needed, and understanding the C
    is easier  :-)
***********************************************************/
SIGNAL(SIG_UART_RECV)
{
    unsigned char tmpHead;
    /* read the data byte, put it in the serial queue, and
    post the event */
 
    UIMgr_rxFifo[UIMgr_rxFifoHead] = UDR;

    /* now move the head up */
    tmpHead = (UIMgr_rxFifoHead + 1) & (UI_MGR_RX_FIFO_MASK);
    UIMgr_rxFifoHead = tmpHead;
    
    /* write the serial received event to the event fifo */
    Exec_eventFifo[Exec_eventFifoHead] = EV_SERIAL_DATA_RECEIVED;

    /* now move the head up */
    tmpHead = (Exec_eventFifoHead + 1) & (EXEC_EVENT_FIFO_MASK);
    Exec_eventFifoHead = tmpHead;
}

