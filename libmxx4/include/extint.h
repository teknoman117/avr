/*
 *      extint.h
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
 */
 
 #ifndef _AVR_EXTINT_H_
 #define _AVR_EXTINT_H_ 1
 
 #include <stdint.h>
 
 #define INT_TRIGGER_LOW     0 
 #define INT_TRIGGER_ANY     1
 #define INT_TRIGGER_FALLING 2
 #define INT_TRIGGER_RISING  3
 
 void set_mode_int0( uint8_t mode );
 void set_mode_int1( uint8_t mode );
 void set_mode_int2( uint8_t mode );
 
 void set_mask_pcint0( uint8_t mask );
 void set_mask_pcint1( uint8_t mask );
 void set_mask_pcint2( uint8_t mask );
 void set_mask_pcint3( uint8_t mask );
 
 void enable_int0( void );
 void enable_int1( void );
 void enable_int2( void );
 
 void enable_pcint0( void );
 void enable_pcint1( void );
 void enable_pcint2( void );
 void enable_pcint3( void );
 
 void disable_int0( void );
 void disable_int1( void );
 void disable_int2( void );
 
 void disable_pcint0( void );
 void disable_pcint1( void );
 void disable_pcint2( void );
 void disable_pcint3( void );
 
 #endif
