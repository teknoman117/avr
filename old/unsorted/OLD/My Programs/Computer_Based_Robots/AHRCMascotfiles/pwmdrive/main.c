#define F_CPU 11059200UL
#define wait_interval 2
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "timer1PWM.h"

void slowstop();
void slowstart();

int main () {
  double x;
  DDRB=0xFF;
  initPWM();
  while (1) {
    slowstart();
	for(x=0;x<12;x++){setCHA(200);setCHB(200);_delay_loop_2(30000);}
	slowstop();
	for(x=0;x<100;x++){setCHA(128);setCHB(128);_delay_loop_2(65000);}
	setCHA(150);
	setCHB(128);
	for(x=0;x<50;x++){_delay_loop_2(30000);}
	setCHA(128);
	setCHB(128);
  }
}

void slowstop () {
  double y=200,z;
  while (y != 128) {
    setCHA(y);
	setCHB(y);
	for(z=0;z<2;z++) {_delay_loop_2(30000);}
	y--;
  }
}

void slowstart () {
  double a=128,b;
  while (a != 200) {
    setCHA(a);
	setCHB(a);
	for(b=0;b<2;b++) {_delay_loop_2(30000);}
	a++;
  }
}
  