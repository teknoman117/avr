#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static QueueHandle_t TransmitQueue;
static QueueHandle_t ReceiveQueue;

#ifdef USART0_RX_vect
ISR(USART0_RX_vect) {
#else
ISR(USART_RX_vect) {
#endif
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t b = UDR0;

    xQueueSendFromISR(ReceiveQueue, &b, &xHigherPriorityTaskWoken);
    if(xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

#ifdef USART0_UDRE_vect
ISR(USART0_UDRE_vect) {
#else
ISR(USART_UDRE_vect) {
#endif
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t b;

    if (xQueueReceiveFromISR(TransmitQueue, &b, &xHigherPriorityTaskWoken)) {
        UDR0 = b;
    } else {
        // Disable interrupt if the transmit queue is empty
        UCSR0B &= ~_BV(UDRIE0);
    }

    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

void setupUART0(void) {
    TransmitQueue = xQueueCreate(TXBUFFER_SIZE, 1);
    ReceiveQueue = xQueueCreate(RXBUFFER_SIZE, 1);
    
    // Configure 8n1 with receive interrupt
    UBRR0L = (F_CPU / (8UL * BAUD)) - 1;
    UBRR0H = 0;
    UCSR0A = _BV(U2X0);
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
    
}

void putByteUART0(uint8_t b) {
    while (xQueueSendToBack(TransmitQueue, &b, 1) != pdPASS);
    UCSR0B |= _BV(UDRIE0);
}

uint8_t getByteUART0(void) {
    uint8_t b;
    while (xQueueReceive(ReceiveQueue, &b, 1) != pdPASS);
    return b;
}
