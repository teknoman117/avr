#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "adc.h"
#include "timer1PWM.h"
 
void     MAIN_PRG( void );
void     DELAY_MS( uint16_t milliseconds );
//#define MCU_FREQ 16000000UL
int main( void )
{

    DDRD = 0xFF;
	DDRB = 0xFF;
    cli();
    TIMSK = 0;
    initPWM();
	initADC();
    MAIN_PRG();
    return 0;
}

void MAIN_PRG( void )
{ 
	while(1)
	{
      PORTB = ( 1 << PB0 );
	  DELAY_MS(50);
	  //setCHB(ADC08bit(0));
	  if(ADC08bit(0) < 100){setCHB(255);}
	  if(ADC08bit(0) > 99){setCHB(0);}
	  PORTB = ( 1 << PB1 );
	  DELAY_MS(50);
	  //setCHA(ADC08bit(0)); 
	  if(ADC08bit(0) < 100){setCHA(255);}
	  if(ADC08bit(0) > 99){setCHA(0);}
	}   
}

void DELAY_MS( uint16_t milliseconds )
{
    uint16_t i;

    for ( i = 0; i < milliseconds; ++i )
    {
        DELAY_US( 1000 );
    }
}