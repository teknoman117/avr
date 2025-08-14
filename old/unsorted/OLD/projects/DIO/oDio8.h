#include <avr/io.h>
#include <stdint.h>
/**ports----
group 0: PortA
group 1: PortB
group 2: PortC
group 3: PortD
**/
class oDio8 {
	uint8_t state;
	uint8_t port;
	uint8_t direction;
public:
    oDio8();
    oDio8(uint8_t group, uint8_t io);
    void setIOP(uint8_t group, uint8_t io);
    void set(uint8_t value);
    uint8_t get();
};

oDio8::oDio8() {state = port = direction = 0;}
oDio8::oDio8(uint8_t group, uint8_t io) {
	port = group;
	state = 0;
	direction = io;
	switch (group)
	{
		case 0: if(io) {DDRA=0xFF;PORTA=0x00;} else {DDRA=0x00;} break;
	    case 1: if(io) {DDRB=0xFF;PORTB=0x00;} else {DDRB=0x00;} break;
		case 2: if(io) {DDRC=0xFF;PORTC=0x00;} else {DDRC=0x00;} break;
		case 3: if(io) {DDRD=0xFF;PORTD=0x00;} else {DDRD=0x00;} break;
	}
}

void oDio8::setIOP(uint8_t group, uint8_t io) {
	port = group;
	state = 0;
	direction = io;
	switch (group)
	{
		case 0: if(io) {DDRA=0xFF;PORTA=0x00;} else {DDRA=0x00;} break;
	    case 1: if(io) {DDRB=0xFF;PORTB=0x00;} else {DDRB=0x00;} break;
		case 2: if(io) {DDRC=0xFF;PORTC=0x00;} else {DDRC=0x00;} break;
		case 3: if(io) {DDRD=0xFF;PORTD=0x00;} else {DDRD=0x00;} break;
	}
}	

void oDio8::set(uint8_t value) {
	state = value;
	switch (port) {
		case 0: PORTA = value; break;
		case 1: PORTB = value; break;
		case 2: PORTC = value; break;
		case 3: PORTD = value; break;
	}
}	

uint8_t oDio8::get() {	
	switch (port) {
		case 0: return PINA; break;
		case 1: return PINB; break;
		case 2: return PINC; break;
		case 3: return PIND; break;
	}
}
