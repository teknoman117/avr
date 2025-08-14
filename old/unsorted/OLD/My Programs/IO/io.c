

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "servo.h" 
#define MCU_FREQ 8000000UL

void     MAIN_PRG( void );
void     DELAY_US( uint16_t microseconds );
void     DELAY_MS( uint16_t milliseconds );

int main( void )
{
    initservos(625,ON,OFF);
    DDRD = 0;
	DDRB = DDRB + (1 << DDB2);
    MAIN_PRG();
    return 0;
}

void MAIN_PRG( void )
{ 
    uint8_t ack;
	uint8_t count;
	uint8_t dir;
	count = 28;
	dir = 0;
	ack = 0;
	for (;;)
	{
	  if(count == 28){dir = 1;}
	  if(count == 74){dir = 0;}
      if(((PIND >> PD0) & 0x01) == 1)
	  { 
	    if(ack == 0){PORTB = PORTB + (1 << PB2);ack = 1;}
		if(dir == 1){count++;}
		if(dir == 0){count--;}
		setservo1(count);
	    while(((PIND >> PD0) & 0x01) == 1);
	  }
	  if(((PIND >> PD0) & 0x01) == 0)
	  {
	    if(ack == 1){PORTB = PORTB - (1 << PB2);ack = 0;}
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