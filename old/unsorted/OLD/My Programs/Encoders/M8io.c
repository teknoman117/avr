#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "M8io.h"

void  initIO ( void )
{
  prtb = 0;
  prtc = 0;
  prtd = 0;
  dirb = 0;
  dirc = 0;
  dird = 0;
}

void  setIOB(uint8_t pin , uint8_t direction)
{
  switch (direction)
  {
    case O:
	if(((dirb >> pin) & 0x01) == 0){DDRB = DDRB + (1 << pin);dirb = dirb + (1 << pin);}
	break;
	
	case I:
	if(((dirb >> pin) & 0x01) == 1){DDRB = DDRB - (1 << pin);dirb = dirb - (1 << pin);}
	break;
  }
}

void  setIOC(uint8_t pin , uint8_t direction)
{
  switch (direction)
  {
    case O:
	if(((dirc >> pin) & 0x01) == 0){DDRC = DDRC + (1 << pin);dirc = dirc + (1 << pin);}
	break;
	
	case I:
	if(((dirc >> pin) & 0x01) == 1){DDRC = DDRC - (1 << pin);dirc = dirc - (1 << pin);}
	break;
  }
}

void  setIOD(uint8_t pin , uint8_t direction)
{
  switch (direction)
  {
    case O:
	if(((dird >> pin) & 0x01) == 0){DDRD = DDRD + (1 << pin);dird = dird + (1 << pin);}
	break;
	
	case I:
	if(((dird >> pin) & 0x01) == 1){DDRD = DDRD - (1 << pin);dird = dird - (1 << pin);}
	break;
  }
}

void  setStateB(uint8_t pin , uint8_t state)
{
  switch (state)
  {
    case 1:
	if(((prtb >> pin) & 0x01) == 0){PORTB = PORTB + (1 << pin);prtb = prtb + (1 << pin);}
	break;
	
	case 0:
	if(((prtb >> pin) & 0x01) == 1){PORTB = PORTB - (1 << pin);prtb = prtb - (1 << pin);}
	break;
  }
}

void  setStateC(uint8_t pin , uint8_t state)
{
  switch (state)
  {
    case 1:
	if(((prtc >> pin) & 0x01) == 0){PORTC = PORTC + (1 << pin);prtc = prtc + (1 << pin);}
	break;
	
	case 0:
	if(((prtc >> pin) & 0x01) == 1){PORTC = PORTC - (1 << pin);prtc = prtc - (1 << pin);}
	break;
  }
}

void  setStateD(uint8_t pin , uint8_t state)
{
  switch (state)
  {
    case 1:
	if(((prtd >> pin) & 0x01) == 0){PORTD = PORTD + (1 << pin);prtd = prtd + (1 << pin);}
	break;
	
	case 0:
	if(((prtd >> pin) & 0x01) == 1){PORTD = PORTD - (1 << pin);prtd = prtd - (1 << pin);}
	break;
  }
}

uint8_t  getStateB(uint8_t pin)
{
  return ((PINB >> pin) & 0x01);
}

uint8_t  getStateC(uint8_t pin)
{
  return ((PINC >> pin) & 0x01);
}

uint8_t  getStateD(uint8_t pin)
{
  return ((PIND >> pin) & 0x01);
}
	