#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(TIMER1_OVF_vect) {
    if (PORTB & _BV(PORTB1)) {
        PORTB &= ~_BV(PORTB1);
    } else {
        PORTB |= _BV(PORTB1);
    }
}

int main() {
    DDRB = _BV(PORTB1) | _BV(PORTB0);
    TCCR1A = 0;
    TCCR1B = _BV(CS11) | _BV(CS10);
    TIMSK1 = _BV(TOIE1);
    sei();

    // spin forever
    while (1) {
        PORTB |= _BV(PORTB0);
        _delay_ms(1000);
        PORTB &= ~_BV(PORTB0);
        _delay_ms(1000);
    }
    return 0;
}
