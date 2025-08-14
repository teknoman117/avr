#include <avr/io.h>
#include <stdint.h>

//this is targeted for 40 pin avrs

/** groups
group 0: Porta pins 0-3
group 1: Porta pins 4-7
group 2: Portb pins 0-3
group 3: Portb pins 4-7
group 4: Portc pins 0-3
group 5: Portc pins 4-7
group 6: Portd pins 0-3
group 7: Portd pins 4-7
**/

class oDio4 {
	uint8_t state;
	uint8_t dir;
	uint8_t grp;
	uint8_t prt;
	uint8_t hl;
public:
    oDio4();
    oDio4(uint8_t group, uint8_t direction);
    void setGD(uint8_t group, uint8_t direction);
    void set (uint8_t value);
    uint8_t get ();
};

oDio4::oDio4 () {state = dir = grp = prt = hl = 0;}
oDio4::oDio4 (uint8_t group, uint8_t direction) {
	grp = group;
	dir = direction;
	switch (grp)
	{
		case 0: prt=0; hl = 0; break;
		case 1: prt=0; hl = 1; break;
		case 2: prt=1; hl = 0; break;
		case 3: prt=1; hl = 1; break;
		case 4: prt=2; hl = 0; break;
		case 5: prt=2; hl = 1; break;
		case 6: prt=3; hl = 0; break;
		case 7: prt=3; hl = 1; break;
	}
	if(dir) {
		switch (grp)
		{
			case 0: DDRA = (0x0F + (DDRA & 0xF0)); break;
			case 1: DDRA = (0xF0 + (DDRA & 0x0F)); break;
			case 2: DDRB = (0x0F + (DDRB & 0xF0)); break;
			case 3: DDRB = (0xF0 + (DDRB & 0x0F)); break;
			case 4: DDRC = (0x0F + (DDRC & 0xF0)); break;
			case 5: DDRC = (0xF0 + (DDRC & 0x0F)); break;
			case 6: DDRD = (0x0F + (DDRD & 0xF0)); break;
			case 7: DDRD = (0xF0 + (DDRD & 0x0F)); break;
		}
	}
	else {
		switch (grp)
		{
			case 0: DDRA = DDRA & 0xF0; break;
			case 1: DDRA = DDRA & 0x0F; break;
			case 2: DDRB = DDRB & 0xF0; break;
			case 3: DDRB = DDRB & 0x0F; break;
			case 4: DDRC = DDRC & 0xF0; break;
			case 5: DDRC = DDRC & 0x0F; break;
			case 6: DDRD = DDRD & 0xF0; break;
			case 7: DDRD = DDRD & 0x0F; break;
		}
	}
}
void oDio4::setGD (uint8_t group, uint8_t direction) {
	grp = group;
	dir = direction;
	switch (grp)
	{
		case 0: prt=0; hl = 0; break;
		case 1: prt=0; hl = 1; break;
		case 2: prt=1; hl = 0; break;
		case 3: prt=1; hl = 1; break;
		case 4: prt=2; hl = 0; break;
		case 5: prt=2; hl = 1; break;
		case 6: prt=3; hl = 0; break;
		case 7: prt=3; hl = 1; break;
	}
	if(dir) {
		switch (grp)
		{
			case 0: DDRA = (0x0F + (DDRA & 0xF0)); break;
			case 1: DDRA = (0xF0 + (DDRA & 0x0F)); break;
			case 2: DDRB = (0x0F + (DDRB & 0xF0)); break;
			case 3: DDRB = (0xF0 + (DDRB & 0x0F)); break;
			case 4: DDRC = (0x0F + (DDRC & 0xF0)); break;
			case 5: DDRC = (0xF0 + (DDRC & 0x0F)); break;
			case 6: DDRD = (0x0F + (DDRD & 0xF0)); break;
			case 7: DDRD = (0xF0 + (DDRD & 0x0F)); break;
		}
	}
	else {
		switch (grp)
		{
			case 0: DDRA = DDRA & 0xF0; break;
			case 1: DDRA = DDRA & 0x0F; break;
			case 2: DDRB = DDRB & 0xF0; break;
			case 3: DDRB = DDRB & 0x0F; break;
			case 4: DDRC = DDRC & 0xF0; break;
			case 5: DDRC = DDRC & 0x0F; break;
			case 6: DDRD = DDRD & 0xF0; break;
			case 7: DDRD = DDRD & 0x0F; break;
		}
	}
}

void oDio4::set (uint8_t value) {
	if(dir) {
		state = value;
		switch (grp)
		{
			case 0: PORTA = (value + (PORTA & 0xF0)); break; 
			case 1: PORTA = ((value << 4) + (PORTA & 0x0F)); break;
			case 2: PORTB = (value + (PORTB & 0xF0)); break;
			case 3: PORTB = ((value << 4) + (PORTB & 0x0F)); break;
			case 4: PORTC = (value + (PORTC & 0xF0)); break;
			case 5: PORTC = ((value << 4) + (PORTC & 0x0F)); break;
			case 6: PORTD = (value + (PORTD & 0xF0)); break;
			case 7: PORTD = ((value << 4) + (PORTD & 0x0F)); break;
		}
	}
}

uint8_t oDio4::get () {
    switch (grp)
    {
    	case 0: return PINA & 0x0F; break;
    	case 1: return (PINA >> 4) & 0x0F; break; 
    	case 2: return PINB & 0x0F; break;
    	case 3: return (PINB >> 4) & 0x0F; break; 
    	case 4: return PINC & 0x0F; break;
    	case 5: return (PINC >> 4) & 0x0F; break; 
    	case 6: return PIND & 0x0F; break;
    	case 7: return (PIND >> 4) & 0x0F; break; 
    }
}
    	
		

		
	
	
	 
