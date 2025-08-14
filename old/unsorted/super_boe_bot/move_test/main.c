#include <util/delay.h>

#include <a2d.h>
#include <timer1servo.h>

unsigned char calcgp2d12( unsigned short val ) {
    if( val < 65 ) return 255;
	return (6787 / (val - 3)) - 4;
}

unsigned char calcgp2d120( unsigned short val) {
    if( val < 65 ) return 255;
    return (2914 / (val + 5)) - 1;
}

int main ( void ) {
    // Initalize the a2d converter and the servo controller
    initADC();
    init_timer1_servos();

    // Forever run the servos
    while (1) {
        unsigned short dist = calcgp2d12(getADC(0));
        if(dist > 15) {
            // If the distance is greater than 15 centimeters, move forward
            set_servo_1a(1000);
            set_servo_1b(2000);
        } else {
            // If not, we have found an obstical!  Reverse and turn....
            set_servo_1a(2000);
            set_servo_1b(1000);
            _delay_ms(1500);

            // Spin for 1 second
            set_servo_1a(2000);
            set_servo_1b(2000);
            _delay_ms(750);
        }
    }
    return 0;
}

