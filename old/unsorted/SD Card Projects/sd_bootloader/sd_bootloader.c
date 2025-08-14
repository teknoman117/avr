#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include "config.h"
#include "usart0.h"
#include "spi.h"
#include "mmc.h"

#ifdef HONOR_WATCHDOG_RESET
#include <avr/wdt.h>
#endif

#define TIMEOUT 256

uint16_t flash_address;

/* prototypes */
void (*jump_to_application)(void) = (void *)0x0000;

char numbuf[10];

/* defines */
#define HIGH(x) ( (uint8_t) (x >> 8) )
#define LOW(x)  ( (uint8_t) x )

#define noinline __attribute__((noinline))

/** move interrupt vectors to application section and jump to main program */
static noinline void start_application(void) {
    /* reset input pin */
    START_PORT &= START_MASK;
    /* move interrupt vectors to application section and jump to main program */
    MCUCR = _BV(IVCE);
    MCUCR = 0;
    jump_to_application();
}

static noinline int uart_get( void ) {
    uint16_t counter = 0;
    while( !(UCSR0A & (1<<RXC0))) {
        if(counter++ > TIMEOUT) return -1;
        _delay_us(1);
    }
    return UDR0;
}

static noinline int obtain_slot( void ) {  //return a -1 for current slot, num for new
    int slot = -1;
    uint16_t counter = 0;
    int ch;
    for(counter=0;counter<10;counter++) numbuf[counter] = '\0';
    counter = 0;
    while(1) {
        if((ch = uart_get()) > -1) {
            if(ch == '\r') {
                slot = atoi(numbuf);
                for(counter=0;counter<10;counter++) numbuf[counter] = '\0';
                counter = 0;
            } else {
                numbuf[counter++] = ch;
            }
        }
        if(START_PIN & _BV(START_PINNUM)) break;
    }
    return slot;
}

static noinline char start_mmc_read(uint32_t address) {
    uint16_t al = address & 0x0000FFFF;
	uint16_t ah = (address >> 16) & 0x0000FFFF;
	uint8_t r1 =  mmc_command(0x51, 0xFF, ah, al);
	for (al = 0; al < 50000; al++) {
		if (r1 == 0) break;
		r1 = spi_transfer(0xFF);
	}
	if (r1 != 0) return 1;
	while(spi_transfer(0xFF) != 0xFE);
	return 0;
}

int main(void) {
#   ifdef HONOR_WATCHDOG_RESET
    /* if this reset was caused by the watchdog timer, just start the application, else disable the watchdog */
    if (MCUSR & _BV(WDRF)) jump_to_application();
    else wdt_disable();
#   endif

    /* BUF_T is defined in config.h, according the pagesize */
    BUF_T buffer_size, i;
    init_spi( FOSC_2, MODE0, MSBFIRST );       //start SPI
	if( !init_mmc() ) jump_to_application();   //init mmc, if fail jump to application
    init_usart0( UART_UBRR, DB8 | P_N | SB1 ); //start the usart
    int slot = obtain_slot();                  //obtain the new slot number
    if( slot < 0 ) jump_to_application();      //if current slot, jump to application
    /* upload new code */
    /* do a chip erase */
    for (flash_address = 0; flash_address < BOOT_SECTION_START; flash_address += SPM_PAGESIZE)
        boot_page_erase_safe(flash_address);
    /* get the location and size of the program to be downloaded */
    int index = slot * 8;
    uint32_t progsize = 0;
    uint32_t progaddr = 0;
    mmc_readpartialblock( index, 4, &progsize );
    mmc_readpartialblock( index+4, 4, &progaddr );
    uint16_t temp_buffer;
    uint16_t temp_address = flash_address = 0;
    txb_0('p');
    while(1) {
        buffer_size = 512;
        if(buffer_size > BLOCKSIZE) buffer_size = BLOCKSIZE;
        if(buffer_size > progsize) buffer_size = progsize;
        progsize -= buffer_size;
        temp_address = flash_address;
        boot_spm_busy_wait();
        start_mmc_read(progaddr);
        for (i = 0; i < buffer_size/2; i++) {
            /* get data word */
            temp_buffer = spi_transfer(0xFF) | (spi_transfer(0xFF) << 8);
            /* write data to temporary buffer */
            boot_page_fill(temp_address, temp_buffer);
            /* increment by two, since temp_address is a byte* address, but we are writing words! */
            temp_address += 2;
        }
        for(;i<256;i++) {
            spi_transfer(0xFF);
            spi_transfer(0xFF);
        }
        spi_transfer(0xFF);
        spi_transfer(0xFF);
        /* after filling the temp buffer, write the page and wait till we're done */
        boot_page_write_safe(flash_address);
        boot_spm_busy_wait();
        /* re-enable application flash section, so we can read it again */
        boot_rww_enable();
        /* store next page's address, since we do auto-address-incrementing */
        flash_address = temp_address;
        progaddr += buffer_size;
        if(progsize == 0) break;
    }
    txb_0('d');
    start_application();
    return 0;
}
