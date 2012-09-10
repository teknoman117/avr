#define F_CPU 11059200UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <ioctl8.h>

//range 990=750us
//      2074=1500us
//      2970=2250us
//      magic number = 1us = 1.3204 timer clocks when divided by 8
int main (void) {
    uint16_t cnt;
    TCCR1A = _BV(COM1A1)
      | _BV(COM1B1)
      | _BV(WGM11); 
    TCCR1B = _BV(WGM13)
      | _BV(WGM12)
      | _BV(CS11);
    //ICR1 = 20000;
    ICR1 = 27648;
    //ICR1 = 13824;
    OCR1A = 2074;
    //OCR1B = 2074;
    set8(PORTD6, &DDRD); 
    set8(PORTD5, &DDRD);
    set8(PORTD4, &DDRD); 
    cnt = 0;
    while (1) {
        set8(PORTD6, &PORTD); 
        cnt = 990;
        while(cnt < 2970) {
            cnt += 31;
            OCR1B = cnt;
            _delay_ms(75);
        }
        clear8(PORTD6, &PORTD);
        cnt = 2970;
        while(cnt > 990) {
            cnt -= 31;
            OCR1B = cnt;
            _delay_ms(75);
        }
    }
    return 0;
}
