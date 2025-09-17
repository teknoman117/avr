cmake_minimum_required(VERSION 3.20)

add_executable(dram
    main.c
    dram.c
    uart.c
)

avr_add_extra_outputs(dram)
