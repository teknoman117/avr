#include "oDio1.h"
#include <stdint.h>

class oLED {
	uint8_t IOLine;
	uint8_t onoff;
	oDio1 tl;
public:
    oLED();
    oLED(uint8_t ioline);
    void setIOL(uint8_t ioline);
    void on();
    void off();
    void toggle();
    uint8_t isOn();
    uint8_t isOff();
};

oLED::oLED() {IOLine = onoff = 0;}
oLED::oLED(uint8_t ioline) {
	IOLine = ioline;
	onoff = 0;
	tl.setPD(IOLine, 1);
}

void oLED::setIOL(uint8_t ioline) {
	IOLine = ioline;
	onoff = 0;
	tl.setPD(IOLine, 1);
}

void oLED::on() {
	onoff = 1;
	tl.set(1);
}

void oLED::off() {
	onoff = 0;
	tl.set(0);
}
	
uint8_t oLED::isOn () {return (onoff == 1);}
uint8_t oLED::isOff () {return (onoff == 0);}
