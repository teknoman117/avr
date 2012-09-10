# Makefile for compiling the Linbot Project AVR Library for the
# ATmega164(p), ATmega324(p), and ATmega644(p) microcontrollers

CC=avr-gcc

AR=avr-ar

RM=rm -rf

FREQ=1000000

MCU=atmega644

OBJECTS= src/a2d.o src/acomp.o src/crc32.o src/eeprom.o src/extint.o src/i2c.o src/i2c_slave.o src/iusart0.o src/iusart1.o src/mmc.o src/si2c.o src/spi.o src/stdusart0.o src/stdusart1.o src/timer0.o src/timer0ctc.o src/timer1.o src/timer1pwm.o src/timer2pwm.o src/usart0.o src/timer1servo.o

LIB=libAVRLib_$(MCU)_$(FREQ).a

DEFINES= -DF_CPU=$(FREQ) -DRBUFFER_SIZE0=256 -DTBUFFER_SIZE0=256 -DRBUFFER_SIZE1=256 -DTBUFFER_SIZE1=256

CFLAGS= -Iinclude -mmcu=$(MCU) -Os -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall $(DEFINES)

all: $(OBJECTS)
	$(AR) rcs $(LIB) $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

