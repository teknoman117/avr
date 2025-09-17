#include "uart.h"

#include <avr/interrupt.h>

#include <stdio.h>

int main() {
    setup_uart0();
    sei();

    // spin forever
    printf("AVR echo demo!\n");
    char buf[32];
    while (1) {
        printf("what\'s your name? ");
        scanf("%s", buf);
        printf("well, hello %s!\n", buf);
    }
    return 0;
}
