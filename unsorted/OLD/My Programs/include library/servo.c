#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include "servo.h"

void initservos( uint16_t reloadtime, uint8_t servo1en, uint8_t servo2en )
{
  reload = reloadtime;
  if(servo1en == ON){DDRD = DDRD + 16;plsref[0] = ON;}
  else{plsref[0] = NULL;}
  if(servo2en == ON){DDRD = DDRD + 32;plsref[1] = ON;}
  else{plsref[1] = NULL;}
  TCCR0 = 1;
  TIMSK = TIMSK + 1;
  sei();
}

void setservo1( uint16_t cycles1 )
{
  counttopulse[0] = cycles1;
  counter[0] = 0;
}

void setservo2( uint16_t cycles2 )
{
  counttopulse[1] = cycles2;
  counter[1] = 0;
}

SIGNAL (SIG_OVERFLOW0)
{
  switch (plsref[0])
  {
    case ON:
	  if(counter[0] == reload)
	  {
	    plsref[0] = OFF;
		PORTD = PORTD + 16;
		counter[0] = 0;
	  }
	  break;
	
	case OFF:
	  if(counter[0] == counttopulse[0])
	  {
	    plsref[0] = ON;
		PORTD = PORTD - 16;
		counter[0] = 0;
	  }
	  break;
	  
	case NULL:
	  break;
  }
  switch (plsref[1])
  {
    case ON:
	  if(counter[1] == reload)
	  {
	    plsref[1] = OFF;
		PORTD = PORTD + 32;
		counter[1] = 0;
	  }
	  break;
	
	case OFF:
	  if(counter[1] == counttopulse[1])
	  {
	    plsref[1] = ON;
		PORTD = PORTD - 32;
		counter[1] = 0;
	  }
	  break;
	  
	case NULL:
	  break;
  }
  counter[0]++;
  counter[1]++;
}