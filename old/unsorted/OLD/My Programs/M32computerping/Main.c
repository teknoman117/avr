#define F_CPU 16000000UL
#include "avrstdio.h"
#include "M32io.h"
#include <avr/delay.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
int cnt;
int main() {
  char string[20];
  int c;
  initIO();
  enable_uart();
  setIOB(0,I);
  setIOD(5,O);
  setIOD(4,O);
  c = 0;
  while (1) {
      /*if(getStateB(0) == 0) {
	    uprintf("if you can read this, PING\r");
	    while(getStateB(0) == 0);
		for(cnt=0;cnt<200;cnt++){_delay_loop_2(30000);}
	  }*/
	  if(c == 200){c = 0;}
	  else {c++;}
	  itoa(c,string,10);
	  strcat(string,"\r");
	  uprintf(string);
	  for(cnt=0;cnt<200;cnt++){_delay_loop_2(30000);}
  }
}