cmake_minimum_required(VERSION 3.20)

# SRF02 Sonar Test
add_executable(srf02-test
    main.c
    uart.c
    i2c.c
    console.c
)
target_link_libraries(srf02-test PRIVATE freertos_kernel)
avr_add_extra_outputs(srf02-test)
