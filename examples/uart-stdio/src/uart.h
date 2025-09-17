#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void setup_uart0();

uint8_t hasbyte0();
uint8_t getbyte0();
void putbyte0(uint8_t b);

#endif /* UART_H_ */