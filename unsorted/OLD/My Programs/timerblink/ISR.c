#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include "ISR.h"

enum {ON , OFF};
uint8_t onoff;
  
SIGNAL (SIG_OVERFLOW0)           //unquote if using timer 0 interrupts
{
  switch (onoff)
  {
    case ON:
	  if(counter == counttoON)
	  {
	    onoff = OFF;
		PORTB = 0;
		counter = 0;
	  }
	  break;
	
	case OFF:
	  if(counter == counttoOFF)
	  {
	    onoff = ON;
		PORTB = 2;
		counter = 0;
	  }
	  break;
  }
  counter++;
}

void start( void )
{
  DDRB = 255;
  counter = 0;
  onoff = OFF;
  //MCUCR = 15;   //unquote if using external interrupts
  //GICR = 192;   //unquote if using external interrupts
  TCCR0 = 1;    //unquote if using timer interrupts
  TIMSK = 1;  //unquote if using timer interrupts
  sei();
}

void setONcycles( uint16_t cyclesON )
{
  counter = 0;
  counttoON = cyclesON;
}

void setOFFcycles( uint16_t cyclesOFF )
{
  counter = 0;
  counttoOFF = cyclesOFF;
}
  
	