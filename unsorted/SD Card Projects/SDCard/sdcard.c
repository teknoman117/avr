#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>

#include "spi.h"
#include "mmc.h"
#include "stdusart0.h"

#define BAUD 5     //115.2kBaud @11.0592 MHz

uint8_t sector[512];
uint8_t *sect = (uint8_t*) sector;

int main ( void ) {
	DDRB |= (1 << PORTB4);	    // set port B SPI chip select to output
	PORTB &= ~(1 << PORTB4);	// set chip select to low (MMC is selected)
	uint16_t i;
	uint8_t v = 0;
	init_stdusart0( BAUD, DB8 | P_N | SB1 );
	init_spi( FOSC_64, MODE0, MSBFIRST );
	printf("ATmega644 CPU Initalized\r");
	if( !init_mmc() ) {
		printf("SD Card Error!\r");
		while(1);
	}
	printf("SD Card Initalized\r");
	printf("Filling Ram...\r");
	for (i=0;i<512;i++) {
		sector[i] = v++;
		if (v == 255) v = 0;
	}
	printf("Writing to SD Card...\r");
	if( mmc_writeblock( 512, sect ) ) while(1);;
	printf("Reading from SD Card...\r");
	mmc_readblock( 512, sect );
	for( i = 0; i < 512; i++ ) printf( "%d\r", sector[i]);
	printf("\rSuccess at last!!!!\r");
	while(1);
	return 0;
}
