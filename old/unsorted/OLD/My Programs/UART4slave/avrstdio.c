#include <avr/io.h>
#include <string.h>
#include "avrstdio.h"

void enable_uart(void) {
  UBRR0H = 0x00;
  UBRR0L = 5;
  UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);	
  UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void txb(unsigned char dat) {
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = dat;
}

void uprintf(unsigned char data[]) {
	for(x=0;x<strlen(data);x++) {
	  txb(data[x]);
	}
}

unsigned char rxb(void) {
  while( !(UCSR0A & (1<<RXC0)));
  s = UDR0;
  return s;
}

unsigned char *uscanf(char retstr[],uint8_t elements,uint8_t onoff) {
  unsigned char str[50];
  strcpy(str,"");
  switch (onoff) {
   case ON:
    for(x=0;x<elements;x++) {
	  str[x] = rxb();
	  if(str[x] == '\r'){str[x] = 0;x = elements;}
	}
	break;
   case OFF:
    for(x=0;x<elements;x++) {
	  str[x] = rxb();
	}
	break;
  }
  strcpy(retstr,str);
  return str;
}