//#define F_CPU 1000000

#include <stdio.h>
#include <util/delay.h>
#include <stdint.h>

#include <a2d.h>
#include <stdusart0.h>

int main ( void ) {
    initADC();
    init_stdusart0(BAUD(38400), DB8 | P_N | SB1);
    while (1) {
        printf("ADC Value: %u\r", getADC(1));
        _delay_ms(100);
    }
    return 0;
}

