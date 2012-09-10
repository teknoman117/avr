#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "i2c.h"

void i2cinit(uint8_t speed,uint8_t prescaler) {
  TWBR = speed;
  TWSR &= 0xF8;
  TWSR += prescaler;
}

void i2cstart(void) {
  TWCR= (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
}

void i2cstop(void) {
  TWCR= (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
}

uint8_t i2ctransmit(uint8_t data) {
  uint8_t ret;
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  if((TWSR & 0xF8) == 0x18 || (TWSR & 0xF8) == 0x28) {ret = 1;}
  if((TWSR & 0xF8) == 0x20 || (TWSR & 0xF8) == 0x30) {ret = 0;}
  return ret;
}

uint8_t i2creceive(uint8_t ack) {
  TWCR = (1 << TWINT) | (ack << TWEA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  return TWDR;
}
    
void i2cwritereg(uint8_t address, uint8_t reg, uint8_t value) {
  i2cstart();
  i2ctransmit(address);
  i2ctransmit(reg);
  i2ctransmit(value);
  i2cstop();
}

uint8_t i2creadreg(uint8_t address, uint8_t reg) {
  uint8_t retval;
  i2cstart();
  i2ctransmit(address);
  i2ctransmit(reg);
  i2cstart();
  i2ctransmit(address + 1);
  i2creceive(retval);
  i2cstop();
  return retval;
}