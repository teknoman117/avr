#define F_CPU 11059200UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "avr40pio.h"
#include "masterNbus.h"

void init_nBus (void) {
  setIOB(0,0);
  setIOB(1,0);
  setIOB(2,0);
  setDIRB(0,1);
  setDIRB(1,1);
  setDIRB(2,1);
}

void tx_nbyte (unsigned char daByte) {
  setIOB(0,1);
  setIOB(1,0);
  for(cnt=0;cnt<8;cnt++) {
    setIOB(2,((daByte>>cnt)&1));
    setIOB(1,1);
    _delay_loop_2(1000);
    setIOB(1,0);
  }
  setIOB(0,0); 
  setIOB(2,0);
  setIOB(1,0);
}

void tx_nword (unsigned int daWord) {
  setIOB(0,1);
  setIOB(1,0);
  for(cnt=0;cnt<16;cnt++) {
    setIOB(2,((daWord>>cnt)&1));
    setIOB(1,1);
    _delay_loop_2(1000);
    setIOB(1,0);
  }
  setIOB(0,0); 
  setIOB(2,0);
  setIOB(1,0);
}

unsigned char rx_nbyte (unsigned char daByte) {
  unsigned char bufchar=0;
  setDIRB(2,0);
  setIOB(0,1);
  setIOB(1,0);
  for(cnt=0;cnt<8;cnt++) {
	_delay_loop_2(333);
    setIOB(1,1);
    _delay_loop_2(334);
	bufchar+=(getIOB(2)<<cnt);
	_delay_loop_2(333);
    setIOB(1,0);
  }
  setIOB(0,0); 
  daByte=bufchar;
  return bufchar;
} 

unsigned int rx_nword (unsigned int daWord) {
  unsigned int bufword=0;
  setDIRB(2,0);
  setIOB(0,1);
  setIOB(1,0);
  for(cnt=0;cnt<16;cnt++) {
	_delay_loop_2(333);
    setIOB(1,1);
    _delay_loop_2(334);
	bufword+=(getIOB(2)<<cnt);
	_delay_loop_2(333);
    setIOB(1,0);
  }
  setIOB(0,0); 
  daWord=bufword;
  return bufword;
}
 
  