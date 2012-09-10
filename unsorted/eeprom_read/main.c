#include <stdio.h>
#include <util/delay.h>

#include <eeprom.h>
#include <stdusart0.h>

int main () {
	init_stdusart0(BAUD(38400), DB8 | P_N | SB1);
	printf("Dump AVR EEprom of size %u bytes\r", E2END + 1);
	_delay_ms(5000);
	unsigned short cntr = 0;
	for(cntr=0;cntr<E2END+1;cntr++) {
		unsigned char b = eeprom_read(cntr);
		printf("Byte (%u) = %u\r", cntr, b);
	}
	while(1); // In an embedded application, NEVER exit the program
	return 0;
}
