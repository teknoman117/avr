#define F_CPU 11059200UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <t1servo.h>

int main (void) {
    enableServosT1(PORTD4, PORTD5, &DDRD, &DDRD);
    int8_t cnt=0;
    while (1) {
        for(cnt=-90;cnt<90;cnt++) {
            setAngleT1A(cnt);
            setAngleT1B(cnt*(-1));
            _delay_ms(50);
        }
        for(cnt=90;cnt>-90;cnt--) {
            setAngleT1A(cnt);
            setAngleT1B(cnt*(-1));
            _delay_ms(50);
        }
    }
    return 0;
}
