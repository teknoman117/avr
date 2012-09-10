#include <stdint.h>
#include <avr/io.h>

/** groups
group 0: PortA & PortB
group 1: PortC & PortD
*/

class oDio16 {
	uint16_t state;
	uint8_t direction;
	uint8_t group;
public:
    oDio16();
    oDio16(uint8_t grp, uint8_t dir);
    void setGP(uint8_t grp, uint8_t dir);
    void set (uint16_t value);
    uint16_t get();
};

oDio16::oDio16() {state = direction = group = 0;}

oDio16::oDio16(uint8_t grp, uint8_t dir) {
	direction = dir;
	group = grp;
	state = 0;
	if(grp == 0) {
		if(dir) {DDRA = DDRB = 0xFF; PORTA = PORTB = 0x00;}
		else {DDRA = DDRB = 0x00;}
    }
	if(grp == 1) {
		if(dir) {DDRC = DDRD = 0xFF; PORTC = PORTD = 0x00;}
		else {DDRC = DDRD = 0x00;}
    }
}
	
void oDio16::setGP(uint8_t grp, uint8_t dir) {
	direction = dir;
	group = grp;
	state = 0;
	if(grp == 0) {
		if(dir) {DDRA = DDRB = 0xFF; PORTA = PORTB = 0x00;}
		else {DDRA = DDRB = 0x00;}
    }
	if(grp == 1) {
		if(dir) {DDRC = DDRD = 0xFF; PORTC = PORTD = 0x00;}
    	else {DDRC = DDRD = 0x00;}
    }
}	

void oDio16::set(uint16_t value) {
	state = value;
	if(group == 0) {PORTA = value & 0xFF; PORTB = (value >> 8) & 0xFF;}
    if(group == 1) {PORTC = value & 0xFF; PORTD = (value >> 8) & 0xFF;}	
}

uint16_t oDio16::get() {
	uint16_t ret;
	ret = 0;
	if(group == 0) {
        ret = PINB;
        ret = ret << 8;
        ret = ret + PINA;
	}
	if(group == 1) {
		ret = PIND;
		ret = ret << 8;
		ret = ret + PINC;
	}
	return ret;
}
