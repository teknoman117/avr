#include <stdint.h>
#include <string.h>

char FIFO[50],ins,outs[50],str[50],fsr[50];

void enable_uart(void);
void sendByte(char dat);
void write(char data[]);
char *read(char rts,unsigned int num);

