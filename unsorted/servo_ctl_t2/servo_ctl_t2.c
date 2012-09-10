#define F_CPU 11059200UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <t2servo.h>

int main (void) {
    enableServoT2B(PORTD6, &DDRD);
    int8_t cnt=0;
    //setAngleT2B(0);
    setAngleT2B(90);
    while (1) {
        /*for(cnt=-90;cnt<90;cnt++) {
            setAngleT2B(cnt);
            //setAngleT2B(cnt*(-1));
            _delay_ms(50);
        }
        for(cnt=90;cnt>-90;cnt--) {
            setAngleT2B(cnt);
            //setAngleT2B(cnt*(-1));
            _delay_ms(50);
        }*/
    }
    return 0;
}
