cmake_minimum_required(VERSION 3.20)

include(cmake/avr.cmake)

project(freertos-examples VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(F_CPU 16000000UL)
set(F_I2C 333333UL)
set(MCU atmega328p)
set(BAUD 115200)
set(RXBUFFER_SIZE 128)
set(TXBUFFER_SIZE 128)

add_definitions(-DF_CPU=${F_CPU} -DBAUD=${BAUD} -DF_I2C=${F_I2C} -DRXBUFFER_SIZE=${RXBUFFER_SIZE} -DTXBUFFER_SIZE=${TXBUFFER_SIZE})
add_compile_options(-mmcu=${MCU})
add_link_options(-mmcu=${MCU})

# Add FreeRTOS
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE include)
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)

set(FREERTOS_HEAP "3" CACHE STRING "" FORCE)
set(FREERTOS_PORT "GCC_ATMEGA" CACHE STRING "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/FreeRTOS-Kernel)

# Add project sources
add_subdirectory(src)
