//Master code
#define F_CPU 11059200UL
#include <util/delay.h>
#include <avr/io.h>
#include "avrstdio.h"
#include <stdint.h>
#include <string.h>

int main() {
  char rechar[100];int x;
  DDRB=0;
  enable_uart();
  uprintf("ONLINE\r");
  for(x=0;x<100;x++){_delay_loop_2(65000);}
  while(1) {
    uprintf(">>0,1:PING;FART;\r");
	for(x=0;x<20;x++){_delay_loop_2(65000);}
    uprintf(">>0,1:SHUTDOWN;GOAWAY;\r");
	for(x=0;x<20;x++){_delay_loop_2(65000);}
  }
}