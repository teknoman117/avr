#include <avr/led_blink.h>
#include <avr/delay.h>

void main(void)
{
  int x;
  int counter;
  x = 1;
  while (1)
  {
    set_port_c(x);
	x << 1;
	counter = 0;
	while (counter != 5)
	{
	  _delay_loop_2(30000);
	  counter ++;
	}
	if(x = 16)
	{
	  x = 1;
	}
  }
}