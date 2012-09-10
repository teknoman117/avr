#include <avr/io.h>
#include <stdint.h>
#include <string.h>

char rxbuffer[256];

void init_uart (uint16_t baudrate, uint8_t databits, uint8_t parity, uint8_t stopbits) {
	UBRR0H = (uint8_t) (baudrate>>8);
	UBRR0L = (uint8_t) baudrate;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	UCSR0C = 0;
	switch (databits) {
		case 5: break; 
		case 6: UCSR0C |= (1 << UCSZ00); break;
		case 7: UCSR0C |= (1 << UCSZ01); break;
		case 8: UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); break;
	}
	switch (parity) {
		case 'n': break;
		case 'e': UCSR0C |= (1 << UPM01); break;
		case 'o': UCSR0C |= (1 << UPM00) | (1 << UPM01); break;
	}
    if(stopbits == 2) {UCSR0C |= (1 << USBS0);}
}

void txb(char b) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = (char) b;
}

char rxb() {
   while (!(UCSR0A & (1 << RXC0)));
   return UDR0;
}

void u_transmit(char txbytes[]) {
	uint16_t i;
	for(i = 0; i < strlen(txbytes); i++) {txb(txbytes[i]);}
}

void u_transmitn(char txbytes[], uint16_t num) {
	uint16_t i;
	for(i = 0; ((i < num) && (i < strlen(txbytes))); i++) {txb(txbytes[i]);}
}

void u_receive(char rxbytes[], uint16_t num) {
	uint16_t i;
    strcpy(rxbytes, "");
    for(i = 0; i < num; i++) {strcat(rxbytes, (char *)rxb());}
}

void u_receivec(char rxbytes[], char schar) {
    char bt;
    char op = 1;
    strcpy(rxbytes, "");
    while (op == 1) {
    	bt = rxb();
    	if(bt == schar) {op = 0;}
    	else {strcat(rxbytes, (char*) bt);}
    }
}
