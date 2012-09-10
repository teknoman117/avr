#include "oDio1.h"
#include <stdint.h>

class oLED2 {
	uint8_t state;
	uint8_t IOLine1, IOLine2;
	oDio1 line1, line2;
public:
    oLED2();
    oLED2(uint8_t cath1, uint8_t cath2);
    void setIOLS(uint8_t cath1, uint8_t cath2);
    void off();   //shuts it off
    void on();    //turns both sides on
    void on1();   //turns side 1 on
    void on2();   //turns side 2 on
};

oLED2::oLED2() {
	IOLine1 = IOLine2 = state = 0;
}

oLED2::oLED2(uint8_t cath1, uint8_t cath2) {
	IOLine1 = cath1;
	IOLine2 = cath2;
	state = 0;
	line1.setPD(IOLine1, 1);
	line2.setPD(IOLine2, 1);
}

void oLED2::setIOLS(uint8_t cath1, uint8_t cath2) {
	IOLine1 = cath1;
	IOLine2 = cath2;
	state = 0;
	line1.setPD(IOLine1, 1);
	line2.setPD(IOLine2, 1);
}

void oLED2::off() {
	line1.set(0);
	line2.set(0);
}

void oLED2::on() {
	line1.set(1);
	line2.set(1);
}

void oLED2::on1() {
	line1.set(1);
	line2.set(0);
}

void oLED2::on2() {
	line1.set(0);
	line2.set(1);
}
