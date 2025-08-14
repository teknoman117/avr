#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>

int main () {
  char *name;
  UBRRH = 0x00;
  UBRRL = 25;
  UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);	
  UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
  sei();
  fdevopen(def_putc, NULL);
  printf("What is your name? ");
  scanf("%s",&name);
  printf("nice name %s!",name);
  return 0;
}