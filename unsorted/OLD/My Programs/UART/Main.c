#define F_CPU 11059200UL
#include <util/delay.h>
#include <avr/io.h>
#include "avrstdio.h"
#include <stdint.h>
#include <string.h>

int main() {
  char rechar[100];int x;
  DDRB = 0xFF;
  enable_uart();
  while(1) {
	/*uscanf(rechar,95,ON);
	if(!(strncmp(rechar,"s1",2))) {PORTB=0xFF;uprintf("light on!\r");}
	if(!(strncmp(rechar,"s2",2))) {PORTB=0x00;uprintf("light off!\r");}
	strcpy(rechar,"");*/
	uprintf("hi person!");
	for(x=0;x<10;x++){_delay_loop_2(65000);}
	uprintf("you *****!");
	for(x=0;x<10;x++){_delay_loop_2(65000);}
  }
}
    
	