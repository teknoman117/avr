################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
qenc_simulator.lss \

FLASH_IMAGE += \
qenc_simulator.hex \

EEPROM_IMAGE += \
qenc_simulator.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: qenc_simulator.elf secondary-outputs

# Tool invocations
qenc_simulator.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,qenc_simulator.map -mmcu=attiny85 -o "qenc_simulator.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

qenc_simulator.lss: qenc_simulator.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S qenc_simulator.elf  >"qenc_simulator.lss"
	@echo 'Finished building: $@'
	@echo ' '

qenc_simulator.hex: qenc_simulator.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex qenc_simulator.elf  "qenc_simulator.hex"
	@echo 'Finished building: $@'
	@echo ' '

qenc_simulator.eep: qenc_simulator.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex qenc_simulator.elf  "qenc_simulator.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: qenc_simulator.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t qenc_simulator.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) qenc_simulator.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
