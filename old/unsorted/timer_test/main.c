#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdusart0.h>
#include <a2d.h>

#define T1_PRESCALER_1 (1 << CS10)
#define T1_PRESCALER_8 (1 << CS11)
#define T1_PRESCALER_64 (1 << CS11) | (1 << CS10)
#define T1_PRESCALER_256 (1 << CS12)
#define T1_PRESCALER_1024 (1 << CS12) | (1 << CS10)

void init_timer1 ( unsigned char prescaler, unsigned short top ) {
	TCNT1 = 0;
	ICR1 = top;
	TCCR1A = 0;
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (prescaler & 0x07);
}	

int main ( ) {
	init_stdusart0(BAUD(38400), DB8 | P_N | SB1);
	init_timer1(T1_PRESCALER_1024, 32787);
	initADC();
	printf("AVR Timer Test Program\r");
	srand(getADC(3));
	while (1) {
		printf("Current Value of TCNT1: %u\r", TCNT1);
		_delay_ms(((rand() % 5) + 1) * 200);
	}
	return 0;
}
		
