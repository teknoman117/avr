#include "srf02.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

uint16_t getCM(uint8_t address) {
  TWBR = 3;
  if((TWSR & 1) == 1){TWSR -= 1;}
  if(((TWSR >> 1) & 1) == 1){TWSR -= 2;}
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x00;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x51;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  _delay_loop_2(65535);
  TWCR = (1 << TWINT) | (! << TWSTO) | (1 << TWEN);
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x02;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address + 1;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  dis = TWDR;
  dis <<= 8; 
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x03;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address + 1;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  dis += TWDR;
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
  return dis;
}

uint16_t getIN(uint8_t address) {
  TWBR = 3;
  if((TWSR & 1) == 1){TWSR -= 1;}
  if(((TWSR >> 1) & 1) == 1){TWSR -= 2;}
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x00;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x50;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  _delay_loop_2(65535);
  TWCR = (1 << TWINT) | (! << TWSTO) | (1 << TWEN);
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x02;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address + 1;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  dis = TWDR;
  dis <<= 8; 
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x03;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address + 1;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  dis += TWDR;
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
  return dis;
}

uint16_t getUS(uint8_t address) {
  TWBR = 3;
  if((TWSR & 1) == 1){TWSR -= 1;}
  if(((TWSR >> 1) & 1) == 1){TWSR -= 2;}
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x00;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x52;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  _delay_loop_2(65535);
  TWCR = (1 << TWINT) | (! << TWSTO) | (1 << TWEN);
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x02;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address + 1;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  dis = TWDR;
  dis <<= 8; 
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = 0x03;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  TWDR = address + 1;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT)));
  dis += TWDR;
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
  return dis;
}
  