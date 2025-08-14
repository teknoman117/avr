#include <avr/io.h>
#include <stdint.h>

class A2D {
    uint8_t channel;
public:
    A2D();
    A2D(uint8_t ch);
    void changeCH(uint8_t ch);
    uint16_t getValue10();
    uint8_t getValue8();
};

A2D::A2D() {
	channel = 0;
}

A2D::A2D(uint8_t ch) {
	channel = ch;
	ADMUX = (1 << REFS0);
	ADCSRA = (6 << ADPS0);
	ADCSRA |= (1 << ADEN);	
}

void A2D::changeCH(uint8_t ch) {
	channel = ch;
	ADMUX = (1 << REFS0);
	ADCSRA = (6 << ADPS0);
	ADCSRA |= (1 << ADEN);	
}

uint16_t A2D::getValue10() {
	ADMUX &= ~0x1F;
	ADMUX |= channel;
	ADCSRA |= (1 << ADSC);
	while (bit_is_set(ADCSRA, ADSC));
	return (ADCL | (ADCH << 8));
}	

uint8_t A2D::getValue8() {	
	return((getValue10() >> 2) & 0xFF);
}    
