#include <stdint.h>
#include <string.h>
enum {ON,OFF};

uint64_t x;
unsigned char s;
unsigned char str[50];

void enable_uart(void);
void uprintf(unsigned char data[]);
unsigned char *uscanf(uint8_t elements, uint8_t onoff);
void txb(unsigned char dat);
unsigned char rxb(void); 
 