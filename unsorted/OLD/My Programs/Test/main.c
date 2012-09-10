#include "avrstdio.h"
#include <string.h>
#include <stdint.h>

void f();
void b();
void l();
void r();
void sp();

char inputinfo[50];

int main () {
  enable_uart(25);
  uprintf("AVR started, ready to except commands\r");
  while (1) {
    uscanf(inputinfo,50,ON);
	uprintf("You Entered: ");uprintf(inputinfo);uprintf("\r");
	if(strncmp(inputinfo,"Forward",7) == 0) {f();}
	if(strncmp(inputinfo,"Backward",8) == 0) {b();}
	if(strncmp(inputinfo,"Left",4) == 0) {l();}
	if(strncmp(inputinfo,"Right",5) == 0) {r();}
	if(strncmp(inputinfo,"Stop",4) == 0) {sp();}
  }
}

void f() {uprintf("Command Excepted, Forward\r");}
void b() {uprintf("Command Excepted, Backward\r");}
void l() {uprintf("Command Excepted, Left\r");}
void r() {uprintf("Command Excepted, Right\r");}
void sp() {uprintf("Command Excepted, Stop\r");}
	