#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer1PWM.h"

void initPWM( void )
{
  TCCR1A = ( 1 << WGM10 ) | ( 1 << COM1A1 ) | ( 1 << COM1B1 );
  TCCR1B = ( 1 << CS10  ) | ( 1 << WGM12  );
}

void setCHA( uint16_t valueA )
{
  OCR1A = valueA;
}

void setCHB( uint16_t valueB )
{
  OCR1B = valueB;
}
    