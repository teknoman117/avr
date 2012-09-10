/* This program tests USART0 in interrupt driven mode */

#include <stdio.h>
#include <util/delay.h>
#include <iusart0.h>

volatile char string_dat[64];

int main ( void ) {
    DDRD = _BV(PORTD1);
    init_iusart0(BAUD(38400), DB8 | P_N | SB1 );
    char* str = (char*) string_dat;
    while (1) {
    	unsigned short bs = bytesInBuf0();
    	sprintf(str, "Bytes in buf: %u, most recent: %c\r", bs, (char) rxbi0());
    	writeStringi0(str);
		_delay_ms(500);
    }
    return 0;
}
