/*
 *      usii2c.c
 *
 *      Copyright 2010 Nathaniel Robert Lewis <linux.robotdude@gmail.com>
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
 *      This header controls a USI I2C Master on attinyx5 series
 *
 */

#include "usii2c.h"
#include <util/delay.h>

void i2c_init( void ) {
    USIPORT |= (1<<USISDA);           // Enable pullup on SDA, to set high as released state.
    USIPORT |= (1<<USISCL);           // Enable pullup on SCL, to set high as released state.
    USIDDR  |= (1<<USISCL);           // Enable SCL as output.
    USIDDR  |= (1<<USISDA);           // Enable SDA as output.
    USIDR  =  0xFF;                       // Preload dataregister with "released level" data.
    USICR  =  (0<<USISIE)|(0<<USIOIE)|                            // Disable Interrupts.
              (1<<USIWM1)|(0<<USIWM0)|                            // Set USI in Two-wire mode.
              (1<<USICS1)|(0<<USICS0)|(1<<USICLK)|                // Software stobe as counter clock source
              (0<<USITC);
    USISR  =  (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Clear flags,
              (0x0<<USICNT0);                                     // and reset counter.
}

uint8_t transfer( uint8_t stats ) {
    USISR = stats;                                  // Set USISR according to temp.
    stats =(0<<USISIE)|(0<<USIOIE)|                 // Interrupts disabled
           (1<<USIWM1)|(0<<USIWM0)|                 // Set USI in Two-wire mode.
           (1<<USICS1)|(0<<USICS0)|(1<<USICLK)|     // Software clock strobe as source.
           (1<<USITC);                              // Toggle Clock Port.
    do {
        _delay_us(10);
        USICR = stats;                  // Generate positve SCL edge.
        while(!(USIPIN & (1<<USISCL))); // Wait for SCL to go high.
        _delay_us(10);
        USICR = stats;                  // Generate negative SCL edge.
    } while(!(USISR & (1<<USIOIF)));    // Check for transfer complete.
    _delay_us(10);
    stats  = USIDR;                     // Read out data.
    USIDR = 0xFF;                       // Release SDA.
    USIDDR |= (1<<USISDA);              // Enable SDA as output.
    return stats;
}

char i2c_start( uint8_t address ) {       // runs i2c start condition returns 0 for ack, 1 for nack
    USIPORT |= (1<<USISCL);               // Release SCL.
    while(!(USIPIN & (1<<USISCL)));      // Verify that SCL becomes high.
    _delay_us(10);
    USIPORT &= ~(1<<USISDA);              // Force SDA LOW.
    _delay_us(10);
    USIPORT &= ~(1<<USISCL);              // Pull SCL LOW.
    USIPORT |= (1<<USISDA);               // Release SDA.
    USIPORT &= ~(1<<USISCL);              // Pull SCL LOW.
    USIDR   = address;                    // Setup data.
    transfer( BYTETRANSFER );             // Send 8 bits on bus.
    USIDDR  &= ~(1<<USISDA);              // Enable SDA as input.
    return transfer( BITTRANSFER ) & 1;
}

void i2c_stop( void ) {
    USIPORT &= ~(1<<USISDA);           // Pull SDA low.
    USIPORT |= (1<<USISCL);            // Release SCL.
    while( !(USIPIN & (1<<USISCL)) );  // Wait for SCL to go high.
    _delay_us( 10 );
    USIPORT |= (1<<USISDA);            // Release SDA.
    _delay_us( 10 );
}

char i2c_writebyte( uint8_t byte ) {
    USIPORT &= ~(1<<USISCL);              // Pull SCL LOW.
    USIDR   = byte;                       // Setup data.
    transfer( BYTETRANSFER );             // Send 8 bits on bus.
    USIDDR  &= ~(1<<USISDA);              // Enable SDA as input.
    return transfer( BITTRANSFER ) & 1;
}

uint8_t i2c_readbyte( char ack ) {
    USIDDR &= ~(1<<USISDA);               // Enable SDA as input.
    uint8_t b = transfer( BYTETRANSFER );
    USIDR = 0xFF;
    if(ack) USIDR = 0x00;
    transfer( BITTRANSFER );
    return b;
}
