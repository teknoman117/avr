/* ATTiny85 USB-I2C Converter */

/*
 * Commands
 * Writing: address:w:register:data[:datan]
 * Reading: address:r:register:bytes to read
 *
 */

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "bitbang.h"
#include "si2c.h"

#define delay 5

int main(void) {
    uint8_t len, address, reg, count, val, c;
    char *element, *backup;
    char msg[256];
    i2c_init();
    DDRB |= (1 << PORTB3);
    DDRB &= ~(1 << PORTB4);
	printstr( &PORTB, PORTB3, "Tiny85 I2C\r" );
    while(1) {
        len = getstr( &PINB, PORTB4, msg, '\r' );
        msg[len] = '\0';
        address = atoi(strtok_r( msg, ":", &backup ));
        address &= 0xFE;
        element = strtok_r( NULL, ":", &backup );
        reg = atoi(strtok_r( NULL, ":", &backup ));
        if((element[0] == 'r') || (element[0] == 'R')) {
            printstr( &PORTB, PORTB3, "ACK" );
            i2c_start(address);
            i2c_writebyte( reg );
			_delay_ms(5);
            count = atoi(strtok_r( NULL, ":", &backup ));
            i2c_start(address | 0x01);
			_delay_ms(5);
            for(c=0;c<count-1;c++) {
                sprintf( msg, ":%u\0", i2c_readbyte(1) );
                printstr( &PORTB, PORTB3, msg );
				_delay_us(500);
            }
            sprintf( msg, ":%u\r\0", i2c_readbyte(0) );
            printstr( &PORTB, PORTB3, msg );
			_delay_us(500);
            i2c_stop();
        } else if((element[0] == 'w') || (element[0] == 'W')) {
            printstr( &PORTB, PORTB3, "ACK" );
			uint16_t counter = 0;
            i2c_start(address);
			_delay_ms(delay);
            i2c_writebyte( reg );
			_delay_ms(delay);
            element = strtok_r(NULL, ":", &backup);
            while(element != NULL) {
                val = atoi(element);
				counter++;
                if(i2c_writebyte(val)) break;
                element = strtok_r(NULL, ":", &backup);
				_delay_us(500);
            }
            i2c_stop();
            sprintf( msg, ":%u\r\0", counter );
            printstr( &PORTB, PORTB3, msg );
        }
    }
    return 0;
}
