#ifndef _SDBOOTLOADER_CONFIG_H
#define _SDBOOTLOADER_CONFIG_H

/* only do avr specific configuration when compiling for avr architecture */
#ifdef __AVR__

#include <avr/version.h>

/* check for avr-libc version */
#if __AVR_LIBC_VERSION__ < 10402UL
#error newer libc version (>= 1.4.2) needed!
#endif

/* check if cpu speed is defined */
#ifndef F_CPU
#error please define F_CPU!
#endif

/* check if the bootloader start address has been given
#if !(defined(BOOT_SECTION_START))
#error please define BOOT_SECTION_START as the byte address of bootloader section
#endif

/* start button configuration */
#ifndef START_DDR
#define START_DDR DDRB
#endif

#ifndef START_PORT
#define START_PORT PORTB
#endif

#ifndef START_PIN
#define START_PIN PINB
#endif

#ifndef START_PINNUM
#define START_PINNUM PINB0                    //if pulled to ground, AVR goes into bootloader mode
#endif

#define START_MASK _BV(START_PINNUM)

/* watchdog configuration */
#define HONOR_WATCHDOG_RESET

/* buffer load configuration */
#define BLOCKSIZE SPM_PAGESIZE

/* use 8 or 16 bit counter, according to the page size of the target device */
#if SPM_PAGESIZE < 256
#   define BUF_T uint8_t
#else
#   define BUF_T uint16_t
#endif

/* uart configuration */
#ifndef UART_BAUDRATE
#define UART_BAUDRATE 115200
#endif

#ifndef UART_UBRR
#define UART_UBRR ((F_CPU/(UART_BAUDRATE*16))-1)
#endif

#endif

#endif /* _SDBOOTLOADER_CONFIG_H */
