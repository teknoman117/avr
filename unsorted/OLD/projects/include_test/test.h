#include <avr/io.h>

void init ();
void ledON();
void ledOFF();

void init () {DDRB = 0xFF;}
void ledON () {PORTB = 1;}
void ledOFF () {PORTB = 0;}

