#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

ISR (INT0_vect) {       //if int0 is triggered
    if(PORTB & 1) {     //if portb bit 0 is 1
    	PORTB &= ~(1 << PORTB0); //clear portb bit 0
    }
    else {                     //otherwise
    	PORTB |= (1 << PORTB0);//set portb bit 0
    }
}

int main ( void ) {
	DDRB = 0xFF;
	DDRD &= ~(1 << PORTD2); //input pin where int0 is located
	EIMSK |= (1 << INT0);   //enable int0
	EICRA |= (1 << ISC01);   //trigger int0 on falling edge of int0 pin
	sei();      //enable interrupts
	while (1);  //loop forever, interrupts run system
	return 0;
}
	
