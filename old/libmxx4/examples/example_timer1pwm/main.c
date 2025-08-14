#include <avr/io.h>
#include <util/delay.h>
#include <timer1pwm.h>

int main ()
{
    // Initalize timer1 in 10 bit PWM mode
    init_timer1_pwm(T1_PRESCALER_64, T1_PWM_10BIT, 0);
    
    // Initalize PortB low nib to all outputs, set forward
    DDRB = 0x0F;
    PORTB = 0x0A;
    
    // Variables
    uint8_t  direction = 0;
    uint16_t counter = 0;
    
    // Loop forever
    while (1)
    {
        // Ramp up for 1 second
        for(counter = 0; counter < T1_PWM_10BIT; counter++)
        {
            set_pwm_1a(counter);
            set_pwm_1b(counter);
            _delay_ms(2);
        }
        
        // Ramp down for 1 second
        for(counter = T1_PWM_10BIT - 1; counter > 0; counter--)
        {
            set_pwm_1a(counter);
            set_pwm_1b(counter);
            _delay_ms(2);
        }
        
        // Set PORTB for forward
        if(direction)
        {
            direction = 0;
            PORTB = 0x0A;
        } else
        {
            direction = 1;
            PORTB = 0x05;
        }
    }
    
    return 0;
}