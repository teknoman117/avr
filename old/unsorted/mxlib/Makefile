# Makefile for compiling the Linbot Project AVR Library for the
# ATmega164(p), ATmega324(p), and ATmega644(p) microcontrollers

CC=avr-gcc

AR=avr-ar

RM=rm -rf

FREQ=1000000

MCU=atmega8

OBJECTS= src/a2d.o src/eeprom.o src/extint.o src/iusart.o src/timer0ctc.o src/timer1pwm.o

LIB=libAVRLib_$(MCU).a

DEFINES= -DF_CPU=$(FREQ)

CFLAGS= -Iinclude -mmcu=$(MCU) -Os -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall $(DEFINES)

all: $(OBJECTS)
	$(AR) rcs $(LIB) $(OBJECTS)

clean:
	$(RM) $(OBJECTS) *.a

