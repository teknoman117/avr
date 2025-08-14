

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
 
uint16_t ADCIN( uint8_t channel );
void     MAIN_PRG( uint16_t thresh );
uint16_t Cal_IRSN( void );
void     DELAY_US( uint16_t microseconds );
void     DELAY_MS( uint16_t milliseconds );
#define MCU_FREQ 8000000UL

int main( void )
{
    uint16_t threshold;
    DDRB = 255;
	DDRD = 255; 
    cli();
    TIMSK = 0;
    TCCR1A = ( 1 << WGM10 ) | ( 1 << COM1A1 ) | ( 1 << COM1B1 );
    TCCR1B = ( 1 << CS10  ) | ( 1 << WGM12  );
    ADCSR = ( 1 << ADEN  ) | ( 1 << ADSC  )
          | ( 1 << ADPS2 ) | ( 1 << ADPS1 );
    while ( ADCSR & ( 1 << ADSC ) );
	threshold = Cal_IRSN();
    MAIN_PRG(threshold);
    return 0;
}

void MAIN_PRG( uint16_t thresh )
{ 
    uint16_t buffer;
    uint8_t fwdon;
	uint8_t lfton;
	OCR1A = 255;
	OCR1B = 255;
	PORTB = PORTB + 136;
	fwdon = 1;
	lfton = 0;
	while (1) 
	{
	  buffer = ADCIN(3);
      if(buffer < thresh && lfton == 0)
      {
        fwdon = 0;
        lfton = 1;
        PORTB = PORTB - 64;
      }
      if(buffer > thresh && fwdon == 0)
      {
        lfton = 0;
        fwdon = 1;
		PORTB = PORTB + 64;
      }    
    }   
}

uint16_t Cal_IRSN( void )
{
  uint16_t buf;
  uint16_t Hval;
  uint16_t Lval;
  uint16_t cntr;
  cntr = 0;
  Hval = ADCIN(3);
  PORTD = PORTD + 4;
  PORTB = PORTB + 32;
  DELAY_MS(5000);
  PORTB = PORTB - 32;
  Lval = ADCIN(3);
  buf = Hval;
  while(buf != Lval)
  {
    cntr++;
	buf--;
  }
  cntr = cntr / 2;
  buf = Hval - cntr;
  return buf;
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