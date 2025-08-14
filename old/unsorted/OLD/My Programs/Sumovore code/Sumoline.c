/*
    Sumoline.c
    Copyright 2005 Solarbotics Ltd.
 
    Written by Bob Cook, based on the original by Grant McKee
 
    Designed for the Sumovore Atmel Mega8 Brainboard Add-on Kit.
    Intended to be compiled with WinAVR 20050214 tool chain.
 
    Solarbotics: http://www.solarbotics.com/
    WinAVR     : http://winavr.sourceforge.net/
 
 
    Description: Minisumo/Linefollower program Ver 1.1
 
    If outside edge sensors see black during startup the program will branch
    to sumo. If outside edge sensors see white during startup the program will
    branch to Linefollower.
 
    Sumo mode is fairly basic:
 
        - Wait 5 seconds before moving
        - After 5 seconds go straight forward
        - If a white line is detected on outside Left sensor
            * Reverse both motors briefly
            * Stop left motor
            * Reverse Right motor
            * After set time continue straight forward
        - If a white line is detected on outside Right sensor
            * Reverse both motors briefly
            * Stop Right motor
            * Reverse Left motor
            * After set time continue straight forward
        - If opponent detected on right Side
            * Stop Right motor
        - If opponent detected on Left Side
            * Stop Left motor
        - If both sensors detect opponent
           * Turn on both motors forward
 
    Linefollower mode:
 
        - Start immediately
        - If center sensor sees black all is well- go straight forward
        - If center right sensor sees black, make a gentle right turn by
          slowing down right motor
        - If center left sensor sees black, make a gentle left turn by slowing
          down left motor
        - If far right sensor sees black, make a very sharp right turn by
          reversing right motor
        - If far left sensor sees black, make a very sharp left turn by 
          reversing left motor
        - If all is white, all is lost - go looking for the line!
 
 */


#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>


/*
 *  Function prototypes
 *
 */
 
void     SUMO( void );
void     LINEFOLLOWER( void );
uint16_t ADCIN( uint8_t channel );
void     DELAY_US( uint16_t microseconds );
void     DELAY_MS( uint16_t milliseconds );


/*
 *  Definitions
 *
 */

/* White line sensitivity; higher is more sensitive (255 max) */
#define kWhiteThreshold 128
#define MCU_FREQ 16000000UL


/*
 *  main (program entry point)
 *
 */

int main( void )
{
    uint16_t    linesensor_left, linesensor_right;

    /* turn off all interrupts */

    cli();
    TIMSK = 0;

    /* the sumovore atmel brainboard has five red leds and one green led
       that we can use to display things in our program; we need to set
       the portb and portd data direction registers to enable output for 
       those pins */

    DDRB = ( 1 << PB3 );                     // PB3 = green led
  
    DDRD = ( 1 << PD2 ) | ( 1 << PD3 )       // PD2 - PD6 = red leds
         | ( 1 << PD4 ) | ( 1 << PD5 )
         | ( 1 << PD6 );

    /* the sumovore has two motors that also require output pins */

    DDRB |= ( 1 << PB1 ) | ( 1 << PB2 )
         | ( 1 << PB4 ) | ( 1 << PB5 );

    /* enable the ADC and start the first (dummy) conversion to complete 
       the initialization */

    ADCSR = ( 1 << ADEN  ) | ( 1 << ADSC  )
          | ( 1 << ADPS2 ) | ( 1 << ADPS1 );

    while ( ADCSR & ( 1 << ADSC ) ); /* wait for complete conversion */

    /* read the line sensors to decide whether we should behave like a
       sumo wrestler or like a line chaser */

    linesensor_left  = ADCIN( 0 );
    linesensor_right = ADCIN( 4 );

    if ( ( linesensor_left  < kWhiteThreshold ) &&
         ( linesensor_right < kWhiteThreshold )  )
    {
        /* both line sensors see white */

        LINEFOLLOWER();
    }
    else
    {
        /* one or both line sensors see black */

        SUMO();
    }

    /* this point of the program is never reached, but we satisfy the
       compiler's warnings by saying we'd return zero */

    return 0;
}


/*
 *  SUMO
 *
 *  Find and push our opponent out of the ring!
 *
 */

