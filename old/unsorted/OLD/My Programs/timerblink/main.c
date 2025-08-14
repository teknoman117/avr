#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include "ISR.h"
#define MCU_FREQ 8000000UL

void     DELAY_US( uint16_t microseconds );
void     DELAY_MS( uint16_t milliseconds );
void     MAIN_PRG( void );

int main( void )
{  
	DDRB = 255; 
	start();
    MAIN_PRG();
    return 0;
}

void MAIN_PRG( void )
{ 
    uint16_t counter;
	uint8_t dir;
	dir = 0;
	setONcycles(24000);
	counter = 20;
	setOFFcycles(48000);
	for (;;)
	{
      while(counter < 74)
      { 
        setONcycles(counter);
	    counter++;
	    DELAY_MS(50);
      }
      while(counter > 17)
      { 
        setONcycles(counter);
	    counter--;
	    DELAY_MS(50);
      }   
	}    
}

void DELAY_US( uint16_t microseconds )
{
    register uint16_t loop_count;

#if MCU_FREQ == 8000000UL

    /* 8mhz clock, 4 instructions per loop_count  */
    loop_count = microseconds * 2;

#elif MCU_FREQ == 1000000UL

    /* 1mhz clock, 4 instructions per loop_count */
    loop_count = microseconds / 4;

#elif MCU_FREQ == 16000000UL

    /* 1mhz clock, 4 instructions per loop_count */
    loop_count = microseconds / 4;

#else
#error MCU_FREQ undefined or set to an unknown value!
    loop_count = 0; /* don't really know what to do */
#endif

    __asm__ volatile (
        "1: sbiw %0,1" "\n\t"
        "brne 1b"
        : "=w" ( loop_count )
        : "0"  ( loop_count )
    );
}

void DELAY_MS( uint16_t milliseconds )
{
    uint16_t i;

    for ( i = 0; i < milliseconds; ++i )
    {
        DELAY_US( 1000 );
    }
}
