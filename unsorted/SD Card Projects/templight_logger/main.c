/* Log the of resistances of a photoresistor and thermisitor every 5 seconds */

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "a2d.h"
#include "ff.h"
#include "diskio.h"
#include "timer0ctc.h"
#include "stdusart0.h"
 
#define BAUD 5           //115200 baud at 11.0592 MHz

DWORD get_fattime () { return 0; }
char ch = 0;

ISR( TIMER0_COMPA_vect ) { // runs every 10ms
	disk_timerproc();      // clock fat driver timer variables
}

int main ( void ) {
	initADC();
	init_stdusart0( BAUD, DB8 | P_N | SB1 );
	init_timer0_ctc( T0_PRESCALER_1024, PIN_DISCONNECT, PIN_DISCONNECT );// initalize timer 0 in ctc mode with a 1024 prescale
	DDRD |= (1 << PORTD4) | (1 << PORTD5);                              //set status LED pins to outputs
	set_ocr0a( 108 );                                                    // compare match every 156 ticks, producing 10ms interrupts
	enable_interrupt_t0a();                                             //enable interrupt on timer0 channel A          
	static FATFS FATFS_Obj;                                             //file system descriptor
	disk_timerproc();                                                   //produce a base timer clock
	FIL logfile;       													//file descriptor for datalogging file
	FILINFO info;                                                       //file info structure
	DSTATUS status = disk_initialize(0);                                //initalize the SD card
	if( status & STA_NOINIT ) {                                         //check for an error
		printf( "Disk Error\n\r" );
		PORTD |= (1 << PORTD4);                                         //Set LED to Error
		PORTD &= ~(1 << PORTD5);
		while (1);
	}
	PORTD |= (1 << PORTD5);                                             //set LED to Success
	PORTD &= ~(1 << PORTD4);
	f_mount(0, &FATFS_Obj);                                             //mount the filesystem
	double vout = 0.0;
	unsigned photo = 0, therm = 0, in = 0;
	while(1) {
		_delay_ms(5000);             //delay 5 seconds
		PORTD |= (1 << PORTD4);      //turn on the writing status
		int res = f_stat( "/avr/datalog.txt", &info );    //find the length of the datalog file
		if(f_open(&logfile, "/avr/datalog.txt", FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) { //open the datalog and create anew ifnot present
			printf("System Error"); 
			PORTD |= (1 << PORTD4);                        //Set LED to Error
			PORTD &= ~(1 << PORTD5);	
			while(1);
        }		
		if(res == FR_OK) f_lseek( &logfile, info.fsize ); //If the file existed seek to the end
		in = getADC(0);                                   //Log the values
		vout = 3.3 * (in/1023.0);
		photo = (vout*10000.0)/(3.3-vout);
		printf( "Photo: %u ohms(%u raw) ", photo, in );
		f_printf( &logfile, "Photo: %u ohms(%u raw) ", photo, in );
		in = getADC(1);
		vout = 3.3 * (in/1023.0);
		therm = (vout*10000.0)/(3.3-vout);
		printf( "Therm: %u ohms(%u raw) \r\n", therm, in );
		f_printf( &logfile, "Therm: %u ohms(%u raw)\n", therm, in );
		f_close( &logfile );       //close the file
		PORTD &= ~(1 << PORTD4);   //off with the writing status
	}
	return 0;
}
