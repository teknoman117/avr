#define F_CPU 11059200UL
#include <stdint.h>
#include <util/delay.h>
#include <pthread.h>
#include "oDio1.h"

int main () {
	pthread_t thread1, thread2;
	oDio1 led(8,1), ground(9,1);
	ground.set(0);
	while (1) {
		led.set(1);
		_delay_ms(1000);
		led.set(0);
		_delay_ms(1000);
	}
	return 0;
}
	
