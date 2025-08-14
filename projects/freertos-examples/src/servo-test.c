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

void TaskServo1(void *pvParameters) {
    TickType_t wakeUpTime = xTaskGetTickCount();
    while (1) {
        // 0.5 ms
        OCR1A = 8000;
        vTaskDelayUntil(&wakeUpTime, 2500 / portTICK_PERIOD_MS);

        // 2.5 ms
        OCR1A = 40000;
        vTaskDelayUntil(&wakeUpTime, 2500 / portTICK_PERIOD_MS);
    }
}

void TaskESC1(void *pvParameters) {
    TickType_t wakeUpTime = xTaskGetTickCount();
    while (1) {
        // Slowly move to full forward (2.0 ms)
        for (uint16_t i = 24000; i < 32000; i += 100) {
            OCR1B = i;
            vTaskDelay(1);
        }

        // Slowly move to full reverse (1.0 ms)
        for (uint16_t i = 32000; i >= 16000; i -= 100) {
            OCR1B = i;
            vTaskDelay(1);
        }

        // Slowly move to neutral (1.5 ms)
        for (uint16_t i = 16000; i < 24000; i += 100) {
            OCR1B = i;
            vTaskDelay(1);
        }

        // Delay for 5 seconds
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void setupLED(void) {
    // Set LED pin to an output and default to off
    DDRB |= _BV(PORTB5);
    PORTB &= ~_BV(PORTB5);
}

void setupServos(void) {
    // Configure Fast-PWM to overflow at 333 Hz.
    // At 16 MHz, gives 32000 steps across a 180 degree range
    // 1 ms = 16000
    DDRB |= _BV(PORTB2) | _BV(PORTB1);
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
    ICR1 = (F_CPU / 333) - 1;

    // center both servos
    // 1.5 ms
    OCR1A = 24000;
    OCR1B = 24000;
}

void main(void) {
    setupLED();
    setupServos();

    xTaskCreate(TaskBlink1, "Blink", 128, NULL, 2, NULL);
    xTaskCreate(TaskServo1, "Servo", 128, NULL, 2, NULL);
    xTaskCreate(TaskESC1, "ESC", 128, NULL, 2, NULL);
    vTaskStartScheduler();
}