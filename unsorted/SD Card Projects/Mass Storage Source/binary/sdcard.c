#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>

#include "spi.h"
#include "mmc.h"
#include "stdusart0.h"

#define BAUD 5     //115.2kBaud @11.0592 MHz

uint8_t sector[512];

int main ( void ) {
	DDRB |= (1 << PORTB4);	    // set port B SPI chip select to output
	PORTB &= ~(1 << PORTB4);	// set chip select to low (MMC is selected)
	DDRD |= (1 << PORTD4) | (1 << PORTD5);                              //set status LED pins to outputs
	uint16_t i, b;
	unsigned int address = 0;
	init_stdusart0( BAUD, DB8 | P_N | SB1 );
	init_spi( FOSC_64, MODE0, MSBFIRST );
	printf("ATmega644 CPU Initalized\r\n");
	if( !init_mmc() ) {
		printf("Disk Error!\r\n");
		PORTD |= (1 << PORTD4);                        //Set LED to Error
		PORTD &= ~(1 << PORTD5);		
		while(1);
	}
	PORTD |= (1 << PORTD5);                            //set LED to Success
	PORTD &= ~(1 << PORTD4);	
	printf("SD Card Initalized\r\n");
	while(1) {
		printf("Enter the address to read from: ");
		scanf( "%u", &address );
		printf("Reading from SD Card...\r");
		mmc_readblock( 512, sector );
		for( i = 0; i < 512; i+=16 ) {
			for(b=0;b<16;b++) printf( "%x  ", sector[i+b]);
			printf( "\r\n" );
		}
	}
	return 0;
}
