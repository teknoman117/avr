# Makefile for Compiling AVR projects against mxx4libv3!
# Written by Nathaniel Lewis as part of The Linbot Project

# Compiler and Linker settings
CC= avr-gcc
LD= avr-gcc
SIZE= avr-size
OBJCOPY= avr-objcopy

# Project Info
PROJECT= threading
MAIN= main

# CPU Info
MCU= atmega644
FREQ= 20000000

# Includes, Objects, and Libraries
LIBMXX4_DIR= /home/nathaniel/Projects/AVR/mxx4libv3
OBJECTS= $(MAIN).o fakeuart.o
INCLUDES= -I. -I$(LIBMXX4_DIR)/include
LIBS= $(LIBMXX4_DIR)/libAVRLib_$(MCU)_$(FREQ).a libavr-thread.a

# Flags
CFLAGS= $(INCLUDES) -mmcu=$(MCU) -Os -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -DF_CPU=$(FREQ)
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
