#define F_CPU 11059200UL
#define wait_interval 200
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "timer1PWM.h"

int main () {
  DDRB=0xFF;
  initPWM();
  int x;
  while (1) {
    setCHA(255);
	setCHB(255);
	if((PORTB & 1)==1){PORTB-=1;}
	else{PORTB+=1;}
	for(x=0;x<wait_interval;x++) {_delay_loop_2(30000);}
    setCHA(0);
	setCHB(255);
	if((PORTB & 1)==1){PORTB-=1;}
	else{PORTB+=1;}
	for(x=0;x<wait_interval;x++) {_delay_loop_2(30000);}
    setCHB(0);
	setCHA(255);
	if((PORTB & 1)==1){PORTB-=1;}
	else{PORTB+=1;}
	for(x=0;x<wait_interval;x++) {_delay_loop_2(30000);}
    setCHB(0);
	setCHA(0);
	if((PORTB & 1)==1){PORTB-=1;}
	else{PORTB+=1;}
	for(x=0;x<wait_interval;x++) {_delay_loop_2(30000);}
    setCHB(128);
	setCHA(128);
	if((PORTB & 1)==1){PORTB-=1;}
	else{PORTB+=1;}
	for(x=0;x<wait_interval;x++) {_delay_loop_2(30000);}
  }
  return 0;
}


	
	