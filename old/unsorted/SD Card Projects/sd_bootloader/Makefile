# Makefile for foodloader

TARGET = sd_bootloader

# include avr-generic makefile configuration
include avr.mk

ifeq ($(MCU),atmega644)
	# atmega644 with 1024 words bootloader:
	# bootloader section starts at 0x7c00 (word-address) == 0xf800 (byte-address)
	BOOT_SECTION_START = 0xf800
	#
	# atmega644 with 512 words bootloader:
	# bootloader section starts at 0x7e00 (word-address) == 0xfc00 (byte-address)
	#BOOT_SECTION_START = 0xfc00
endif

LDFLAGS += -Wl,--section-start=.text=$(BOOT_SECTION_START)
CFLAGS += -DBOOT_SECTION_START=$(BOOT_SECTION_START)

OBJECTS += $(patsubst %.c,%.o,$(shell echo *.c))
HEADERS += $(shell echo *.h)
LDFLAGS += -L/usr/local/avr/avr/lib

.PHONY: all

all: $(TARGET).hex $(TARGET).lss
	@echo "==============================="
	@echo "compiled for: $(MCU)"
	@echo -n "bootloader size is: "
	@$(SIZE) -A $< | grep "\.sec1" | tr -s " " | cut -d" " -f2
	@echo "==============================="

$(TARGET): $(OBJECTS) $(TARGET).o

%.o: $(HEADERS)

clean:
	rm -f $(TARGET) *.hex *.eep.hex *.o *.lst *.lss *~
