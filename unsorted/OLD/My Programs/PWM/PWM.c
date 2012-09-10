#include <avr/io.h>
#include <util/delay.h>
#include "timer1PWM.h"

int main () {
  initPWM();
  double x;
  DDRB = DDRD = 255;
  while (1) {
    setCHA(255);
	setCHB(255);
	for(x=0;x<200;x++){_delay_loop_2(30000);}
	setCHB(128);
	for(x=0;x<200;x++){_delay_loop_2(30000);}
    setCHA(0);
    setCHB(0);
    for(x=0;x<200;x++){_delay_loop_2(30000);}	
	setCHA(255);
	for(x=0;x<200;x++){_delay_loop_2(30000);}
  }
  return 0;
}