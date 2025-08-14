#include <avr/io.h>
#include <avr/delay.h>
 
#define time 30000000

void main (void)
  {
  unsigned char counter;
  DDRB = 0xFF;
  while (1)
  {
  PORTB = 4;
  counter = 0;
  while (counter != 20)
    {
	_delay_loop_2(time);
	counter++;
	}
  PORTB = 2;
  counter = 0;
  while (counter != 20)
    {
	_delay_loop_2(time);
	counter++;
	}
  }
  
  return 1;
  }
  
    