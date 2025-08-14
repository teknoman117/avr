#include "wait.h"
#include <stdint.h>
#include <util/delay.h>

void waitus (uint32_t __us) {
  for(x=0;x<__us;x++) {
    _delay_loop_1(MCU_FREQ / 998);
  }
}

void waitms (uint32_t __ms) {
  for(x=0;x<__ms;x++) {
    _delay_loop_2(MCU_FREQ / 1251);
  }
}
