#include <avr/io.h>
#include <util/delay.h>
#include "bitbang.h"

int main(void) {
    char msg[255];
    uint8_t len;
    DDRB |= (1 << PORTB3);
    DDRB &= ~(1 << PORTB4);
    while(1) {
        /*printstr( &PORTB, PORTB3, "Enter a string: " );
        len = getstr( &PINB, PORTB4, msg, '\r' );
        msg[len] = '\0';
        //txb_b(&PORTB, PORTB3, '\n');
        printstr( &PORTB, PORTB3, "You Entered: " );
        printstr( &PORTB, PORTB3, msg );
        printstr( &PORTB, PORTB3, "\r\n" );*/
        if(PINB&(1<<PORTB4)) {
            printstr( &PORTB, PORTB3, "On\n\r" );
        } else {
            printstr( &PORTB, PORTB3, "Off\n\r" );
        }
    }
    return 0;
}
