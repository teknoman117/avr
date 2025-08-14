#define F_CPU 11059200UL
#include <util/delay.h>
#include "test.h"

int main () {
    init();
    while (1) {
        ledON();
        _delay_ms(1000);
        ledOFF();
        _delay_ms(1000);
    }
    return 1;
}
