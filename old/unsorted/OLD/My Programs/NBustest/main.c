#define F_CPU 11059200UL
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "avr40pio.h"
#include "masterNbus.h"

int main () {
  unsigned char count;
  init_nBus();
  while (1) {
    tx_nbyte(0xBD);
	for(count=0;count<50;count++){_delay_loop_2(30000);}
	tx_nword(0xBBB9);
	for(count=0;count<50;count++){_delay_loop_2(30000);}
  }
  return 0;
}