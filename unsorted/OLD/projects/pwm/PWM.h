//this is optimized for the ATmega644
//timer 0 and 2 only support 8 bit PWM
//channel A is represented by 1 and channel B is represented by 2
#include <avr/io.h>
#include <stdint.h>

class PWM {
	uint8_t timer;
	uint8_t channel;
	void initT0();
	void initT1();
	void initT2();
public:
    PWM();
    PWM(uint8_t n);
    void setTC(uint8_t n);
    void setvalue(uint8_t value);
};

PWM::PWM() {}
PWM::PWM(uint8_t n) {
    if((n == 0) || (n == 1)) {timer = 0;}
    if((n == 2) || (n == 3)) {timer = 1;}
    if((n == 4) || (n == 5)) {timer = 2;}
    if((n == 0) || (n == 2) || (n == 4)) {channel= 1;}
    else {channel = 2;}
	switch (timer) {
		case 0: initT0(); break;
		case 1: initT1(); break;
		case 2: initT2(); break;
	}
}	
void PWM::setTC(uint8_t n) {
    if((n == 0) || (n == 1)) {timer = 0;}
    if((n == 2) || (n == 3)) {timer = 1;}
    if((n == 4) || (n == 5)) {timer = 2;}
    if((n == 0) || (n == 2) || (n == 4)) {channel= 1;}
    else {channel = 2;}
    switch (timer) {
		case 0: initT0(); break;
		case 1: initT1(); break;
		case 2: initT2(); break;
	}
}

void PWM::setvalue (uint8_t value) {
	switch (timer) {
		case 0: 
		  if(channel == 1) {OCR0A = value;}
		  if(channel == 2) {OCR0B = value;}
	      break;
	    case 1:
	      if(channel == 1) {OCR1A = value;}
	      if(channel == 2) {OCR1B = value;} 
	      break;
	    case 2:
		  if(channel == 1) {OCR2A = value;}
		  if(channel == 2) {OCR2B = value;}
	      break;
	}
}

void PWM::initT0 () {
    TCCR0A = (1<<WGM00) | (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01);
    TCCR0B = (1<<CS00);
}

void PWM::initT1 () {
    TCCR1A = (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1); 
    TCCR1B = (1<<CS10) | (1<<WGM12);		
}

void PWM::initT2 () {
    TCCR0A = (1<<WGM20) | (1<<COM2A1) | (1<<COM2B1) | (1<<WGM21);
    TCCR0B = (1<<CS20);
}
