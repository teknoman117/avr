#define F_CPU 11059200UL
#include <util/delay.h>
#include "oLED2.h"

int main () {
	oLED2 led(8, 9);
	while (1) {
		led.on1();
		_delay_ms(1000);
		led.off();
		_delay_ms(1000);
	}
    return 0;
}
	
	
