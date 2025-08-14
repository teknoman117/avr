#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/iotnx5.h>

// Simulate a quadrature encoder pulse chain
int main ()
{
    // Set DDRs for sonar
	DDRB |=  (1 << PORTB0);  // qenc.cha
	DDRB |=  (1 << PORTB1);  // qenc.chb

	// Loop forever
	while (1)
	{
		// Set state to 0,0
		PORTB = 0;
		_delay_ms(100);

		// Set state to 1,0
		PORTB = 2;
		_delay_ms(100);

		// Set state to 1,1
		PORTB = 3;
		_delay_ms(100);

		// Set state to 0,1
		PORTB = 1;
		_delay_ms(100);
	}

	// Should never get here
	return 0;
}
