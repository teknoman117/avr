#include "srf02.h"
#include "i2c.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

void initcommsunit() {
  i2cinit(8,0);
}

void changeAddress(uint8_t address,uint8_t newadd) {
  i2cwritereg(address,0x00,0xA0);
  i2cwritereg(address,0x00,0xA5);
  i2cwritereg(address,0x00,0xAA);
  i2cwritereg(address,0x00,newadd);
}

uint16_t getCM(uint8_t address) {
  i2cwritereg(address,0x00,0x51);
  for(wit=0;wit<1000;wit++);
  dis = i2creadreg(address,0x02);
  dis <<= 8;
  dis += i2creadreg(address,0x03);
  return dis; 
}

uint16_t getIN(uint8_t address) {  
  i2cwritereg(address,0x00,0x50);
  for(wit=0;wit<1000;wit++);
  dis = i2creadreg(address,0x02);
  dis <<= 8;
  dis += i2creadreg(address,0x03);
  return dis; 
}

uint16_t getUS(uint8_t address) {
  i2cwritereg(address,0x00,0x52);
  for(wit=0;wit<1000;wit++);
  dis = i2creadreg(address,0x02);
  dis <<= 8;
  dis += i2creadreg(address,0x03);
  return dis; 
}
  