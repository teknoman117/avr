#include <stdint.h>
#include <avr/io.h>

void SetDir(char port,uint8_t pin,char dir);
void SetPin(char port,uint8_t pin,uint8_t state);
uint8_t GetPin (char port,uint8_t pin);