#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <avr/delay.h>

int main() {
  uint16_t cnt;
  cnt = 0;
  DDRC = 0xFF;
  while (1) {
    PORTC = (1 << PC0);
	for(cnt=0;cnt<200;cnt++){_delay_loop_2(30000);}
	PORTC = (1 << PC1);
	for(cnt=0;cnt<200;cnt++){_delay_loop_2(30000);}
  }
}
	