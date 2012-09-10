#define F_CPU 11059200UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void i2c_start(void);
void i2c_stop(void);
unsigned char i2c_tx(unsigned char d);
unsigned char i2c_rx(unsigned char ack);
void i2c_dly(void);

int main () {
  i2cstart();i2c_tx(0xE0);i2c_tx(0x00);i2c_tx(0xA0);i2cstop();
  i2cstart();i2c_tx(0xE0);i2c_tx(0x00);i2c_tx(0xA5);i2cstop();
  i2cstart();i2c_tx(0xE0);i2c_tx(0x00);i2c_tx(0xAA);i2cstop();
}

void i2c_start(void) {
  PORTB=0;
  DDRB=0xFF;
  PORTB+=1;
  i2c_dly();
  PORTB+=2;
  i2c_dly();
  PORTB-=1;
  i2c_dly();
  PORTB-=2;
  i2c_dly();
}

void i2c_stop(void) {
  DDRC=0xFF;
  PORTB = 0;             
  i2c_dly();
  PORTB += 2;
  i2c_dly();
  PORTB += 1;
  i2c_dly();
}

unsigned char i2c_tx(unsigned char d) {
DDRB=0xFF;
char x;
unsigned char b;
  for(x=8; x; x--) {
    if(d&0x80) PORTB=1;
    else PORTB=0;
    PORTB += 2;
    d <<= 1;
    PORTB -= 2;
  }
  PORTB=3;
  DDRB=0xFE;
  i2c_dly();
  b = (PINB & 1);          // possible ACK bit
  DDRB=0xFF;
  PORTB = 0;
  return b;
}

unsigned char i2c_rx(unsigned char ack) {
DDRB=0xFE;
char x,y=1;,d=0; 
  for(x=0; x<8; x++) {
    d <<= 1;
    DDRB-=2;
    while((PINB>>1)&1);	
    i2c_dly();
    if(PINB&1) d |= 1;
    DDRB+=2;
    PORTB = 2;
  } 
  DDRB=0xFF;
  if(ack) PORTB=0;
  else PORTB=1;
  PORTB+=2;
  i2c_dly();             
  PORTB = 1;
  return d;
}

void i2c_dly (void) {_delay_loop_2(3000);}