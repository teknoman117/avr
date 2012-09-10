#define F_CPU 16000000UL
#include "timer1PWM.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <stdint.h>

int main() {
  DDRC = 255;
  DDRD = 255;
  PORTC = 1;                          ;
  uint8_t x;
  uint16_t y;
  initPWM10();
  PORTD = ( 1 << PD6 );
  /*while(1) {
    while(PORTC < 128) {
      PORTC <<= 1;
      for(x=0;x<25;x++){_delay_loop_2(30000);}
    }
	while(PORTC != 1) {
	  PORTC >>= 1;
      for(x=0;x<25;x++){_delay_loop_2(30000);}	
    }
 
    PORTC = 128;
    for(x=0;x<50;x++){_delay_loop_2(30000);}
    PORTC = 0;
    for(x=0;x<50;x++){_delay_loop_2(30000);}   
  }*/ 
	for(y=0;y<1023;y++) {
	  setCHB(y);
      _delay_ms(10);
	}
	for(y=1023;y!=0;y--) {
	  setCHB(y);
      _delay_ms(10);
	} 
}
    
  