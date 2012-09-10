# programs
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AS = avr-as
CP = cp
RM = rm -f
AVRDUDE = avrdude
AVRDUDE_BAUDRATE = 115200
SIZE = avr-size
MCU=atmega644
F_CPU=11059200UL

-include config.mk

# flags for avrdude
ifeq ($(MCU),atmega644)
	AVRDUDE_MCU=m644
endif

AVRDUDE_FLAGS += -p $(AVRDUDE_MCU) -b $(AVRDUDE_BAUDRATE)

# flags for the compiler
CFLAGS += -g -Os -finline-limit=800 -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=gnu99

# flags for the linker
LDFLAGS += -mmcu=$(MCU)

ifneq ($(DEBUG),)
	CFLAGS += -Wall -W -Wchar-subscripts -Wmissing-prototypes
	CFLAGS += -Wmissing-declarations -Wredundant-decls
	CFLAGS += -Wstrict-prototypes -Wshadow -Wbad-function-cast
	CFLAGS += -Winline -Wpointer-arith -Wsign-compare
	CFLAGS += -Wunreachable-code -Wdisabled-optimization
	CFLAGS += -Wcast-align -Wwrite-strings -Wnested-externs -Wundef
	CFLAGS += -Wa,-adhlns=$(basename $@).lst
	CFLAGS += -DDEBUG
endif

all:

.PHONY: sanity-check

sanity-check:
ifndef F_CPU
	@echo "please define F_CPU!"
	@exit 1
endif

ifndef MCU
	@echo "please define MCU!"
	@exit 1
endif


$(OBJECTS): sanity-check

.PHONY: all 

%.hex: %
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.eep.hex: %
	$(OBJCOPY) --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex -j .eeprom $< $@

%.lss: %
	$(OBJDUMP) -h -S $< > $@

%-size: %.hex
	$(SIZE) $<


