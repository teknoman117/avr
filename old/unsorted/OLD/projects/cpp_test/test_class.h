#include <avr/io.h>

class test_class {
public:
    test_class();
    void led_on();
    void led_off();
};

test_class::test_class () {DDRB = 0xFF;}
void test_class::led_on () {PORTB = 1;}
void test_class::led_off () {PORTB = 0;}
    
