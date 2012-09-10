

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
 
uint16_t ADCIN( uint8_t channel );
void     MAIN_PRG( void );
void     DELAY_US( uint16_t microseconds );
void     DELAY_MS( uint16_t milliseconds );
#define MCU_FREQ 8000000UL

int main( void )
{
    DDRB = 255;
	DDRD = 255; 
    cli();
    TIMSK = 0;
    TCCR1A = ( 1 << WGM10 ) | ( 1 << COM1A1 ) | ( 1 << COM1B1 );
    TCCR1B = ( 1 << CS10  ) | ( 1 << WGM12  );
    ADCSR = ( 1 << ADEN  ) | ( 1 << ADSC  )
          | ( 1 << ADPS2 ) | ( 1 << ADPS1 );
    while ( ADCSR & ( 1 << ADSC ) );
    MAIN_PRG();
    return 0;
}

void MAIN_PRG( void )
{ 
    uint16_t counter;
	for (;;)
	{
	  PORTB = PORTB + 168;
	  OCR1A = 90;
	  OCR1B = 90;
	  counter = 90;
      while(counter < 255)
	  {
	    OCR1A = counter;
		OCR1B = counter;
		DELAY_MS(5);
		counter = counter + 1;
	  }
      while(counter > 90)
	  {
	    OCR1A = counter;
		OCR1B = counter;
		DELAY_MS(5);
		counter = counter - 1;
	  }
	  PORTB = PORTB - 88;
	  DELAY_MS(1000);
	  OCR1A = 90;
	  OCR1B = 90;
	  counter = 90;
      while(counter < 255)
	  {
	    OCR1A = counter;
		OCR1B = counter;
		DELAY_MS(5);
		counter = counter + 1;
	  }
      while(counter > 90)
	  {
	    OCR1A = counter;
		OCR1B = counter;
		DELAY_MS(5);
		counter = counter - 1;
	  }
	  PORTB = 0;
	  // motor turn in opposing direction from here
	  PORTB = PORTB + 8;
	  PORTB = PORTB + 32;
	  PORTB = PORTB + 64;
	  while(counter < 255)
	  {
	    OCR1A = counter;
		OCR1B = counter;
		DELAY_MS(5);
		counter = counter + 1;
	  }
	  while(counter > 90)
	  {
	    OCR1A = counter;
		OCR1B = counter;
		DELAY_MS(5);
		counter = counter - 1;
	  }
	  PORTB = 0;
	  PORTB = PORTB + 16;
	  PORTB = PORTB + 128;
	  while(counter < 255)
	  {
	    OCR1A = counter;
		OCR1B = counter;
		DELAY_MS(5);
		counter = counter + 1;
	  }
	  while(counter > 90)
	  {
	    OCR1A = counter;
		OCR1B = counter;
		DELAY_MS(5);
		counter = counter - 1;
	  }
	  PORTB = 0;
   }   
}

uint16_t ADCIN( uint8_t channel )
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