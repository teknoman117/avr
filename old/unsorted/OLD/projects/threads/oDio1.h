#include <avr/io.h>
#include <stdint.h>
//targeted for 40 pin AVRs

/**List of pins
--porta-----portb-----portc-----portd---
pa0: 0   |pb0: 8   |pc0: 16  |pd0: 24  |
pa1: 1   |pb1: 9   |pc0: 17  |pd0: 25  | 
pa2: 2   |pb2: 10  |pc0: 18  |pd0: 26  |
pa3: 3   |pb3: 11  |pc0: 19  |pd0: 27  |
pa4: 4   |pb4: 12  |pc0: 20  |pd0: 28  |
pa5: 5   |pb5: 13  |pc0: 21  |pd0: 29  |
pa6: 6   |pb6: 14  |pc0: 22  |pd0: 30  |
pa7: 7   |pb7: 15  |pc0: 23  |pd0: 31  |
----------------------------------------
**/

class oDio1 {
    uint8_t state;
    uint8_t dir;  //direction 1=output, 0=input
    uint8_t pn;
    uint8_t prt;
    uint8_t ppn;
public:
    oDio1();
    oDio1(uint8_t pin, uint8_t direction);   
    void setPD(uint8_t pin, uint8_t direction);
    void set(uint8_t ste);
    uint8_t get();
};

oDio1::oDio1 () {
	state = 0;
	dir = 0;
	pn = 0;
	prt = 0;
	ppn = 0;
}

oDio1::oDio1 (uint8_t pin, uint8_t direction) {
	pn = pin;
	dir = direction;
	if(pin < 8) {
		prt=0;
		ppn = pin;
	}
	if((pin > 7) && (pin < 16)) {
		prt = 1;
		ppn = pin - 8;
	}
	if((pin > 15) && (pin < 24)) {
		prt = 2;
        ppn = pin - 16;
	}
	if(pin > 23) {
		prt = 3;
		ppn = pin - 24;
	}
    if(dir) {
    	switch (prt) {
      		case 0:if(!((DDRA >> ppn) & 1)) {DDRA += (1 << ppn);} break;
      		case 1:if(!((DDRB >> ppn) & 1)) {DDRB += (1 << ppn);} break;
      		case 2:if(!((DDRC >> ppn) & 1)) {DDRC += (1 << ppn);} break;
      		case 3:if(!((DDRD >> ppn) & 1)) {DDRD += (1 << ppn);} break;
    	}
    	switch (prt) {
      		case 0:if((PORTA >> ppn) & 1) {PORTA -= (1 << ppn);} break;
      		case 1:if((PORTB >> ppn) & 1) {PORTB -= (1 << ppn);} break;
      		case 2:if((PORTC >> ppn) & 1) {PORTC -= (1 << ppn);} break;
      		case 3:if((PORTD >> ppn) & 1) {PORTD -= (1 << ppn);} break;
    	}
    	state = 0;
    }	
}

void oDio1::setPD (uint8_t pin, uint8_t direction) {
	pn = pin;
	dir = direction;
	if(pin < 8) {
		prt=0;
		ppn = pin;
	}
	if((pin > 7) && (pin < 16)) {
		prt = 1;
		ppn = pin - 8;
	}
	if((pin > 15) && (pin < 24)) {
		prt = 2;
        ppn = pin - 16;
	}
	if(pin > 23) {
		prt = 3;
		ppn = pin - 24;
	}
    if(dir) {
    	switch (prt) {
      		case 0:if(!((DDRA >> ppn) & 1)) {DDRA += (1 << ppn);} break;
      		case 1:if(!((DDRB >> ppn) & 1)) {DDRB += (1 << ppn);} break;
      		case 2:if(!((DDRC >> ppn) & 1)) {DDRC += (1 << ppn);} break;
      		case 3:if(!((DDRD >> ppn) & 1)) {DDRD += (1 << ppn);} break;
    	}
    	switch (prt) {
      		case 0:if((PORTA >> ppn) & 1) {PORTA -= (1 << ppn);} break;
      		case 1:if((PORTB >> ppn) & 1) {PORTB -= (1 << ppn);} break;
      		case 2:if((PORTC >> ppn) & 1) {PORTC -= (1 << ppn);} break;
      		case 3:if((PORTD >> ppn) & 1) {PORTD -= (1 << ppn);} break;
    	}
    	state = 0;
    }	
}

void oDio1::set (uint8_t ste) {
	if (dir) {
		state = ste;
		if(!(ste)) {
            switch (prt)
            {
      		    case 0:if((PORTA >> ppn) & 1) {PORTA -= (1 << ppn);} break;
      		    case 1:if((PORTB >> ppn) & 1) {PORTB -= (1 << ppn);} break;
      		    case 2:if((PORTC >> ppn) & 1) {PORTC -= (1 << ppn);} break;
      		    case 3:if((PORTD >> ppn) & 1) {PORTD -= (1 << ppn);} break;         
            }
		}
		if(ste) {
            switch (prt)
            {
      		    case 0:if(!((PORTA >> ppn) & 1)) {PORTA += (1 << ppn);} break;
      		    case 1:if(!((PORTB >> ppn) & 1)) {PORTB += (1 << ppn);} break;
      		    case 2:if(!((PORTC >> ppn) & 1)) {PORTC += (1 << ppn);} break;
      		    case 3:if(!((PORTD >> ppn) & 1)) {PORTD += (1 << ppn);} break;         
            }
		}
	}
}

uint8_t oDio1::get () {
	switch (prt)
	{
		case 0 : return ((PINA >> ppn) & 1); break;
		case 1 : return ((PINB >> ppn) & 1); break;
		case 2 : return ((PINC >> ppn) & 1); break;
		case 3 : return ((PIND >> ppn) & 1); break;
	}
}
