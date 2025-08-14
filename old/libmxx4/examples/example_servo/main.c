#include <stdio.h>

#include <timer1servo.h>
#include <stdusart0.h>

#define DEFAULT_POSITION 1500

int main ()
{
    // Initialize the servo stack
    init_timer1_servos();
    set_servo_1a(DEFAULT_POSITION);
    set_servo_1b(DEFAULT_POSITION);

    // Initialize the stdusart stack
    init_stdusart0(51, DB8 | P_N | SB1);

    // Forever loop
    while (1)
    {
        // Request a position in microseconds
        printf("Enter position in [us]: ");
       
        // Get that position
        unsigned int _pos = 0;
        scanf("%u", &_pos);
      
        // Set the positions
        set_servo_1a(_pos);
        set_servo_1b(_pos);   
    }

    // Return control to libgcc.a
    return 0;
}
