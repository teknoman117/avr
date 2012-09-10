#include <avr/io.h>
#include <util/delay.h>

void main () {
  DDRB = 1;
  double x;
  while (1) {
    PORTB = 1;
    for(x=0;x<10;x++){_delay_loop_2(30000);} 
	PORTB = 0;
	for(x=0;x<10;x++){_delay_loop_2(30000);} 
  }
}