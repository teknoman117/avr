#include <avr/delay.h>
#include <avr/io.h>

#define time 30000
       
main (void)
{
  unsigned char counter;
  DDRB = 255;
  while (1)
  {
    PORTB = 129;
	counter = 0;
    while (counter != 10)
	{
      _delay_loop_2(time);
 	  counter++;
	}
    PORTB = 66;
	counter = 0;
    while (counter != 10)
	{
      _delay_loop_2(time);
 	  counter++;
	}
    PORTB = 36;
	counter = 0;
    while (counter != 10)
	{
      _delay_loop_2(time);
 	  counter++;
	}
    PORTB = 24;
	counter = 0;
    while (counter != 10)
	{
      _delay_loop_2(time);
 	  counter++;
	}
    PORTB = 24;
	counter = 0;
    while (counter != 10)
	{
      _delay_loop_2(time);
 	  counter++;
	}
    PORTB = 36;
	counter = 0;
    while (counter != 10)
	{
      _delay_loop_2(time);
 	  counter++;
	}
    PORTB = 66;
	counter = 0;
    while (counter != 10)
	{
      _delay_loop_2(time);
 	  counter++;
	}
    PORTB = 129;
	counter = 0;
    while (counter != 10)
	{
      _delay_loop_2(time);
 	  counter++;
	}
  }	   
}