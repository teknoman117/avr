cmake_minimum_required(VERSION 3.20)

add_executable(uart-stdio
    main.c
    uart.c
)

avr_add_extra_outputs(uart-stdio)
