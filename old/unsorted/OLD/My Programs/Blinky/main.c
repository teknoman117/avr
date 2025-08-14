#include <avr/io.h>

/* This example flashes the LED's on STK500, it handles GCC
   tendency to trash empty delay loops by inserting volatile inline asm. */
   
void wait (void) 
{
	asm volatile ("nop"::);
}

int main (void) 
{
	unsigned char i;
	unsigned long j;
	
	DDRB = 0xFF;			// Configure PORTB as output
	
	for(;;) 
	{
		for (i=0x01; i<0x80; i<<=1) 
		{
			PORTB = ~i;
			
			/* Delay */
			for (j=0; j<20000; j++) wait();
		}
		
		for (i=0x80; i>0x01; i>>=1) 
		{
			PORTB = ~i;
			
			/* Delay, the delay loops defined in avr/delay.h, could also be used */
			for (j=0; j<20000; j++) wait();
		}
	}
}

