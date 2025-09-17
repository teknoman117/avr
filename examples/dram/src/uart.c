#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <stdint.h>

// Baudrate defined by Makefile
#define BAUD_UBRR ((F_CPU / (16UL * BAUD)) - 1)
#define BAUD_UBRRL (BAUD_UBRR & 0xff)
#define BAUD_UBRRH ((BAUD_UBRR >> 8) & 0xff)

#define RX_BUFFER_SIZE 32
#define TX_BUFFER_SIZE 32
#define RX_BUFFER_MASK (RX_BUFFER_SIZE - 1)
#define TX_BUFFER_MASK (TX_BUFFER_SIZE - 1)

static char uart0_rx_buffer[RX_BUFFER_SIZE];
static char uart0_tx_buffer[TX_BUFFER_SIZE];
static volatile uint8_t uart0_rx_buffer_rp = 0;
static volatile uint8_t uart0_rx_buffer_wp = 0;
static volatile uint8_t uart0_tx_buffer_rp = 0;
static volatile uint8_t uart0_tx_buffer_wp = 0;

// read a byte
ISR(USART0_RXC_vect) {
    uart0_rx_buffer[uart0_rx_buffer_wp++ & RX_BUFFER_MASK] = UDR0;
}

// send a byte
ISR(USART0_UDRE_vect) {
    UDR0 = uart0_tx_buffer[uart0_tx_buffer_rp++ & TX_BUFFER_MASK];
    // if no more data is available, disable the interrupt
    if (uart0_tx_buffer_rp == uart0_tx_buffer_wp) {
        UCSR0B &= ~_BV(UDRIE0);
    }
}

static uint8_t uart0_tx_pending() {
    uint8_t pending;
    if (uart0_tx_buffer_rp < uart0_tx_buffer_wp) {
        pending = uart0_tx_buffer_wp - uart0_tx_buffer_rp;
    } else {
        pending = (255 - uart0_tx_buffer_rp) + uart0_tx_buffer_wp + 1;
    }
    return pending;
}

static int uart0_putchar(char c, FILE* stream) {
    (void) stream;
    // add a carriage return to form CRLF
    if (c == '\n') {
        uart0_putchar('\r', stream);
    }
    putbyte0(c);
    return 0;
}

static int uart0_getchar(FILE* stream) {
    (void) stream;
    return getbyte0();
}

static FILE uart0_stream = FDEV_SETUP_STREAM(uart0_putchar, uart0_getchar, _FDEV_SETUP_RW);

void setup_uart0() {
    UBRR0L = BAUD_UBRRL;
    UBRR0H = BAUD_UBRRH;
    UCSR0C = _BV(URSEL0) | _BV(UCSZ01) | _BV(UCSZ00);
    UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
    stdout = stdin = stderr = &uart0_stream;
}

uint8_t hasbyte0() {
    return uart0_rx_buffer_rp != uart0_rx_buffer_wp;
}

uint8_t getbyte0() {
    while (!hasbyte0());
    return uart0_rx_buffer[uart0_rx_buffer_rp++ & RX_BUFFER_MASK];
}

void putbyte0(uint8_t b) {
    while (uart0_tx_pending() > TX_BUFFER_MASK);
    uart0_tx_buffer[uart0_tx_buffer_wp++ & TX_BUFFER_MASK] = b;
    UCSR0B |= _BV(UDRIE0);
}