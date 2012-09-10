# Makefile for Compiling AVR projects
# Written by Nathaniel Lewis

# Compiler and Linker settings
CC= avr-gcc
LD= avr-gcc
SIZE= avr-size
OBJCOPY= avr-objcopy

# Project Info
PROJECT=simple_rtos
MAIN=main

# CPU Info
MCU= atmega644
FREQ= 11059200


# Source => Objects
C_SOURCES=main.c avr-rtos-lib.c
C_OBJECTS=$(C_SOURCES:.c=.o) 

ASM_SOURCES=avr-rtos-isr.s avr-rtos-task-scheduling.s avr-rtos-task-yield.s
ASM_OBJECTS=$(ASM_SOURCES:.s=.o) 

HEADERS=avr-rtos.h avr-rtos-asm-defs.inc

# Flags
DEFINES= -DF_CPU=$(FREQ)
CFLAGS= -mmcu=$(MCU) $(DEFINES) -O2 -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -I. 
LFLAGS= -mmcu=$(MCU) -lm

# Programmer Settings
PROGRAMMER= avrispmkii
PORT= usb

all: binary hex

# Compile and Link project
binary: $(C_OBJECTS) $(ASM_OBJECTS) $(HEADERS)
	$(LD) $(LFLAGS) -o $(PROJECT).out $(C_OBJECTS) $(ASM_OBJECTS)

# Build the Intel HEX file for the AVR
hex: binary
	$(OBJCOPY) -j .text -j .data -O ihex $(PROJECT).out $(PROJECT).hex
	$(SIZE) $(PROJECT).out

# Downloads program to AVR
install: hex
	avrdude -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -e -U flash:w:$(PROJECT).hex

# Clean the Project
clean:
	rm -rf *.o .DS_Store *~ $(PROJECT).hex $(PROJECT).out

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

.s.o:
	$(CC) $(CFLAGS) $< -c -o $@

# Documentation
docs:
	doxygen Doxyfile

cleandocs:
	rm -rf html