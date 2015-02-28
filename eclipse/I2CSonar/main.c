#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/iotnx5.h>

#include "usitwislave.h"

// Ranging mode enum
typedef enum
{
	Ranging_Mode_Inches = 0,
	Ranging_Mode_Centimeters = 1,
	Ranging_Mode_Microseconds = 2,
} Ranging_Mode;

// address of the I2C sonar
extern volatile uint8_t slave_address;

// I2C Callback
static void twi_callback(uint8_t buffer_size, volatile uint8_t input_buffer_length, volatile const uint8_t *input_buffer, volatile uint8_t *output_buffer_length, volatile uint8_t *output_buffer)
{
	// If we have received a ranging command, perform a ranging operation.  The i2c unit will not re-enable until this process is complete
	if(input_buffer_length && input_buffer[0] >= 0 && input_buffer[0] <= 2)
	{
		// Check if the line is still high from lost echo, if so, just return DISTANCE_MAX
		if(PINB & (1 << PORTB3))
		{
			// Output distance of 65535 no matter the request
			output_buffer[0] = 0xff;
			output_buffer[1] = 0xff;
			*output_buffer_length = 2;

			// Forcefully return
			return;
		}

		// Variable to hold the time count
		uint16_t distance = 0;

		// Reset the timer
		TCNT0        = 0;
		TIFR        |= (1 << OCF0A);

		// Trigger the sonar (hold line high)
		PORTB |= (1 << PORTB4);

		// Wait for the compare match flag to be set (10 us wait)
		while(!(TIFR & (1 << OCF0A)));
		TIFR |= (1 << OCF0A);

		// Release sonar trigger
		PORTB &= ~(1 << PORTB4);

		// Wait for the echo pulse
		while(!(PINB & (1 << PORTB3)));

		// Reset the timer
		TCNT0        = 0;

		// Wait for the end of the pulse
		while(PINB & (1 << PORTB3))
		{
			// Check if the distance has gotten too great (echo probably lost)
			if(distance > 29000)
			{
				// Output distance of 65535 no matter the request
				output_buffer[0] = 0xff;
				output_buffer[1] = 0xff;
				*output_buffer_length = 2;

				// Forcefully return
				return;
			}

			// Delay 10 us
			while(!(TIFR & (1 << OCF0A)));
			TIFR |= (1 << OCF0A);

			// Increment distance by 10 us
			distance += 10;
		}

		// Check measurement type
		if(input_buffer[0] == Ranging_Mode_Inches)
		{
			// Convert microseconds to inches
			distance = distance / 148;
		} else if(input_buffer[0] == Ranging_Mode_Centimeters)
		{
			// Convert microseconds to Centimeters
			distance = distance / 58;
		}

		// Put the distance on the output buffer
		output_buffer[0] = (distance >> 8) & 0x007F; // Mask the high bit on high byte (part of not ready identifier)
		output_buffer[1] = (uint8_t)(distance & 0x00FF);
		*output_buffer_length = 2;
	} else if(input_buffer_length && input_buffer[0] == 0xa0 && input_buffer[1] == 0xaa && input_buffer[2] == 0xa5)
	{
		// Change address mode
		if(input_buffer[3] == 0x00 || input_buffer[3] == 0x7f) return;

		// Write address to eeprom
		while(EECR & (1 << EEPE));

		// Set programming mode
		EECR = 0;

		// Set address and data registers
		EEAR = 0;
		EEDR = input_buffer[3];

		// Start the eeprom write
		EECR |= (1 << EEMPE);
		EECR |= (1 << EEPE);

		// Update our address
		slave_address = input_buffer[3];
	}
}

int main ()
{
    // Set DDRs for sonar
	DDRB |=  (1 << PORTB1);
	DDRB |=  (1 << PORTB4);      // PortB.4 is the sonar trigger pin, set to output
    DDRB &= ~(1 << PORTB3);      // PortB.3 is the sonar echo pin, set to input

    // Setup a timer to time the echo pulse
    TCCR0A  = (1 << WGM01);
    TCCR0B  = (1 << CS00);
    OCR0A   = 80;

    // Load our address from eeprom
    while(EECR & (1<<EEPE));
    EEAR = 0;
    EECR |= (1 << EERE);

	// Initialize USI hardware
	usi_twi_slave(EEDR, 0, &twi_callback, 0);

	// Should never get here
	return 0;
}
