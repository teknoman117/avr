/*
 *      mmc.h
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

#ifndef _MMC_H
#define _MMC_H 1

#include <stdint.h>

char init_mmc( void );

uint8_t mmc_command( uint8_t befF, uint8_t befH, uint16_t AdrH, uint16_t AdrL );

char mmc_readblock( uint32_t address, uint8_t *sector );

char mmc_writeblock( uint32_t address, uint8_t *sector );

#endif 