void SUMO()
{
    /* sumo mode turns the green led off */

    PORTB &= ~( 1 << PB3 );

    /* start with a five second countdown */

    PORTD |= ( 1 << PD2 ); /* turn on L1 */
    DELAY_MS( 1000 );      /* 1000 milliseconds = 1 second */

    PORTD |= ( 1 << PD3 ); /* turn on L2 */
    DELAY_MS( 1000 );

    PORTD |= ( 1 << PD4 ); /* turn on L3 */
    DELAY_MS( 1000 );

    PORTD |= ( 1 << PD5 ); /* turn on L4 */
    DELAY_MS( 1000 );

    PORTD |= ( 1 << PD6 ); /* turn on L5 */
    DELAY_MS( 1000 );

    /* match begins! turn off all the leds */

    PORTD &= ~( ( 1 << PD2 )
              | ( 1 << PD3 )
              | ( 1 << PD4 )
              | ( 1 << PD5 )
              | ( 1 << PD6 ) );

    /* turn on both motors going full forward */

    PORTB |= ( ( 1 << PB1 )     /* left  motor enable  */
             | ( 1 << PB2 )     /* right motor enable  */
             | ( 1 << PB4 )     /* left  motor forward */
             | ( 1 << PB5 ) );  /* right motor forward */

    /* loop here forever, chasing our opponent out of the ring */

    for ( ;; )
    {
        uint16_t    linesensor_left, linesensor_right, irsensors;

        /* read the line sensors to see if we've hit the ring edge */

        linesensor_left  = ADCIN( 0 );
        linesensor_right = ADCIN( 4 );

        if ( linesensor_right  < kWhiteThreshold )
        {
            /* white ring edge on right side; reverse both motors to prevent
               sliding over the edge */

            PORTB &= ~( ( 1 << PB4 ) | ( 1 << PB5 ) );
            DELAY_MS( 40 );

            /* now turn around by stopping the right motor */

            PORTB &= ~( 1 << PB2 );
            DELAY_MS( 400 );
        }

        if ( linesensor_left  < kWhiteThreshold )
        {
            /* white ring edge on left side; reverse both motors to prevent
               sliding over the edge */

            PORTB &= ~( ( 1 << PB4 ) | ( 1 << PB5 ) );
            DELAY_MS( 40 );

            /* now turn around by stopping the left motor */

            PORTB &= ~( 1 << PB1 );
            DELAY_MS( 400 );
        }

        /* read the forward-looking sensors looking for our opponent */

        irsensors = PIND & ( ( 1 << PD1 ) | ( 1 << PD0 ) ); /* just PD0, PD1 */

        switch ( irsensors )
        {
            case 0:
            {
                /* both sensors see something; full steam ahead! */

                PORTD |= ( ( 1 << 2 )       /* turn on L1 */
                         | ( 1 << 6 ) );    /* turn on L5 */

                PORTB |= ( ( 1 << PB1 )     /* left  motor enable  */
                         | ( 1 << PB2 )     /* right motor enable  */
                         | ( 1 << PB4 )     /* left  motor forward */
                         | ( 1 << PB5 ) );  /* right motor forward */
                break;
            }

            case 1:
            {
                /* opponent in the right sensor */

                PORTD &= ~( 1 << 2 );       /* turn off L1 */
                PORTD |=  ( 1 << 6 );       /* turn on  L5 */

                PORTB &= ~( 1 << PB2 );     /* right motor disable */

                PORTB |= ( ( 1 << PB1 )     /* right motor enable  */
                         | ( 1 << PB4 ) );  /* right motor forward */
                break;
            }

            case 2:
            {
                /* opponent in the left sensor */

                PORTD &= ~( 1 << 6 );       /* turn off L5 */
                PORTD |=  ( 1 << 2 );       /* turn on  L1 */

                PORTB &= ~( 1 << PB1 );     /* left  motor disable */

                PORTB |= ( ( 1 << PB2 )     /* right motor enable  */
                         | ( 1 << PB5 ) );  /* right motor forward */
                break;
            }

            case 3:
            {
                /* neither sensor sees anything */

                PORTD &= ~( ( 1 << 2 )      /* turn off L1 */
                          | ( 1 << 6 ) );   /* turn off L5 */

                PORTB |= ( ( 1 << PB1 )     /* left  motor enable  */
                         | ( 1 << PB2 )     /* right motor enable  */
                         | ( 1 << PB4 )     /* left  motor forward */
                         | ( 1 << PB5 ) );  /* right motor forward */
                break;
            }
        }

        /* loop back to check sensors again */
    }
    /* NEVER REACHED */
}


/*
 *  LINEFOLLOWER
 *
 *  Chase the black line as fast as we can!
 *
 */

