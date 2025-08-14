// FreeRTOS Test
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stream_buffer.h"

static SemaphoreHandle_t TransmitSemaphore, ReceiveSemaphore;
static StreamBufferHandle_t TransmitBuffer, ReceiveBuffer;
static StreamBufferHandle_t Buffer;

// read a byte
#ifdef USART0_RX_vect
ISR(USART0_RX_vect) {
#else
ISR(USART_RX_vect) {
#endif
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t cIn = UDR0;

    xStreamBufferSendFromISR(ReceiveBuffer, &cIn, 1, &xHigherPriorityTaskWoken);
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

    if (xStreamBufferReceiveFromISR(TransmitBuffer, &cOut, 1, &xHigherPriorityTaskWoken)) {
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
    while (!xStreamBufferSend(TransmitBuffer, &c, 1, 0));
    UCSR0B |= _BV(UDRIE0);
    return 0;
}

static int _getchar(FILE *stream) {
    uint8_t cIn;
    while (!xStreamBufferReceive(ReceiveBuffer, &cIn, 1, 0));
    return cIn;
}

static FILE uart0_stream =
        FDEV_SETUP_STREAM(_putchar, _getchar, _FDEV_SETUP_RW);

void setupUART(void) {
    TransmitBuffer = xStreamBufferCreate(128, 1);
    ReceiveBuffer = xStreamBufferCreate(128, 1);
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
        //if (xSemaphoreTake(TransmitSemaphore, 10) == pdTRUE) {
            taskENTER_CRITICAL();
            printf_P(PSTR("[%u] This task sends every %u ms\r\n"), xTaskGetTickCount(), rate);
            taskEXIT_CRITICAL();
            //xSemaphoreGive(TransmitSemaphore);
            vTaskDelayUntil(&wakeUpTime, delay);
        //}
    }
}

void TaskWrite(void *pvParameters) {
    while (1) {
        int num = 0;
        if (xStreamBufferReceive(Buffer, &num, sizeof(num), 1000 / portTICK_PERIOD_MS)
                == sizeof(num)) {
            //if (xSemaphoreTake(TransmitSemaphore, 10) == pdTRUE) {
                taskENTER_CRITICAL();
                printf_P(PSTR("[%u] Received: %d\r\n"), xTaskGetTickCount(), num);
                taskEXIT_CRITICAL();
                //xSemaphoreGive(TransmitSemaphore);
            //}
        }
    }
}

void TaskGenerate(void *pvParameters) {
    TickType_t wakeUpTime = xTaskGetTickCount();

    while (1) {
        int num = rand();
        xStreamBufferSend(Buffer, &num, sizeof(num), 1);
        vTaskDelayUntil(&wakeUpTime, 1000 / portTICK_PERIOD_MS);
    }
}

void vApplicationMallocFailedHook(void) {
    taskENTER_CRITICAL();
    while (1) {
        PORTB |= _BV(PORTB5);
        _delay_ms(100);
        PORTB &= ~_BV(PORTB5);
        _delay_ms(100);
        PORTB |= _BV(PORTB5);
        _delay_ms(100);
        PORTB &= ~_BV(PORTB5);
        _delay_ms(1000);
    }
    taskEXIT_CRITICAL();
}

void vApplicationStackOverflowHook(TaskHandle_t task, char*) {
    taskENTER_CRITICAL();
    while (1) {
        PORTB |= _BV(PORTB5);
        _delay_ms(100);
        PORTB &= ~_BV(PORTB5);
        _delay_ms(1000);
    }
    taskEXIT_CRITICAL();
}

void main(void) {
    DDRB |= _BV(PORTB5);
    setupUART();
    sei();

    Buffer = xStreamBufferCreate(128, sizeof(int));

    TickType_t rate1 = 333;
    TickType_t rate2 = 500;
    TickType_t rate3 = 1000;

    xTaskCreate(TaskSend, "Send1", 96, &rate1, 2, NULL);
    xTaskCreate(TaskSend, "Send2", 96, &rate2, 2, NULL);
    xTaskCreate(TaskSend, "Send3", 96, &rate3, 2, NULL);
    xTaskCreate(TaskWrite, "Write", 96, NULL, 2, NULL);
    xTaskCreate(TaskGenerate, "Generate", 96, NULL, 2, NULL);
    vTaskStartScheduler();
}