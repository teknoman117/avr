#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "dram.h"

#include <stdio.h>
#include <stdlib.h>

#define XMODEM_SOH 0x01
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_ETB 0x17
#define XMODEM_CAN 0x18
#define XMODEM_HEY_LISTEN 0x43

// millisecond counter
static volatile uint32_t millis = 0;
ISR(TIMER1_COMPA_vect) {
    millis++;
}

// delay a specific number of milliseconds
void delay(uint16_t ms) {
    uint32_t next = millis;
    next += ms;
    while (millis < next) {
        // spin
    }
}

// delay until a specific time is reached
inline static void delay_until(uint32_t ms) {
    while (millis < ms) {
        // spin
    }
}

static uint8_t dram_buffer[128] = {0};

// compute crc16
uint16_t crc16(uint8_t* data, size_t size) {
    uint16_t crc = 0;
    for (uint16_t i = 0; i < size; i++) {
        // add to crc computation
        crc = crc ^ (uint16_t) data[i] << 8;
        uint8_t j = 8;
        do {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        } while (--j);
    }
    return crc;
}

// upload flash over xmodem
void dram_upload_xmodem() {
    // synchronize with receiver
    while (getbyte0() != XMODEM_HEY_LISTEN);

    // send all 128 KiB of dram
    uint32_t address = 0;
    uint32_t address_end = 0x20000;
    do {
        // send header
        uint8_t packet_number = (address >> 7) + 1;
        putbyte0(XMODEM_SOH);
        putbyte0(packet_number);
        putbyte0(255 - packet_number);

        // read data from dram
        dram_read(dram_buffer, address, sizeof dram_buffer);
        uint16_t crc = crc16(dram_buffer, sizeof dram_buffer);
        for (uint8_t i = 0; i < sizeof dram_buffer; i++) {
            putbyte0(dram_buffer[i]);
        }

        // send checksum
        putbyte0(crc >> 8);
        putbyte0(crc & 0xff);

        // await ack
        uint8_t response = getbyte0();
        if (response == XMODEM_ACK) {
            address += 128;
        }
    } while (address < address_end);

    // end transmission
    putbyte0(XMODEM_EOT);
    getbyte0();
}

void dram_download_xmodem() {
    // wait for something
    uint32_t curtime = millis;
    do {
        putbyte0(XMODEM_HEY_LISTEN);
        while (!hasbyte0() && ((millis - curtime) < 1000));
        curtime = millis;
    } while (!hasbyte0());

    // receive up to 128 KiB of DRAM
    uint32_t address = 0;
    uint32_t address_end = 0x20000;
    while (1) {
        // process header
        uint8_t header = getbyte0();
        if (header == XMODEM_ETB) {
            break;
        } else if (header == XMODEM_EOT) {
            break;
        } else if (header != XMODEM_SOH) {
            return;
        }

        // receive packet number
        uint8_t packet_number = getbyte0();
        uint8_t n_packet_number = getbyte0();

        // receive packet data
        for (uint8_t i = 0; i < sizeof dram_buffer; i++) {
            dram_buffer[i] = getbyte0();
        }
        uint16_t crc = crc16(dram_buffer, sizeof dram_buffer);

        // receive packet crc
        uint16_t remote_crc;
        remote_crc = getbyte0();
        remote_crc <<= 8;
        remote_crc |= getbyte0();

        // validate packet crc
        if (crc != remote_crc) {
            putbyte0(XMODEM_NAK);
            continue;
        }

        // program dram
        if (address < address_end) {
            dram_write(dram_buffer, address, sizeof dram_buffer);
            address += sizeof dram_buffer;
        }
        putbyte0(XMODEM_ACK);
    }
    putbyte0(XMODEM_ACK);
}

int main() {
    setup_uart0();
    setup_dram();

    // setup millisecond timer
    OCR1A = F_CPU / 1000UL;
    TCCR1B = _BV(WGM12) | _BV(CS10);
    TIMSK |= _BV(OCIE1A);

    // enable interrupts
    sei();

    // fill ram
    uint32_t address = 0;
    puts("Zeroing ram");
    for (address = 0; address < 0x20000; address += sizeof dram_buffer) {
        dram_write(dram_buffer, address, sizeof dram_buffer);
    }

    // spin forever
    puts("AVR DRAM demo - (U to xmodem upload, D to xmodem download)");
    address = 0;
    while (1) {
        switch (getbyte0()) {
            case 'U':
                // perform upload
                puts("performing xmodem upload");
                dram_upload_xmodem();
                break;
            case 'D':
                // perform download
                puts("performing xmodem download");
                dram_download_xmodem();
                break;
            default:
                puts("unknown command");
                break;
        }
    }
    return 0;
}