void LINEFOLLOWER()
{
    /* linefollower mode turns the green led on */

    PORTB |= ( 1 << PB3 );

    /* set up Timer1 to provide PWM output to drive the motors */

    TCCR1A = ( 1 << WGM10 ) | ( 1 << COM1A1 ) | ( 1 << COM1B1 );
    TCCR1B = ( 1 << CS10  ) | ( 1 << WGM12  );

    /* loop here forever, following the black line */

    for ( ;; )
    {
        uint16_t    linesensor_left;
        uint16_t    linesensor_midleft;
        uint16_t    linesensor_center;
        uint16_t    linesensor_midright;
        uint16_t    linesensor_right;

        /* read the current sensor values */

        linesensor_left     = ADCIN( 0 );
        linesensor_midleft  = ADCIN( 1 );
        linesensor_center   = ADCIN( 2 );
        linesensor_midright = ADCIN( 3 );
        linesensor_right    = ADCIN( 4 );

        /* check the outer right sensor */

        if ( linesensor_right < kWhiteThreshold )
        {
            /* seeing white on this sensor */

            PORTD |= ( 1 << PD6 );  /* turn on L5 */
        }
        else
        {
            /* seeing black on this sensor */

            PORTD &= ~( 1 << PD6 ); /* turn off L5 */
            PORTB &= ~( 1 << PB5 ); /* left motor reverse */
            DELAY_US( 1300 );
        }

        /* check the middle-right sensor */

        if ( linesensor_midright < kWhiteThreshold )
        {
            /* seeing white on this sensor */

            PORTD |= ( 1 << PD5 );  /* turn on L4 */
        }
        else
        {
            /* seeing black on this sensor */

            PORTD &= ~( 1 << PD5 ); /* turn off L4 */
            OCR1B = 127;            /* slow down right motor */
            DELAY_US( 1300 );
        }

        /* check the center sensor */

        if ( linesensor_center < kWhiteThreshold )
        {
            /* seeing white on this sensor */

            PORTD |= ( 1 << PD4 );  /* turn on L3 */
        }
        else
        {
            /* seeing black on this sensor */

            PORTD &= ~( 1 << PD4 ); /* turn off L3 */

            /* both motors forward at max speed */

            PORTB |= ( 1 << PB4 ) | ( 1 << PB5 );
            OCR1B  = 255;
            OCR1A  = 255;
        }

        /* check the left-middle sensor */

        if ( linesensor_midleft < kWhiteThreshold )
        {
            /* seeing white on this sensor */

            PORTD |= ( 1 << PD3 );  /* turn on L2 */
        }
        else
        {
            /* seeing black on this sensor */

            PORTD &= ~( 1 << PD3 ); /* turn off L2 */
            OCR1A = 127;            /* slow down left motor */
            DELAY_US( 1300 );
        }

        /* check the outer left sensor */

        if ( linesensor_left < kWhiteThreshold )
        {
            /* seeing white on this sensor */

            PORTD |= ( 1 << PD2 );  /* turn on L1 */
        }
        else
        {
            /* seeing black on this sensor */

            PORTD &= ~( 1 << PD2 ); /* turn off L1 */
            PORTB &= ~( 1 << PB4 ); /* left motor reverse */
            DELAY_US( 1300 );
        }

        /* loop back to check sensors again */
    }
    /* NEVER REACHED */
}
    

/*
 *  ADCIN
 *
 *  Read the specified Analog to Digital Conversion channel
 *
 */

uint16_t ADCIN( uint8_t channel )
{
    /* set for 8-bit results for the desired channel number then start the
       conversion; pause for the hardware to catch up */
    
    ADMUX  = ( 1 << ADLAR ) | ( 1 << REFS0 ) | channel;
    ADCSRA = ( 1 << ADEN  ) | ( 1 << ADSC  );
    DELAY_US( 64 );

    /* wait for complete conversion and return the result */

    while ( ADCSR & ( 1 << ADSC ) );
    
    return ADCH;
}


/*
 *  DELAY_US
 *
 *  Spin-wait in a loop for the specified number of microseconds.
 *
 */

void DELAY_US( uint16_t microseconds )
{
    register uint16_t loop_count;

#if MCU_FREQ == 8000000UL

    /* 8mhz clock, 4 instructions per loop_count  */
    loop_count = microseconds * 2;

#elif MCU_FREQ == 1000000UL

    /* 1mhz clock, 4 instructions per loop_count */
    loop_count = microseconds / 4;

#elif MCU_FREQ == 16000000UL

    /* 1mhz clock, 4 instructions per loop_count */
    loop_count = microseconds / 4;

#else
#error MCU_FREQ undefined or set to an unknown value!
    loop_count = 0; /* don't really know what to do */
#endif

    __asm__ volatile (
        "1: sbiw %0,1" "\n\t"
        "brne 1b"
        : "=w" ( loop_count )
        : "0"  ( loop_count )
    );
}


/*
 *  DELAY_MS
 *
 *  Spin-wait in a loop for the specified number of milliseconds.
 *
 */

void DELAY_MS( uint16_t milliseconds )
{
    uint16_t i;

    for ( i = 0; i < milliseconds; ++i )
    {
        DELAY_US( 1000 );
    }
}


/************************************ END ***************************************/
