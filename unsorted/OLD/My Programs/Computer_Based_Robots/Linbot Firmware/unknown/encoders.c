#include "encoders.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void initENC (void) {
  cntL = cntR = dirL = dirR = 0;
  MCUCR = MCUCR + (1 << ISC10) | (1 << ISC00);
  GICR = GICR + (1 << INT0) | (1 << INT1);
  sei();
}

void setDirL (int8_t mode) {
  dirL = mode;
}

void setDirR (int8_t mode) {
  dirR = mode;
}

int64_t getDisL (void) {
  return cntL;
}

int64_t getDisR (void) {
  return cntR;
}

SIGNAL (SIG_INTERRUPT0) {
  cntL = cntL + dirL;
}

SIGNAL (SIG_INTERRUPT1) {
  cntR = cntR + dirR;
}