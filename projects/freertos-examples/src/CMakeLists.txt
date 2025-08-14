# Basic Test
add_executable(blink blink.c)
avr_add_extra_outputs(blink)

# Basic Test
add_executable(blink-test blink-test.c)
target_link_libraries(blink-test PRIVATE FreeRTOS)
avr_add_extra_outputs(blink-test)

# Servo Test
add_executable(servo-test servo-test.c)
target_link_libraries(servo-test PRIVATE FreeRTOS)
avr_add_extra_outputs(servo-test)

# UART Test
add_executable(uart-test uart-test.c)
target_link_libraries(uart-test PRIVATE FreeRTOS)
avr_add_extra_outputs(uart-test)

# Stream Buffer Test
add_executable(streambuffer-test streambuffer-test.c)
target_link_libraries(streambuffer-test PRIVATE FreeRTOS)
avr_add_extra_outputs(streambuffer-test)

# SRF02 Sonar Test
add_executable(srf02-test
    srf02-test.c
    uart.c
    i2c.c
    console.c
)
target_link_libraries(srf02-test PRIVATE FreeRTOS)
avr_add_extra_outputs(srf02-test)

