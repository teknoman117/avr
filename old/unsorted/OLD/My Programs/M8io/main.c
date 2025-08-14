#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "M8io.h"
#define MCU_FREQ 8000000UL
void main_prg( void );
void     DELAY_US( uint16_t microseconds );
void     DELAY_MS( uint16_t milliseconds );

int main (void)
{
  initIO();
  setIOD(3,O);
  setIOD(4,O);
  main_prg();
  return 1;
}

void main_prg( void )
{
  setStateD(4,0);
  for(;;)
  {
    setStateD(3,1);
    DELAY_MS(400);
    setStateD(3,0);
    DELAY_MS(600);  
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