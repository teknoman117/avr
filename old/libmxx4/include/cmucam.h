/*
 *      cmucam.h
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
 *      This header controls the CMUcam version 1
 *    
 */

#ifndef _CMUCAM_H_
#define _CMUCAM_H_

#ifndef CMUCAM_UART
#warning CMUCAM_UART not defined defaulting to USART 0
#define CMUCAM_UART 0
#endif

#if CMUCAM_UART == 0 
// Use USART 0
#include "iusart0.h"
#define init_iusart(b, o) init_iusart0(b, o)
#define rxbi() rxbi0()
#elif CMUCAM_UART == 1
// Use USART 1
#include "iusart1.h"
#define init_iusart(b, o) init_iusart1(b, o)
#define rxbi() rxbi1()
#else
#error CMUCAM_UART is an invalid uart
#endif

#endif
