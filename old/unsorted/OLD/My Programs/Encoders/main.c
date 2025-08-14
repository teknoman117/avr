#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <stdint.h>
#include "M8io.h"
#include "timer1PWM.h"

uint16_t Lcnt;
uint16_t Rcnt;

int main() {
  initPWM();
  initIO();
  setIOB(1,O);
  setIOB(2,O);
  MCUCR = MCUCR + ((1<<ISC11) + (1<<ISC01));
  GICR = GICR + ((1<<INT0) + (1<<INT1)); 
  sei();
  while(1) {
    setCHA(Lcnt);
    setCHB(Rcnt);
  }
  return 0;
}  
    
SIGNAL(SIG_INTERRUPT0) {
  if(getStateB(0) == 1){Lcnt++;}
  if(getStateB(0) == 0){Lcnt--;}
  //setStateB(1,1);
}

SIGNAL(SIG_INTERRUPT1) {
  if(getStateB(5) == 1){Rcnt++;}
  if(getStateB(5) == 0){Rcnt--;}
  //setStateB(1,0);
}

  