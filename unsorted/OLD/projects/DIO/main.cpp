#define F_CPU 11059200UL
#include <stdint.h>
#include <util/delay.h>
//#include "oDio1.h"
//#include "oDio4.h"
//#include "oDio8.h"
#include "oDio16.h"

//this is targeted towards 40 pin avrs

int main () {
	//oDio1 led(8,1), ground(9,1);
	//oDio4 ledgrp(2,1);
	//oDio8 ledgrp(1,1);
	oDio16 prtAB(0,1);
	//ground.set(0);
    while (1) {
    	//led.set(1);
    	//ledgrp.set(1);
    	prtAB.set(256);
    	_delay_ms(1000);
    	//led.set(0);
    	//ledgrp.set(0);
    	prtAB.set(0);
    	_delay_ms(1000);
    }
    return 0;
}
