#include <avr/io.h>
#include <util/delay.h>

int main ()
{
    // Initialize digital io
    DDRA  = 0xFF;
    PORTA = 0x00;

    // Forever loop
    while (1)
    {
        PORTA = 0x01;
        _delay_ms(500);
        PORTA = 0x00;
        _delay_ms(500);
        PORTA = 0x02;
        _delay_ms(500);
        PORTA = 0x00;
        _delay_ms(500);
        PORTA = 0x04;
        _delay_ms(500);
        PORTA = 0x00;
        _delay_ms(500);
        PORTA = 0x08;
        _delay_ms(500);
        PORTA = 0x00;
        _delay_ms(500);
    }

    // Return control to libgcc.a
    return 0;
}
