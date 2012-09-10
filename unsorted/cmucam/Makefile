# Makefile for Compiling AVR projects against mxx4libv3!
# Written by Nathaniel Lewis as part of The Linbot Project

# Compiler and Linker settings
CC= avr-gcc
LD= avr-gcc
SIZE= avr-size
OBJCOPY= avr-objcopy

# Project Info
PROJECT= cmucam_test
MAIN= main

# CPU Info
MCU= atmega644
FREQ= 20000000

# Includes, Objects, and Libraries
LIBMXX4_DIR= /home/nathaniel/Projects/AVR/mxx4libv3
OBJECTS= $(MAIN).o cmucam.o fakeuart.o
INCLUDES= -I. -I$(LIBMXX4_DIR)/include
LIBS= $(LIBMXX4_DIR)/libAVRLib_$(MCU)_$(FREQ).a

# Flags
DEFINES= -DF_CPU=$(FREQ) -DRBUFFER_SIZE0=128 -DTBUFFER_SIZE0=128 -DCMUCAM_UART=0
CFLAGS= $(INCLUDES) -mmcu=$(MCU) -Os -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall $(DEFINES)
LFLAGS= -mmcu=$(MCU) -lm

# Programmer Settings
PROGRAMMER= avrispmkii
PORT= usb

all: binary hex

# Compile and Link project
binary: $(OBJECTS)
	$(LD) $(LFLAGS) -o $(PROJECT).out $(OBJECTS) $(LIBS)

# Build the Intel HEX file for the AVR
hex:
	$(OBJCOPY) -j .text -j .data -O ihex $(PROJECT).out $(PROJECT).hex
	$(SIZE) $(PROJECT).out

# Downloads program to AVR
install:
	avrdude -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -e -U flash:w:$(PROJECT).hex

# Clean the Project
clean:
	rm -rf $(OBJECTS) $(PROJECT).hex $(PROJECT).out
