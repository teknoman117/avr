#define F_CPU 11059200UL
#include <util/delay.h>
#include "uart.h"

int main () {
	init_uart(5,8,'n',1);
	while (1) {
	    u_transmit((char*) "Hello World!");
	    _delay_ms(1500);
	}
	return 0;
}
		
