#include "timer1pwm.h"
#include "motors.h"
#include <stdint.h>
#include <math.h>
/*
pb0 = m1 dir1 (forward)
pb1 = m1 dir2 (backward)
pb2 = m2 dir1 (forward)
pb3 = m2 dir2 (backward)
*/
int mv = 0;
int dir = 0;

void initMotors (void) {
    initPWM();
}

void fwd (uint8_t pwr) {
    dir = 1;
	setCHA(pwr + 128);
	setCHB(pwr + 128);
	mv = 1;
}

void bwd (uint8_t pwr) {
	setCHA(fabs(pwr - 255));
	setCHB(fabs(pwr - 255));
	dir = 2;
	mv = 1;
}

void lft (uint8_t pwr) {
	setCHB(pwr + 128);
	setCHA(128);
	dir = 5;
	mv = 1;
}

void rgh (uint8_t pwr) {
	setCHA(pwr + 128);
	setCHB(128);
	dir = 6;
	mv = 1;
}

void slft (uint8_t pwr) {
	setCHA(fabs(pwr - 255));
	setCHB(pwr + 128);
	dir = 3;
	mv = 1;
}

void srgh (uint8_t pwr) {
	setCHA(pwr + 128);
	setCHB(fabs(pwr - 255));
	dir = 4;
	mv = 1;
}

void blft (uint8_t pwr) {
	setCHA(128);
	setCHB(fabs(pwr - 255));
	dir = 7;
	mv = 1;
}

void brgh (uint8_t pwr) {
	setCHA(fabs(pwr - 255));
	setCHB(128);
	dir= 8;
	mv = 1;
}

void stop (void) {
	setCHA(128);
	setCHB(128);
	mv = 0;
	dir = 0;
}

int isMoving (void) {
    return mv;
}

int whatDir (void) {
    return dir;
}

    
	