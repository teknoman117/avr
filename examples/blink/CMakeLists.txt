cmake_minimum_required(VERSION 3.20)

include(cmake/avr.cmake)

project(blink VERSION 0.1 LANGUAGES C)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_C_COMPILER avr-gcc)

set(F_CPU 16000000UL)
set(MCU atmega328p)

add_definitions(-DF_CPU=${F_CPU})
add_compile_options(-mmcu=${MCU})
add_link_options(-mmcu=${MCU})

# Add project sources
add_executable(blink blink.c)
avr_add_extra_outputs(blink)
