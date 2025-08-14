#define F_CPU 11059200UL
#include <util/delay.h>
#include "test_class.h"

int main () {
    test_class a;
    int b;
    while (1) {
        b = 0;
        while (b < 5) {
            a.led_on();
            _delay_ms(1000);
            a.led_off();
            _delay_ms(1000);
            b++;
        }
        b = 0;
        while (b < 5) {
            a.led_on();
            _delay_ms(250);
            a.led_off();
            _delay_ms(250);
            b++;
        }       
    }
    return 1;
}
