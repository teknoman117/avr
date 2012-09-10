#include "UART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>

void enable_uart(void) {
  UBRRH = 0x00;
  UBRRL = 12;
  UCSRB = (1<<RXCIE)|(1<<RXCIE)|(1<<RXEN)|(1<<TXEN);	
  UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
  sei();
}

void sendByte(char dat) {
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = dat;
}

void write(char data[]) {
    uint8_t x;
	for(x=0;x<strlen(data);x++) {
	  sendByte(data[x]);
	}
}

SIGNAL (SIG_UART_RECV) {
  ins = UDR;
  strcat(FIFO,ins);
}

char *read(char rts,unsigned int num) { 
  unsigned int x;
  x = 0;
  while(x < num) {
    strcat(fsr,FIFO[x]);
	x = x + 1;
  }
  x = num;
  while(x < strlen(FIFO)) {
    strcat(str,FIFO[x]);
	x = x + 1;
  }
  strcpy(FIFO,str);
  strcpy(rts,fsr);
  return fsr;
}
    
  
