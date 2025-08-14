#include <avr/io.h>
#include <avr/interrupt.h>

ISR (PCINT0_vect) {          //if the PCINT interrupt on PORTA is raised
    PORTB = PINA;            //set portb to the input from porta
}

int main ( void ) {
    DDRA = 0x00;             //set all porta pins to inputs
    DDRB = 0xff;             //set all portb pins to outputs
    PCICR = (1 << PCIE0);    //enable the pcint0 interrupt
    PCMSK0 = 0xFF;           //let any porta pin change raise pcint0
    sei();                   //turn on interrupts
    while (1);               //wait forever, interrupts runs everything
    return 0;
}
