/* Explore the filesystem on an SD card */

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ff.h"
#include "diskio.h"
#include "timer0ctc.h"
#include "stdusart0.h"

#define BAUD 5

ISR( TIMER0_COMPA_vect ) { // runs every 10ms
	disk_timerproc();      // clock fat driver timer variables
}

DWORD get_fattime () { return 0; }
char ch = 0;
int deepness = 0;

void scandir( char* path ) {
	printf( "%s\r\n", path );
	char npath[256];
	FILINFO fileinfo;
	DIR dir;
	if(f_opendir( &dir, path ) != FR_OK) {
		printf( "System Error\n\r" );
		PORTD |= (1 << PORTD4);                        //Set LED to Error
		PORTD &= ~(1 << PORTD5);
	} else {
		deepness++;
		f_readdir( &dir, &fileinfo );
        while( fileinfo.fname[0] != '\0' ) {
			if( fileinfo.fattrib & AM_DIR ) {
				if(strlen(path) > 1) sprintf( npath, "%s/%s\0", path, fileinfo.fname );
				else {
					sprintf( npath, "/%s\0", fileinfo.fname );
				}
				if( deepness < 8 ) scandir( npath );
				else {
					printf( "Path \"%s\" too deep\r\n", npath );
				}
            } else {
				if(strlen(path) > 1) printf( "%s/%s\r\n", path, fileinfo.fname );
				else {
					printf( npath, "/%s\0", fileinfo.fname );
				}				
            }
            f_readdir( &dir, &fileinfo );
		}
		deepness--;
	}
}

int main ( void ) {
	init_stdusart0( BAUD, DB8 | P_N | SB1 );
	init_timer0_ctc( T0_PRESCALER_1024, PIN_DISCONNECT, PIN_DISCONNECT );// initalize timer 0 in ctc mode with a 1024 prescale
	DDRD |= (1 << PORTD4) | (1 << PORTD5);                              //set status LED pins to outputs
	set_ocr0a( 108 );                                                    // compare match every 156 ticks, producing 10ms interrupts
	enable_interrupt_t0a();                                             //enable interrupt on timer0 channel A          
	static FATFS FATFS_Obj;                                             //file system descriptor
	disk_timerproc();                                                   //produce a base timer clock
	DSTATUS status = disk_initialize(0);                                //initalize the SD card
	if( status & STA_NOINIT ) {
		printf( "Disk Error\n\r" );
		PORTD |= (1 << PORTD4);                        //Set LED to Error
		PORTD &= ~(1 << PORTD5);
		while (1);
	}
	PORTD |= (1 << PORTD5);                                             //set LED to Success
	PORTD &= ~(1 << PORTD4);
	f_mount(0, &FATFS_Obj);                                             //mount the filesystem
	printf( "Exploring filesystem\r\n" );
	scandir( "/" );
	printf( "Done\r\n" );
	while(1);
	return 0;
}
