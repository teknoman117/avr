#include <avr/io.h>
#include <util/delay.h>

void main(void) {
    DDRB |= _BV(PORTB5);

    while (1) {
        PORTB |= _BV(PORTB5);
        _delay_ms(500);
        PORTB &= ~_BV(PORTB5);
        _delay_ms(500);
    }
}
