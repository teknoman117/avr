/*slave code */
#define F_CPU 11059200UL
#include <util/delay.h>
#include <avr/io.h>
#include "avrstdio.h"
#include <stdint.h>
#include <string.h>

int main() {
  char rechar[100];int x;
  DDRD = DDRC = DDRB = 0xFF;
  enable_uart();
  while(1) {
	uscanf(rechar,95,ON);
	if(!(strncmp(rechar,"s1",2))) {PORTB=PORTC=0xFF;}
	if(!(strncmp(rechar,"s2",2))) {PORTB=PORTC=0x00;}
	strcpy(rechar,"");
  }
}