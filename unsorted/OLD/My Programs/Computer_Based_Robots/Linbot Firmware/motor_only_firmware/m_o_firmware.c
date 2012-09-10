#define F_CPU 11059200UL
#include <util/delay.h>
#include <string.h>
#include "uart.h"
#include "motors.h"

int main () {
    initMotors();
	open(5);
	unsigned char rx_str[50];
	while (1) {
	    read(rx_str,10,ON);
		if(!(strncmp(rx_str,"fwd",3))) {fwd(rx_str[4]);}
		if(!(strncmp(rx_str,"bwd",3))) {bwd(rx_str[4]);}
		if(!(strncmp(rx_str,"lft",3))) {lft(rx_str[4]);}
		if(!(strncmp(rx_str,"rgh",3))) {rgh(rx_str[4]);}
		if(!(strncmp(rx_str,"slft",4))) {slft(rx_str[4]);}
		if(!(strncmp(rx_str,"srgh",4))) {srgh(rx_str[4]);}
		if(!(strncmp(rx_str,"blft",4))) {blft(rx_str[4]);}
		if(!(strncmp(rx_str,"brgh",4))) {brgh(rx_str[4]);}
		if(!(strncmp(rx_str,"stop",4))) {stop();}
	}
	return 0;
}