#include <avr/io.h>
#include <stdint.h>
#include "M8io.h"
#include "adc.h"
#include "timer1PWM.h"

void DELAY_MS( uint16_t millisecnds );

int main() {
  initPWM();
  initADC();
  setIOD(1,O);
  setIOD(2,O);
  while (1) {
    setStateD(1,1);
	DELAY_MS(2);
	setCHA(ADC08bit(4));
	setStateD(1,0);
	DELAY_MS(200);
	setStateD(2,1);
	DELAY_MS(2);
	setCHB(ADC08bit(4));
	setStateD(2,0);
	DELAY_MS(200);
  }
  return 0;
}

void DELAY_MS( uint16_t milliseconds )
{
    uint16_t i;

    for ( i = 0; i < milliseconds; ++i )
    {
        DELAY_US( 1000 );
    }
}
  
  
  
  
  
  