#include <avr/io.h>
#include <stdio.h>

void i2cinit(uint8_t speed,uint8_t prescaler);           //values are 0 through 3 for prescaler
void i2cstart(void);
void i2cstop(void);
uint8_t i2ctransmit(uint8_t data);
uint8_t i2creceive(uint8_t ack);
void i2cwritereg(uint8_t address, uint8_t reg, uint8_t value);
uint8_t i2creadreg(uint8_t address, uint8_t reg);


