#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/delay.h>

void main (void)
{
  unsigned int counter;
  DDRD = 255;
  PORTD += 32;
  while (1)
  {
	  counter = 0;
	  while (counter != 30)
	  {
      _delay_loop_2(30000);
	  counter++;
	  }
      PORTD = PORTD + 64;
	  counter = 0;
	  while (counter != 30)
	  {
      _delay_loop_2(30000);
	  counter++;
	  }
	  PORTD = PORTD - 64;
	  PORTD = PORTD + 8;
	  counter = 0;
	  while (counter != 30)
	  {
      _delay_loop_2(30000);
	  counter++;
	  }
      PORTD = PORTD + 64;
	  counter = 0;
	  while (counter != 30)
	  {
      _delay_loop_2(30000);
	  counter++;
	  }
      PORTD = PORTD - 72;  	
    }
}