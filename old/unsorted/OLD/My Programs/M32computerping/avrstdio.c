#include <avr/io.h>
#include "avrstdio.h"

void enable_uart(void) {
  UBRRH = 0x00;
  UBRRL = 25;
  UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);	
  UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
}

void txb(unsigned char dat) {
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = dat;
}

void uprintf(unsigned char data[]) {
	for(x=0;x<strlen(data);x++) {
	  txb(data[x]);
	}
}

unsigned char rxb(void) {
  while( !(UCSRA & (1<<RXC)));
  s = UDR;
  return s;
}

unsigned char *uscanf(uint8_t elements,uint8_t onoff) {
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
  return str;
}