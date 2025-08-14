#define F_CPU 11059200UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "timer1PWM.h"

int main () {
  DDRB=0xFF;
  DDRD=0x03;
  PORTB+=1;
  initPWM();
  int x;
  while (1) {
    if((PIND >> 3) & 1) {
      setCHA(0);
      setCHB(32);
    }
    else {
      if((PIND >> 2) & 1) {
        setCHA(0);
        setCHB(64);
      }
      else {
        if((PIND >> 4) & 1) {
          setCHA(64);
          setCHB(0);
        }
		else {
		  setCHA(255);
		  setCHB(255);
		}
	  }
	}
	if((PORTB & 1)==1){PORTB-=1;}
	else{PORTB+=1;}
	for(x=0;x<10;x++){_delay_loop_2(30000);}
  }
}
         