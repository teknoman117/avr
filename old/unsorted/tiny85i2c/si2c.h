#ifndef _AVR_SI2C_H_
#define _AVR_SI2C_H_

#include <stdint.h>
#include <avr/io.h>

/* Pin Definitions */
#define SCLP      PORTB1
#define SDAP      PORTB0

#define SCLDDR    DDRB
#define SDADDR    DDRB

#define SCLPIN    PINB
#define SDAPIN    PINB

#define SCLPORT   PORTB
#define SDAPORT   PORTB

/* Pin macros */
#define i2c_init() SDAPORT &= ~(1 << SDAP);SCLPORT &= ~(1 << SCLP);
#define SDAON()   SDADDR &= ~(1 << SDAP);
#define SDAOFF()  SDADDR |= (1 << SDAP);
#define SCLON()   SCLDDR &= ~(1 << SCLP);
#define SCLOFF()  SCLDDR |= (1 << SCLP);
#define SDA       ((SDAPIN >> SDAP)&1)
#define SCL       ((SCLPIN >> SCLP)&1)

char i2c_start( uint8_t address );

void i2c_stop();

char i2c_writebyte( uint8_t byte );

uint8_t i2c_readbyte( char ack );

#endif
