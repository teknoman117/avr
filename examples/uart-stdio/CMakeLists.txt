cmake_minimum_required(VERSION 3.20)

include(cmake/avr.cmake)

project(uart-stdio VERSION 0.1 LANGUAGES C)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_C_COMPILER avr-gcc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(F_CPU 16000000UL)
set(MCU atmega162)
set(BAUD 115200)

add_definitions(-DF_CPU=${F_CPU} -DBAUD=${BAUD})
add_compile_options(-mmcu=${MCU})
add_link_options(-mmcu=${MCU})

# Add project sources
add_subdirectory(src)
