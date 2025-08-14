#include "si2c.h"
#include <util/delay.h>
#define delay 100

char i2c_start( uint8_t address ) {
    /* I2C Start bit sequence */
    SDAON();
    _delay_us(delay);
    SCLON();
    _delay_us(delay);
    SDAOFF();
    _delay_us(delay);
    SCLOFF();
    _delay_us(delay);
    /* Send address */
    return i2c_writebyte( address );
}

void i2c_stop( void ) {
    SDAOFF();
    _delay_us(delay);
    SCLON();
    _delay_us(delay);
    SDAON();
    _delay_us(delay);
}

char i2c_writebyte( uint8_t byte ) {
    char x;
    uint8_t b;
    _delay_us(delay);
    for(x=8; x; x--) {
        if(byte & 0x80) {SDAON();}
        else {SDAOFF();}
        SCLON();
        byte <<= 1;
        _delay_us(delay);
        SCLOFF();
        _delay_us(delay);
    }
    SDAON();
    SCLON();
    _delay_us(delay);
    b = SDA;
    SCLOFF();
	_delay_us(delay);
    return b;
}

uint8_t i2c_readbyte( char ack ) {
    char x;
    uint8_t data = 0;
    SDAON();
    for(x=0; x<8; x++) {
        data <<= 1;
        SCLON();
        while( SCL == 0 ) SCLON();    // wait for any SCL clock stretching
        _delay_us(delay);
        if(SDA) data |= 1;
        SCLOFF();
        _delay_us(delay);
    }
    if(ack) {SDAOFF();}
    else {SDAON();}
    SCLON();
    _delay_us(delay);             // send (N)ACK bit
    SCLOFF();
    SDAON();
    _delay_us(delay);
    return data;
}
