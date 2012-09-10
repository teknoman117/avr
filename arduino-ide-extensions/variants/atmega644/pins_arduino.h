/*
  pins_arduino.h - Pin definition functions for Arduino built from a ATmega644

  Copyright (c) 2012 Nathaniel R. Lewis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
 
   Adaptation of variants/standard/pins_arduino.h to fit the atmega644.  
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            32
#define NUM_ANALOG_INPUTS           8

#define analogInputToDigitalPin(p)  ((p < 8) ? 31 - (p) : -1)
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 4 || (p) == 12 || (p) == 13 || (p) == 14 || (p) == 15)

static const uint8_t SS   = 4;
static const uint8_t MOSI = 5;
static const uint8_t MISO = 6;
static const uint8_t SCK  = 7;

static const uint8_t SDA = 16;
static const uint8_t SCL = 17;

static const uint8_t A0 = 31;
static const uint8_t A1 = 30;
static const uint8_t A2 = 29;
static const uint8_t A3 = 28;
static const uint8_t A4 = 27;
static const uint8_t A5 = 26;
static const uint8_t A6 = 25;
static const uint8_t A7 = 24;

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 31) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? (1) : (((p) <= 15) ? (3) : (((p) <= 23) ? (2) : (0)))) 

#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK1) : (((p) <= 15) ? (&PCMSK3) : (((p) <= 23) ? (&PCMSK2) : (&PCMSK3))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 15) ? ((p) - 8) : (((p) <= 23) ? ((p) - 16) : (7 - ((p) - 24)))))

#ifdef ARDUINO_MAIN

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA 644 / ARDUINO
//
//                   +--\/--+
//      (D 0) PB0   1|      |40  PA0 (AI 0) (D 31)
//      (D 1) PB1   2|      |39  PA1 (AI 1) (D 30)
//      (D 2) PB2   3|      |38  PA2 (AI 2) (D 29)
//  PWM (D 3) PB3   4|      |37  PA3 (AI 3) (D 28)
//  PWM (D 4) PB4   5|      |36  PA4 (AI 4) (D 27)
// mosi (D 5) PB5   6|      |35  PA5 (AI 5) (D 26)
// miso (D 6) PB6   7|      |34  PA6 (AI 6) (D 25)
// sck  (D 7) PB7   8|      |33  PA7 (AI 7) (D 24)
//          RESET   9|      |32  AREF
//            VCC  10|      |31  GND
//            GND  11|      |30  AVCC
//          XTAL1  12|      |29  PC7 (D 23)
//          XTAL2  13|      |28  PC6 (D 22)
//      (D 8) PD0  14|      |27  PC5 (D 21)
//      (D 9) PD1  15|      |26  PC4 (D 20)
//     (D 10) PD2  16|      |25  PC3 (D 19)
//     (D 11) PD3  17|      |24  PC2 (D 18)
// PWM (D 12) PD4  18|      |23  PC1 (D 17)
// PWM (D 13) PD5  19|      |22  PC0 (D 16) 
// PWM (D 14) PD6  20|      |21  PD7 (D 15) PWM
//                   +------+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)

// ATMEL ATMEGA1280 / ARDUINO
//
// 0-7 PE0-PE7   works
// 8-13 PB0-PB5  works
// 14-21 PA0-PA7 works 
// 22-29 PH0-PH7 works
// 30-35 PG5-PG0 works
// 36-43 PC7-PC0 works
// 44-51 PJ7-PJ0 works
// 52-59 PL7-PL0 works
// 60-67 PD7-PD0 works
// A0-A7 PF0-PF7
// A8-A15 PK0-PK7


// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
    NOT_A_PORT,
    (uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
    NOT_A_PORT,
    (uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
    NOT_A_PORT,
    (uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PB, /* 0 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PD, /* 8 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
    PC, /* 16 */
    PC,
    PC,
    PC,
    PC,
    PC,
    PC,
    PC,
    PA, /* 24 */
    PA,
    PA,
    PA,
    PA,
    PA,
    PA,
    PA,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), /* 0, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 8, port D */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),	
	_BV(0), /* 16, port C */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 24, port A */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, /* 0 - port B */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER0A,
	TIMER0B,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,	
    NOT_ON_TIMER, /* 8 - port D */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER1B,
	TIMER1A,
	TIMER2B,
	TIMER2A,
	NOT_ON_TIMER, /* 16 - port C */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 24 - port A */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

#endif

#endif
