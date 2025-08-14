#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include "servo.h"
#define MCU_FREQ 16000000UL
void     MAIN_PRG( void );
void     DELAY_US( uint16_t microseconds );
void     DELAY_MS( uint16_t milliseconds );
uint8_t count;
int main( void )
{
	initservos(625,ON,ON);
    MAIN_PRG();
    return 0;
    MCUCR = MCUCR + ((1<<ISC11) + (1<<ISC01));
    GICR = GICR + ((1<<INT0) + (1<<INT1));
    sei(); 
}

void MAIN_PRG( void )
{ 
    count = 20;
    setservo1(20); 
    setservo2(20);   
	for (;;)
	{
      while(count < 74)
      { 
        setservo1(count);
		setservo2(count);
	    count++;
	    DELAY_MS(50);
      }
      while(count > 19)
      { 
        setservo1(count);
		setservo2(count);
	    count--;
	    DELAY_MS(50);
      }
	  //setservo1(count);
	  //DELAY_MS(50);
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

/*SIGNAL(SIG_INTERRUPT0) {
  count++;
  if(count > 73){count = 73;}  
}

SIGNAL(SIG_INTERRUPT1) {
  count--;
  if(count < 20){count = 20;}
}*/
