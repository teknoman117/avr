# Function to build an AVR executable with all it
function(avr_add_extra_outputs TARGET)
    set_target_properties(${TARGET} PROPERTIES
        OUTPUT_NAME "${TARGET}.elf"
    )

    # Generate output files
    #add_custom_target(${TARGET}-strip ALL avr-strip ${TARGET}.elf DEPENDS ${TARGET})
    add_custom_target(${TARGET}-hex ALL avr-objcopy -R .eeprom -O ihex ${TARGET}.elf ${TARGET}.hex DEPENDS ${TARGET})
    add_custom_target(${TARGET}-eeprom avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex ${TARGET}.elf ${TARGET}.eep DEPENDS ${TARGET})

    # Clean extra files
    # TODO: make this actually work
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${TARGET}.hex;${TARGET}.eep;${TARGET}.lst")
endfunction()
