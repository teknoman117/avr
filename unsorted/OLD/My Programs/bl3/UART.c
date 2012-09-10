

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
 
void     MAIN_PRG( void );
void     USART_Init( uint16_t ubrr );
void     USART_TX( uint8_t data );
//uint8_t  USART_RX( void );
void     DELAY_US( uint16_t microseconds );
void     DELAY_MS( uint16_t milliseconds );
#define MCU_FREQ 8000000UL
#define FOSC 8000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

int main( void )
{
	USART_Init(MYUBRR);
    MAIN_PRG();
    return 0;
}

void MAIN_PRG( void )
{ 
	while (1) 
	{
	  USART_TX("H");
      USART_TX(13);  
    }   
}

void USART_Init( uint16_t ubrr )
{
  UBRRH = (ubrr >> 8);
  UBRRL = ubrr;
  UCSRB = (1<<RXEN) | (1<<TXEN);
  UCSRC = (1<<USBS) | (3<<UCSZ0);
}

void USART_TX( uint8_t data )
{
  while ( !( UCSRA & (1<<UDRE)) )
  UDR = data;
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