#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "config.h"

uint16_t flash_address;             /* start flash (byte address, converted) write at this address */

void (*jump_to_application)(void) = (void *)0x0000;

/* defines */
#define HIGH(x) ( (uint8_t) (x >> 8) )
#define LOW(x)  ( (uint8_t) x )

#define noinline __attribute__((noinline))


/** output one character */
static noinline void uart_putc(uint8_t data) {
    /* loop until data has been transmitted */
    while (!(_UCSRA_UART0 & _BV(_UDRE_UART0)));
    /* put data in buffer */
    _UDR_UART0 = data;
}

/** output a string */
static inline void uart_puts(uint8_t buffer[]) {
    /* send everything until end of string */
    while (*buffer != 0) {
        uart_putc(*buffer);
        buffer++;
    }
}

/** block until one character has been read */
static noinline uint8_t uart_getc(void) {
    /* wait if a byte has been received */
    while (!(_UCSRA_UART0 & _BV(_RXC_UART0)));
    /* return received byte */
    return _UDR_UART0;
}

/* loop a few times, and see if the character is received */
static inline uint8_t wait_for_char(void) {
    uint16_t i;
    for(i = 0; i < 1000; i++) {
        _delay_loop_2(65535);
        if(_UCSRA_UART0 & _BV(_RXC_UART0)) {
            if(_UDR_UART0 == BOOTLOADER_ENTRY_CHAR) {
                    return 1;
            }
        }
    }
    /* never received the character */
    return 0;
}

/** init the hardware uart */
static inline void init_uart(void) {
    /* set baud rate */
    _UBRRL_UART0 = 71; /* low byte */
    /* set mode */
    _UCSRC_UART0 = UART_UCSRC;
    /* enable transmitter, receiver */
    _UCSRB_UART0 = _BV(_TXEN_UART0) | _BV(_RXEN_UART0);
}

/** move interrupt vectors to application section and jump to main program */
static noinline void start_application(void) {
    /* move interrupt vectors to application section and jump to main program */
    uart_putc('X');
    _IVREG = _BV(IVCE);
    _IVREG = 0;
    jump_to_application();
}

int main(void) {
    /* BUF_T is defined in config.h, according the pagesize */
    BUF_T buffer_size;
    BUF_T i = 0;
    uint16_t temp_address = 0;
    uint16_t temp_word_buffer = 0;
    init_uart();
    uart_putc('b');
    /* bootloader activation methods */
    if (wait_for_char() == 0) start_application();
    uart_putc('p');
    while (1) {
        uint8_t command = uart_getc();
        switch (command) {
            case 'a':   /* set write address start (in words), read high and low byte and respond with CR */
                temp_address = (uart_getc() << 8) | uart_getc();
                flash_address = temp_address << 1;
                /* acknowledge */
                uart_putc('\r');
                break;
            case 'e':   /* do a chip-erase, respond with CR afterwards */
                for (flash_address = 0; flash_address < BOOT_SECTION_START; flash_address += SPM_PAGESIZE) {
                    boot_page_erase_safe(flash_address);
                }
                uart_putc('\r');
                break;
            case 'x':   /* start application */
                start_application();
                break;
            case 's':   /* check block size */
                uart_putc(HIGH(BLOCKSIZE));
                uart_putc(LOW(BLOCKSIZE));
                break;
            case 'w':   /* start block flash or eeprom load (fill mcu internal page buffer) */
                /* first, read buffer size (in bytes) */
                buffer_size = (uart_getc() << 8) | uart_getc();
                /* check if our buffer can hold all this data */
                if (buffer_size > BLOCKSIZE) {
                        uart_putc('?');
                        break;
                }
                if (flash_address > BOOT_SECTION_START) {
                    uart_putc(0);
                }
                temp_address = flash_address;
                boot_spm_busy_wait();
                /* read data, wordwise, low byte first */
                for (i = 0; i < buffer_size/2; i++) {
                    temp_word_buffer = uart_getc() | (uart_getc() << 8);
                    boot_page_fill(temp_address, temp_word_buffer);
                    temp_address += 2;
                }
                boot_page_write_safe(flash_address);
                boot_spm_busy_wait();
                boot_rww_enable();
                flash_address = temp_address;
                uart_putc('\r');
                break;
            case 'r':   /* start block flash or eeprom read */
                buffer_size = (uart_getc() << 8) | uart_getc();
                for (i = 0; i < buffer_size; i += 2) {
                    /* read word */
                    temp_word_buffer = pgm_read_word(flash_address);
                    /* send data */
                    uart_putc(LOW(temp_word_buffer));
                    uart_putc(HIGH(temp_word_buffer));
                    /* increment address by 2, since it's a byte address */
                    flash_address += 2;
                }
                break;
            default:    /* default: respond with '?' */
                uart_putc('?');
                break;
        }

    }
}
