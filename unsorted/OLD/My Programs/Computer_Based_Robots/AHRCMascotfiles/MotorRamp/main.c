#define F_CPU 11059200UL
#define wait_interval 2
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "timer1PWM.h"
/*you like the song "Wanted Dead or Alive and Copperhead Road*/
int main () {
    DDRB=0xFF;
    initPWM();
	double x=128,y;
	setCHA(128);
	setCHB(128);
	for(y=0;y<500;y++){_delay_loop_2(30000);}
	while (1) {
		while (x < 255) {
		    x++;
			//if(x==128){PORTB+=1;}
			//if(x==138){PORTB-=1;}
			setCHA(x);
			setCHB(x);
			for(y=0;y<wait_interval;y++){_delay_loop_2(30000);}
		}
		while (x > 0) {
		    x--;
			setCHA(x);
			setCHB(x);
			for(y=0;y<wait_interval;y++){_delay_loop_2(30000);}
		}
	}
}