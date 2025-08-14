#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
void initADC( void )
{
  ADCSR = ( 1 << ADEN  ) | ( 1 << ADSC  )
        | ( 1 << ADPS2 ) | ( 1 << ADPS1 );
  while ( ADCSR & ( 1 << ADSC ) );
}

uint16_t ADC08bit( uint8_t channel )
{
    /* set for 8-bit results for the desired channel number then start the
       conversion; pause for the hardware to catch up */
    
    ADMUX  = ( 1 << ADLAR ) | ( 1 << REFS0 ) | channel;
    ADCSRA = ( 1 << ADEN  ) | ( 1 << ADSC  );
    DELAY_US( 64 );

    /* wait for complete conversion and return the result */

    while ( ADCSR & ( 1 << ADSC ) );
    
    return ADCH;
}

uint16_t ADC10bit( uint8_t channel )
{
  convert = ADC08bit(channel);
  convert = convert << 2;
  convert = convert + (ADCL >> 6);
  return convert;
}

void DELAY_US( uint16_t microseconds )
{

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

