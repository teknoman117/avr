#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

// PORTA[3:0] DATA
// PORTE[0] A[8]
// PORTC[7:0] A[7:0]

#define ADDRESS PORTC
#define A8      _BV(PORTE0)

#define N_CAS   _BV(PORTD4)
#define N_OE    _BV(PORTD5)
#define N_WE    _BV(PORTD6)
#define N_RAS   _BV(PORTD7)

// DRAM control signal helpers
#define RAS_ASSERT \
    PORTD &= ~N_RAS;

#define RAS_DEASSERT \
    PORTD |= N_RAS;

#define CAS_ASSERT \
    PORTD &= ~N_CAS;

#define CAS_DEASSERT \
    PORTD |= N_CAS;

#define WE_ASSERT \
    PORTD &= ~N_WE;

#define WE_DEASSERT \
    PORTD |= N_WE;

#define OE_ASSERT \
    PORTD &= ~N_OE;

#define OE_DEASSERT \
    PORTD |= N_OE;

static volatile uint8_t dram_missed_refreshes = 0;
static volatile uint8_t dram_busy = 0;

// CAS before RAS refresh cycle
// macro to force inlining
#define dram_refresh() \
    CAS_ASSERT; \
    RAS_ASSERT; \
    CAS_DEASSERT; \
    RAS_DEASSERT;

// dram refresh Interrupt
ISR(TIMER3_COMPA_vect) {
    if (!dram_busy) {
        // if the memory is not busy, perform a refresh cycle now
        dram_refresh();
    } else {
        // otherwise, increase the "owed" refresh count
        dram_missed_refreshes++;
    }
}

// dram read
void dram_read(uint8_t* buffer, uint32_t address, uint16_t size)  {
    // lock memory access
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        dram_busy = 1;
    }

    // compute starting addresses
    uint16_t row = (address >> 8) & 0xffff;
    uint8_t col = address & 0xff;

    // fpm read
    OE_ASSERT;
    while (size) {
        // open page
        PORTE = (row & 0x100) ? (PORTE | A8) : (PORTE & ~A8);
        ADDRESS = row;
        RAS_ASSERT;

        // read page
        do {
            // read high nibble
            PORTE = (col & 0x80) ? (PORTE | A8) : (PORTE & ~A8);
            ADDRESS = (col << 1) | 0x00;
            CAS_ASSERT;
            // io synchronization
            _NOP();
            *buffer = PINA << 4;
            CAS_DEASSERT;

            // read low nibble
            ADDRESS = (col << 1) | 0x01;
            CAS_ASSERT;
            // io synchronization
            _NOP();
            *(buffer++) |= PINA & 0x0f;
            CAS_DEASSERT;
        } while (--size && ++col);

        // close page
        RAS_DEASSERT;
        row++;
    }
    OE_DEASSERT;

    // issue refreshes until we've made up our debt
    uint8_t missed = 0;
    ATOMIC_BLOCK(ATOMIC_FORCEON) { missed = dram_missed_refreshes; }
    while (missed) {
        dram_refresh();
        ATOMIC_BLOCK(ATOMIC_FORCEON) { missed = --dram_missed_refreshes; }
    }

    // unlock memory access
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        dram_busy = 0;
    }
}

// dram write
void dram_write(uint8_t* buffer, uint32_t address, uint16_t size) {
    // lock memory access
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        dram_busy = 1;
    }

    // compute starting addresses
    uint16_t row = (address >> 8) & 0xffff;
    uint8_t col = address & 0xff;

    // fpm read
    DDRA = 0xff;
    while (size) {
        // open page
        ADDRESS = row;
        PORTE = (row & 0x100) ? (PORTE | A8) : (PORTE & ~A8);
        RAS_ASSERT;

        // write page
        do {
            // write high nibble
            PORTE = (col & 0x80) ? (PORTE | A8) : (PORTE & ~A8);
            ADDRESS = (col << 1) | 0x00;
            PORTA = *buffer >> 4;
            WE_ASSERT;
            CAS_ASSERT;
            WE_DEASSERT;
            CAS_DEASSERT;

            // read low nibble
            ADDRESS = (col << 1) | 0x01;
            PORTA = *(buffer++) & 0x0f;
            WE_ASSERT;
            CAS_ASSERT;
            WE_DEASSERT;
            CAS_DEASSERT;
        } while (--size && ++col);

        // close page
        RAS_DEASSERT;
        row++;
    }
    DDRA = 0x00;
    PORTA = 0x00;

    // issue refreshes until we've made up our debt
    uint8_t missed = 0;
    ATOMIC_BLOCK(ATOMIC_FORCEON) { missed = dram_missed_refreshes; }
    while (missed) {
        dram_refresh();
        ATOMIC_BLOCK(ATOMIC_FORCEON) { missed = --dram_missed_refreshes; }
    }

    // unlock memory access
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        dram_busy = 0;
    }    
}

void setup_dram() {
    // setup ram i/o ports
    // D[3:0]
    DDRA = 0x00;
    // A[8]
    DDRE |= _BV(PORTE0);
    PORTE &= ~_BV(PORTE0);
    // A[7:0]
    DDRC = 0xFF;
    PORTC = 0x00;
    // Control Signals
    DDRD |= N_CAS | N_RAS | N_OE | N_WE;
    PORTD |= N_CAS | N_RAS | N_OE | N_WE;

    // setup dram refresh timer
    // clocks = (F_CPU * refresh period in ms) / (1000 ms per second * refreshes per period)
    OCR3A = (F_CPU * 64UL) / (1000UL * 512UL);
    TCCR3B = _BV(WGM32) | _BV(CS30);
    ETIMSK |= _BV(OCIE3A);
}