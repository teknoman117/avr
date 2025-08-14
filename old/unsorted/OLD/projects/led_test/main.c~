#define F_CPU 11059200UL
#include <avr/io.h>
#include <util/delay.h>

int main () {
    DDRB=0xFF;
    double x;
    while (1) {
        PORTB=0x01;
        for(x=0;x<100;x++){_delay_ms(10);}
        PORTB=0x00;
        for(x=0;x<100;x++){_delay_ms(10);}
    }
    return 0;
}

