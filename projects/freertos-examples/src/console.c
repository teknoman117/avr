#include "console.h"
#include "uart.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "semphr.h"

static SemaphoreHandle_t Semaphore;

static int _putchar(char c, FILE *stream) {
    putByteUART0(c);
    return 0;
}

static int _getchar(FILE *stream) {
    return getByteUART0();
}

static FILE uart_stream =
        FDEV_SETUP_STREAM(_putchar, _getchar, _FDEV_SETUP_RW);

void setupConsole(void) {
    Semaphore = xSemaphoreCreateMutex();
    stdout = stdin = stderr = &uart_stream;
}

void lockConsole(void) {
    while (!xSemaphoreTake(Semaphore, 1));
}

void unlockConsole(void) {
    xSemaphoreGive(Semaphore);
}
