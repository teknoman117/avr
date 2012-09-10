#define F_CPU 11059200
#include <util/delay.h>
#include "A2D.h"

int main () {
	A2D ch1(0);
	while (1) {
		ch1.getValue8();
		_delay_ms(100);
		ch1.getValue10();
		_delay_ms(100);
	}
	return 0;
}
	
