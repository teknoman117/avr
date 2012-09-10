#define F_CPU 11059200UL
#define L 4
#define CL 3
#define C 2
#define CR 1
#define R 0
#include <avr/io.h>
#include <util/delay.h>
#include "motors.h"

int main () {
    unsigned int counter;
	unsigned char moved=0;
	initMotors();
	DDRD = (1 << 1) | (1 << 7);
	while (1) {
	    unsigned char h = (PIND >> 2) & 0x1F;
		if((h >> C) & 0x01) {
		     moved = 1;
             bwd(64);
             for(counter=0;counter<10;counter++){_delay_loop_2(60000);}
             slft(32);
             for(counter=0;counter<4;counter++){_delay_loop_2(60000);}
             stop();
        }
	    if((((h >> CR)&1) || ((h >> R)&1) ) && !(moved)) {
		     moved = 1;
		     brgh(32);
			 for(counter=0;counter<4;counter++){_delay_loop_2(60000);}
			 stop();
		}
	    if((((h >> CL)&1) || ((h >> L)&1)) && !(moved)) {
		     moved = 1;
		     blft(32);
			 for(counter=0;counter<4;counter++){_delay_loop_2(60000);}
			 stop();
		}
        if(!(moved)) {
             fwd(64);
        }
        moved = 0;
    }
	return 0;
}	
		
         