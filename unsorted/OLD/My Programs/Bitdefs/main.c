#include <avr/io.h>

// Define the bits in the port 
typedef struct 
{ 
   unsigned char bit0 : 1, 
                 bit1 : 1, 
                 bit2 : 1, 
                 bit3 : 1, 
                 bit4 : 1, 
                 bit5 : 1, 
                 bit6 : 1, 
                 bit7 : 1; 
} bit_field; 

// Define macro to get the value of each bit 
#define GET_BIT(port) (*(volatile bit_field *) (_SFR_ADDR(port))) 

// Define functions for each bit of the I/O ports in the program 
#define SIG    GET_BIT(PINB).bit0
#define LED	   GET_BIT(PORTD).bit0

int main (void)
{
	for (;;)
	{
		if (SIG) LED = 1;
			else LED = 0;
	}
}
 

