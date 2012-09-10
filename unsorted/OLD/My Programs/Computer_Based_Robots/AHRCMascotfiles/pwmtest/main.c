#define F_CPU 11059200UL
#define wait_interval 5
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "timer1PWM.h"
/*you like the song "Wanted Dead or Alive and Copperhead Road*/
int main () {
    DDRB=0xFF;
    initPWM();
	setCHA(255);
	setCHB(255);
	double x;
	for(x=0;x<100;x++){_delay_loop_2(30000);}
	setCHA(128);
	setCHB(128);
}