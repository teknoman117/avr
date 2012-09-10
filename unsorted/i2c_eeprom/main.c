#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "i2c.h"
#include "stdusart0.h"

int main( void ) {
	uint8_t c;
	uint16_t v;
	init_i2c( 255, I2C_PRESCALER_1 );
	init_stdusart0( 5, DB8 | P_N | SB1 );
	printf( "I2C EEprom Test\r\n" );
	printf("Addressing eeprom at 0xA0(0)\r\n");
	if( i2c_start(0xA0) < 0 ) {
		printf( "Failed\r\n" );
		while(1);
	}
	_delay_ms(10);
	printf( "Success\r\n" );
	i2c_txb( 0x00 );
	_delay_ms(10);
	for(c=0;c<10;c++) {
		if(i2c_txb(c*2) != 0) {printf("Err @ %u\r\n", c);break;}
		_delay_ms(10);
	}
	i2c_stop();
	_delay_ms(10);
	printf("Addressing eeprom at 0xA0(1)\r\n");
	if( i2c_start(0xA0) < 0 ) {
		printf( "Failed\r\n" );
		while(1);
	}
	printf( "Success\r\n" );
	i2c_txb( 0x00 );
	printf("Addressing eeprom at 0xA0(2)\r\n");
	if( i2c_start(0xA1) < 0 ) {
		printf( "Failed\r\n" );
		while(1);
	}
	printf( "Success\r\n" );
	for(c=0;c<9;c++) {
		uint8_t b = i2c_rxbAck();
		printf( "Got %u\r\n", b );
	}
	uint8_t b = i2c_rxbNack();
	printf( "Got %u\r\n", b );
	i2c_stop();
	_delay_ms(2500);
	printf( "I2C Sonar Test\r\n" );
	while(1) {
		if( i2c_start(0xE0) < 0 ) {
			printf( "Failed(0)\r\n" );
			while(1);
		}
		i2c_txb( 0x00 );
		if(i2c_txb( 0x51 ) != 0) {
			printf("Error\r\n");
			while(1);
		}
		i2c_stop();
		//_delay_ms(1);
		if( i2c_start(0xE0) < 0 ) {
			printf( "Failed(1)\r\n" );
			while(1);
		}
		_delay_ms(5);
		i2c_txb( 0x02 );
		_delay_ms(5);
		if( i2c_start(0xE1) < 0 ) printf( "Failed(2)\r\n" );
		else {
			v = i2c_rxbAck();
			v <<= 8;
			v += i2c_rxbNack();
			printf( "Got %u\r\n", v );
		}
		i2c_stop();
	}
	return 0;
}