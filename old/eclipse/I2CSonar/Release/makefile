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
I2CSonar.lss \

FLASH_IMAGE += \
I2CSonar.hex \

EEPROM_IMAGE += \
I2CSonar.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: I2CSonar.elf secondary-outputs

# Tool invocations
I2CSonar.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,I2CSonar.map -mmcu=attiny85 -o "I2CSonar.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

I2CSonar.lss: I2CSonar.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S I2CSonar.elf  >"I2CSonar.lss"
	@echo 'Finished building: $@'
	@echo ' '

I2CSonar.hex: I2CSonar.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex I2CSonar.elf  "I2CSonar.hex"
	@echo 'Finished building: $@'
	@echo ' '

I2CSonar.eep: I2CSonar.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex I2CSonar.elf  "I2CSonar.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: I2CSonar.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t I2CSonar.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) I2CSonar.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
