#include <stdint.h>

void initcommsunit();
void changeAddress(uint8_t address,uint8_t newadd);
uint16_t getCM(uint8_t address);
uint16_t getIN(uint8_t address);
uint16_t getUS(uint8_t address);

uint16_t dis;
uint32_t wit;
