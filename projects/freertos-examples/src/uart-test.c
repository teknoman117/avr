// FreeRTOS Test
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

static QueueHandle_t TransmitQueue, ReceiveQueue;
static SemaphoreHandle_t TransmitSemaphore, ReceiveSemaphore;

// read a byte
#ifdef USART0_RX_vect
ISR(USART0_RX_vect) {
#else
ISR(USART_RX_vect) {
#endif
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t cIn = UDR0;

    xQueueSendFromISR(ReceiveQueue, &cIn, &xHigherPriorityTaskWoken);
    if(xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

// send a byte
#ifdef USART0_UDRE_vect
ISR(USART0_UDRE_vect) {
#else
ISR(USART_UDRE_vect) {
#endif
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t cOut;

    if (xQueueReceiveFromISR(TransmitQueue, &cOut, &xHigherPriorityTaskWoken)) {
        UDR0 = cOut;
    } else {
        // Disable interrupt if the transmit queue is empty
        UCSR0B &= ~_BV(UDRIE0);
    }

    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static int _putchar(char c, FILE *stream) {
    while (xQueueSendToBack(TransmitQueue, &c, 1) != pdPASS);
    UCSR0B |= _BV(UDRIE0);
    return 0;
}

static int _getchar(FILE *stream) {
    uint8_t cIn;
    while (xQueueReceive(ReceiveQueue, &cIn, 1) != pdPASS);
    return cIn;
}

static FILE uart0_stream =
        FDEV_SETUP_STREAM(_putchar, _getchar, _FDEV_SETUP_RW);

void setupUART(void) {
    TransmitQueue = xQueueCreate(128, 1);
    ReceiveQueue = xQueueCreate(128, 1);
    TransmitSemaphore = xSemaphoreCreateMutex();
    ReceiveSemaphore = xSemaphoreCreateMutex();
    stdout = stdin = stderr = &uart0_stream;
    
    // Configure 115.2k 8n1
    UBRR0L = 16;
    UBRR0H = 0;
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
    UCSR0A = _BV(U2X0);
}

void TaskSend(void *pvParameters) {
    TickType_t rate = *(TickType_t *) pvParameters;
    TickType_t delay = rate / portTICK_PERIOD_MS;
    TickType_t wakeUpTime = xTaskGetTickCount();

    while (1) {
        if (xSemaphoreTake(TransmitSemaphore, 10) == pdTRUE) {
            printf_P(PSTR("[%u] This task sends every %u ms\r\n"), xTaskGetTickCount(), rate);
            xSemaphoreGive(TransmitSemaphore);
            vTaskDelayUntil(&wakeUpTime, delay);
        }
    }
}

void vApplicationMallocFailedHook(void) {
    while (1);
}

void vApplicationStackOverflowHook(TaskHandle_t task, char*) {
    while (1);
}

void main(void) {
    setupUART();
    sei();

    TickType_t rate1 = 333;
    TickType_t rate2 = 500;
    TickType_t rate3 = 1000;

    xTaskCreate(TaskSend, "Send1", 128, &rate1, 2, NULL);
    xTaskCreate(TaskSend, "Send2", 128, &rate2, 2, NULL);
    xTaskCreate(TaskSend, "Send3", 128, &rate3, 2, NULL);
    vTaskStartScheduler();
}