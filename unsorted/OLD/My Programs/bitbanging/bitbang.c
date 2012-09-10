#include <stdint.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>

void     main_prg( void );
void     timerdly( uint16_t cycles );
void     bitbang( char banged );
uint16_t counter;
uint8_t ack;
SIGNAL (SIG_OVERFLOW0)
{
  counter++;
}

int main( void )
{
  TCCR0 = 1;
  sei();
  DDRB = DDRB + (1 << PB0);
  ack = 0;
  main_prg();
  return 0;
}

void main_prg( void ) 
{
  char string[] = "Hello World!\r\n";
  uint8_t index;
  while (1)
  {
    for(index=0;index<15;index++)
	{
	  bitbang(string[index]);
	}
  }
}

void timerdly( uint16_t cycles )
{
  TIMSK = TIMSK + (1 << TOIE0);
  counter = 0;
  while(counter < (cycles + 1));
  TIMSK = TIMSK - (1 << TOIE0);
}

void bitbang( char banged )
{
  uint8_t i;
  PORTB = 0;
  timerdly(26);
  for(i=0; i<8 ; i++)
  {
    PORTB = (banged >> i) & 0x01;
	timerdly(26);
  }
  PORTB = 1;
  timerdly(26);
}
  
  