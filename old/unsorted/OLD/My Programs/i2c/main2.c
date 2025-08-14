#define F_CPU 11059200UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void i2c_start(void);
void i2c_stop(void);
unsigned char i2c_tx(unsigned char d);
unsigned char i2c_rx(unsigned char ack);

int main () {
  i2c_start();i2c_tx(0xE0);i2c_tx(0x00);i2c_tx(0xA0);i2c_stop();
  i2c_start();i2c_tx(0xE0);i2c_tx(0x00);i2c_tx(0xA5);i2c_stop();
  i2c_start();i2c_tx(0xE0);i2c_tx(0x00);i2c_tx(0xAA);i2c_stop();
  i2c_start();i2c_tx(0xE0);i2c_tx(0x00);i2c_tx(0xE4);i2c_stop();
}

void i2c_start(void) {
  DDRB=0xFF;
  PORTB=0;
  PORTB+=1;
  _delay_loop_2(3000);
  PORTB+=2;
  _delay_loop_2(3000);
  PORTB-=1;
  _delay_loop_2(3000);
  PORTB-=2;
  _delay_loop_2(3000);
}

void i2c_stop(void) {
  DDRB=0xFF;
  PORTB=0;
  _delay_loop_2(3000);
  PORTB+=2;
  _delay_loop_2(3000);
  PORTB+=1;
  _delay_loop_2(3000);
}

unsigned char i2c_tx(unsigned char d) {
  DDRB=0xFF;
  char x;
  unsigned char b;
  for(x=8;x;x--) {
    if(d&0x80) PORTB=1;
	else PORTB=0;
	PORTB+=2;
	d<<=1;
	PORTB-=2;
  }
  PORTB=3;
  DDRB=0xFE;
  _delay_loop_2(3000);
  b=(PINB&1);
  DDRB=0xFF;
  PORTB=0;
  return b;
}

unsigned char i2c_rx(unsigned char ack) {
  DDRB=0xFE;
  char x,d=0;
  for(x=0;x<8;x++) {
    d<<=1;
	DDRB-=2;
	while((PINB>>1)&1);
	_delay_loop_2(3000);
	if(PINB&1) d|=1;
	DDRB+=2;
	PORTB=2;
  }
  DDRB=0xFF;
  if(ack) PORTB=0;
  else PORTB=1;
  PORTB+=2;
  _delay_loop_2(3000);
  PORTB=1;
  return d;
}