/*
 *      fakeuart.c
 *      
 *      Copyright 2011 Nathaniel Robert Lewis <linux.robotdude@gmail.com>
 *      
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 *
 *      This program creates a fake (software) uart with an interrupt pin (INT2) for RX,
 *      a standard IO pin for TX, and Timer/Counter 2 for a time source.  It also
 *      creates a 128 byte TX buffer and a 128 byte RX buffer to store data
 *
 *      Example:
 *          initFakeUart()     // initalizes the fake uart
 *          txbs('F')               // writes fake to the tx buffer
 */
 
#include "fakeuart.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <timercommon.h>
#include <extint.h>
 
#define SOFT_BUFF_SIZE 512
#define SOFT_BUFF_MASK 511

#define RX_WAIT_FLAG 0x01
#define RX_ACTIVE_FLAG 0x02
#define TX_ACTIVE_FLAG 0x04

#define BAUD_9600 33
#define BAUD_19200 16

// Software UART variables
volatile unsigned char flags = 0;
volatile unsigned char bitcounter = 0;

// Receiving variables
volatile unsigned char rx_char = 0;
volatile unsigned short rxrp = 0;
volatile unsigned short rxwp = 0;
volatile unsigned short rxbc = 0;
volatile unsigned char rxbuffers[SOFT_BUFF_SIZE];

// Transmit variables
volatile unsigned char tx_char = 0;
volatile unsigned short txrp = 0;
volatile unsigned short txwp = 0;
volatile unsigned short txbc = 0;
volatile unsigned char txbuffers[SOFT_BUFF_SIZE];
 
 // Timer counter, to fetch/produce those bits!
ISR(TIMER2_COMPA_vect) {
    // Check the receive flags
    if(flags & RX_WAIT_FLAG) {
        // The synchronize wait is over, clear the wait flag, set the active flag
        flags &= ~RX_WAIT_FLAG;
        flags |= RX_ACTIVE_FLAG;
    } else if(flags & RX_ACTIVE_FLAG) {
        // The rx system is active, so we need to do bit counting and stuff
        if(bitcounter < 8) {
            // The store is not yet done
            if(PINB & 0x04) rx_char |= (1 << bitcounter);
        } else {
            // Enable interrupts again, clear active flag
            EIMSK |= (1 << INT2);
            EIFR |= (1 << INTF2);
            flags &= ~RX_ACTIVE_FLAG;
            
            // Store the received byte in the buffer
            rxwp = (rxwp + 1) & SOFT_BUFF_MASK;
            rxbuffers[rxwp] = rx_char;
            rxbc++;
        }
        bitcounter++;
    } else if(flags & TX_ACTIVE_FLAG) {
        // Check if bitcounter is still valid
        if(bitcounter < 8) {
            // Transmit a bit
            if(tx_char & (1 << bitcounter)) PORTB |= 0x08;
            else PORTB &= 0xF7;        
        } else if(bitcounter == 8) {
            // Transmit a stop bit
            PORTB |= 0x08;
        } else if(bitcounter > 15) {
            // Clear the TX Flag
            flags &= ~TX_ACTIVE_FLAG;
        }
        bitcounter++;
    } else if(txbc > 0) {
        // If we are not transmitting or receiving, queue a transmission if we have bytes in the tx buffer
        TCNT2 = 0;
        txrp = (txrp + 1) & SOFT_BUFF_MASK;
        txbc --;
        tx_char = txbuffers[txrp];
        bitcounter = 0;
        
        // Disable the receive interrupt, set start bit, and set tx flag
        EIMSK &= ~(1 << INT2);
        flags |= TX_ACTIVE_FLAG;
        PORTB &= 0xF7;
        TCNT2 = 0;
    } else {
        // If we have no tx data, rx data, or doing either, re-enable receive interrupt
        EIMSK |= (1 << INT2);
        EIFR |= (1 << INTF2);
    }
}

// Start bit detector on INT2
ISR(INT2_vect) {
    // Clear INT2 enable bit
    TCNT2 = 20;
    EIMSK &= ~(1 << INT2);
    
    // Set the RX_WAIT flag (and so begins the process)
    flags |= RX_WAIT_FLAG;
    
    // Reset the rx variables
    rx_char = 0;
    bitcounter = 0;
}

void initFakeUart(void) {
    // Initalize IO directions
    DDRB =     _BV(PORTB3);
    DDRB &= ~_BV(PORTB2);
    PORTB |=  _BV(PORTB3);
    
    // Initalize Timer2 on CTC mode (don't turn it on)
    TCCR2A = (1 << WGM21);
    TCCR2B = (T2_PRESCALER_64) & 0x07;
    OCR2A = BAUD_19200;
    
    // Interrupt enables
    set_mode_int2(INT_TRIGGER_FALLING);
	EIMSK |= (1 << INT2);
    TIMSK2 = (1 << OCIE2A);
    sei();
}

// Returns the current byte count in the receive buffer
unsigned char bytesInBufferS (void) {
    return rxbc;
}

unsigned char bytesInTXBufferS (void) {
    return txbc;
}

// Queue a byte in the TX buffer
void txbs(unsigned char b) {
    txwp = (txwp + 1) & SOFT_BUFF_MASK;
    txbuffers[txwp] = b;
    txbc++;
}

// Fetch a byte from the buffer
unsigned char rxbs() {
    // Block while waiting for a character
    while(!rxbc) {}
    
    // Fetch the byte
    rxrp = (rxrp + 1) & SOFT_BUFF_MASK;
    rxbc--;
    return rxbuffers[rxrp];
}

short peeks(unsigned short depth) {
    // Return -1 if we don't have bytes at that depth
    if(rxbc < (depth + 1)) return -1;
    
    // Return the peeks byte
    return rxbuffers[(rxrp + depth + 1) & SOFT_BUFF_MASK];
}
