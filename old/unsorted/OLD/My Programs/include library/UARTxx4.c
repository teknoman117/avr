#include "UARTxx4.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>

void enable_uart(void) {
  UBRR0H = 0x00;
  UBRR0L = 12;
  UCSR0B = (1<<RXCIE0)|(1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);	
  UCSR0C = (1<<UMSEL01)|(1<<UCSZ01)|(1<<UCSZ00);
  sei();
}

void sendByte(char dat) {
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = dat;
}

void write(char data[]) {
    uint8_t x;
	for(x=0;x<strlen(data);x++) {
	  sendByte(data[x]);
	}
}

SIGNAL (SIG_USART_RECV) {
  ins = UDR0;
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