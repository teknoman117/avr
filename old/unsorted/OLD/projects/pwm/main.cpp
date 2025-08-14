#define F_CPU 11059200UL
#include <util/delay.h>
#include <avr/io.h>
#include "PWM.h"

int main () {
	DDRD = 0xFF;
	DDRC = 0xFF;
	PORTC = 0;
	//if((PORTC >> 0) & 1) {PORTC -= (1 << 0);}
	PORTD = 128;
	_delay_ms(1000);
	PORTD = 0;
	PWM led(0);
	int c;
	while (1) {
        /*led.setvalue(255);
        _delay_ms(1000);
        led.setvalue(0);
        _delay_ms(1000);
        led.setvalue(128);
        _delay_ms(1000);
        led.setvalue(0);
        _delay_ms(1000);*/	
        c = 0;
        while (c < 255) {
        	led.setvalue(c);
        	_delay_ms(4);
        	c++;
        }
        while (c > 255) {
        	led.setvalue(c);
        	_delay_ms(8);
        	c--;
        }
   	}
	return 1;
}
	
