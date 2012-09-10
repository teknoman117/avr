#include <avr/io.h>
#include <util/delay.h>

int main () {
  int x;
  x = 0;
  DDRD = DDRC = 0xFF;
  PORTC = 0;
  while (1) {
    PORTD = PORTD + 128; 
	for(x=0;x<20;x++) {
	  _delay_loop_2(30000);
	}
	PORTD = PORTD - 128;
	for(x=0;x<20;x++) {
	  _delay_loop_2(30000);
	}
  }
}

