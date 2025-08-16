// FreeRTOS Test
#include <avr/io.h>

#include "FreeRTOS.h"
#include "task.h"

void vApplicationMallocFailedHook(void) {
    while (1);
}

void vApplicationStackOverflowHook(TaskHandle_t task, char*) {
    while (1);
}

void TaskBlink1(void *pvParameters) {
    // Blink every second
    TickType_t wakeUpTime = xTaskGetTickCount();
    while (1) {
        PORTB |= _BV(PORTB5);
        vTaskDelayUntil(&wakeUpTime, 500 / portTICK_PERIOD_MS);
        PORTB &= ~_BV(PORTB5);
        vTaskDelayUntil(&wakeUpTime, 500 / portTICK_PERIOD_MS);
    }
}

void setupLED(void) {
    // Set LED pin to an output and default to off
    DDRB |= _BV(PORTB5);
    PORTB &= ~_BV(PORTB5);
}

void main(void) {
    setupLED();

    xTaskCreate(TaskBlink1, "Blink", 128, NULL, 2, NULL);
    vTaskStartScheduler();
}