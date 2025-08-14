#include "avr40pio.h"
#include "timer1pwm.h"
#include "motors.h"
#include <stdint.h>
/*
pb0 = m1 dir1 (forward)
pb1 = m1 dir2 (backward)
pb2 = m2 dir1 (forward)
pb3 = m2 dir2 (backward)
*/
int mv = 0;

void initMotors (void) {
    initPWM();
	initIO();
	setDIRB(0, 1);
	setDIRB(1, 1);
	setDIRB(2, 1);
	setDIRB(3, 1);
}

void fwd (uint8_t pwr) {
    setIOB(0, 1);
	setIOB(1, 0);
	setIOB(2, 1);
	setIOB(3, 0);
	setCHA(pwr);
	setCHB(pwr);
	mv = 1;
}

void bwd (uint8_t pwr) {
    setIOB(0, 0);
	setIOB(1, 1);
	setIOB(2, 0);
	setIOB(3, 1);
	setCHA(pwr);
	setCHB(pwr);
	mv = 1;
}

void lft (uint8_t pwr) {
    setIOB(0, 0);
	setIOB(1, 0);
	setIOB(2, 1);
	setIOB(3, 0);
	setCHA(pwr);
	setCHB(pwr);
	mv = 1;
}

void rgh (uint8_t pwr) {
    setIOB(0, 1);
	setIOB(1, 0);
	setIOB(2, 0);
	setIOB(3, 0);
	setCHA(pwr);
	setCHB(pwr);
	mv = 1;
}

void slft (uint8_t pwr) {
    setIOB(0, 0);
	setIOB(1, 1);
	setIOB(2, 1);
	setIOB(3, 0);
	setCHA(pwr);
	setCHB(pwr);
	mv = 1;
}

void srgh (uint8_t pwr) {
    setIOB(0, 1);
	setIOB(1, 0);
	setIOB(2, 0);
	setIOB(3, 1);
	setCHA(pwr);
	setCHB(pwr);
	mv = 1;
}

void blft (uint8_t pwr) {
    setIOB(0, 0);
	setIOB(1, 0);
	setIOB(2, 0);
	setIOB(3, 1);
	setCHA(pwr);
	setCHB(pwr);
	mv = 1;
}

void brgh (uint8_t pwr) {
    setIOB(0, 0);
	setIOB(1, 1);
	setIOB(2, 0);
	setIOB(3, 0);
	setCHA(pwr);
	setCHB(pwr);
	mv = 1;
}

void stop (void) {
    setIOB(0, 0);
	setIOB(1, 0);
	setIOB(2, 0);
	setIOB(3, 0);
	setCHA(0);
	setCHB(0);
	mv = 0;
}

int isMoving (void) {
    return mv;
}

    
	