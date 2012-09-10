#include <stdint.h>
#include <string.h>
enum {ON,OFF};

uint64_t x;
char s;
char str[50];

void enable_uart(uint16_t UBRRval);
void uprintf(char data[]);
char *uscanf(char retstr[],uint8_t elements, uint8_t onoff);
void txb(char dat);
char rxb(void); 
 