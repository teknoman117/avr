#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
/* My custom headers */
#include "spi.h"
#include "mmc.h"
#include "stdusart0.h"

unsigned char waveheader[44];
unsigned char audiobuffer[512];
char ch = 0;

void sendDAC( uint8_t header, uint8_t data ) {
	PORTC &= ~(1 << PORTC2);
	_delay_us(1);
	uint8_t c;
	for(c=0;c<8;c++) {
		if(header & (1 << (7-c))) {PORTC |= (1 << PORTC0);}
		else {PORTC &= ~(1 << PORTC0);}
		PORTC |= (1 << PORTC1);
		_delay_us(1);
		PORTC &= ~(1 << PORTC1);
		_delay_us(1);
	}
	for(c=0;c<8;c++) {
		if(data & (1 << (7-c))) {PORTC |= (1 << PORTC0);}
		else {PORTC &= ~(1 << PORTC0);}
		PORTC |= (1 << PORTC1);
		_delay_us(1);
		PORTC &= ~(1 << PORTC1);
		_delay_us(1);
	}
	PORTC |= (1 << PORTC2);
}


int main ( void ) {
	// Variables
	unsigned int bc = 0;
	uint32_t cntr = 0;
	// code
	init_stdusart0( 5, DB8 | P_N | SB1 );                            // initalize my stdin/stdout driver
	init_spi( FOSC_2, MODE0, MSBFIRST );
	DDRC |= 0xFF;
	PORTC |= (1 << PORTC2);
	sendDAC( 0x0B, 0xFF );
	DDRD |= 0xF0;                         //set status LED pins to outputs along with DAC controls
	DDRB |= (1 << PORTB4);
	PORTB &= ~(1 << PORTB4);	// set chip select to low (MMC is selected)
	printf( "Audio Test Program\n\r" );
	// Rest of Program                             
	if( !init_mmc() ) {
		printf( "Disk Error\n\r" );
		PORTD |= (1 << PORTD4);                                         //Set LED to Error
		PORTD &= ~(1 << PORTD5);
		while (1);
	}
	PORTD |= (1 << PORTD5);                                             //set LED to Success
	PORTD &= ~(1 << PORTD4);
	printf( "Playing\n\r" );
	mmc_readblock( 0, audiobuffer );
	for( bc = 44; bc < 512; bc+=2 ) {
		sendDAC( 0x0B, audiobuffer[bc+1] );
		_delay_us(27);
	}
	cntr = 512;
	while( 1 ) {
		mmc_readblock( cntr, audiobuffer );
		for( bc = 0; bc < 512; bc+=2 ) {
			//printf("%u\r\n", audiobuffer[bc+1] );
			sendDAC( 0x0B, audiobuffer[bc+1] );
			_delay_us(27);
		}
		cntr+=512;
	}
	OCR2B = 0;	                                       //unmount the filesystem
	printf( "Done!\n\r" );
	while(1);
	return 0;
}

