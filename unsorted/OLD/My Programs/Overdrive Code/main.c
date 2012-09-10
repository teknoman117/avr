#include <avr/io.h>
#include <stdint.h>
#include "motor.h"

void main () {
  initMotors();
  double y;
  while (1) {
    forward(255);
	for(y=0;y<100000;y++);
	sleft(128);
	for(y=0;y<1000;y++);
	sright(128);
	for(y=0;y<1000;y++);
	backward(255);
	for(y=0;y<100000;y++);
  }
}