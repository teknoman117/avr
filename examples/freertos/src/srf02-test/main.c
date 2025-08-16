#include "FreeRTOS.h"
#include "task.h"

#include "console.h"
#include "i2c.h"
#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <stdio.h>

void vApplicationMallocFailedHook(void) {
    while (1);
}

void vApplicationStackOverflowHook(TaskHandle_t task, char*) {
    while (1);
}

void setupLED(void) {
    DDRB |= _BV(PORTB5);
    PORTB &= ~_BV(PORTB5);
}

void setLEDOn(void) {
    PORTB |= _BV(PORTB5);
}

void setLEDOff(void) {
    PORTB &= ~_BV(PORTB5);
}

void TaskHeartbeat(void *pvParameters) {
    TickType_t wakeUpTime = xTaskGetTickCount();
    while (1) {
        setLEDOn();
        CONSOLE(printf_P(PSTR("LED On\r\n")));
        vTaskDelayUntil(&wakeUpTime, 500 / portTICK_PERIOD_MS);
        setLEDOff();
        CONSOLE(printf_P(PSTR("LED Off\r\n")));
        vTaskDelayUntil(&wakeUpTime, 500 / portTICK_PERIOD_MS);
    }
}

void TaskSonars(void *pvParameters) {
    uint8_t address = * (uint8_t*) pvParameters;

    // get software revision command
    uint8_t commandRegister = 0;
    uint8_t softwareRevision = 0;

    i2c_msg_t getSoftwareRevisionMsgs[] = {
        {
            .data = &commandRegister,
            .len = sizeof commandRegister - 1,
            .address = address,
        },
        {
            .data = &softwareRevision,
            .len = sizeof softwareRevision - 1,
            .address = address + 1,
        },
    };

    i2c_transfer_t getSoftwareRevision = {
        .notify = xTaskGetCurrentTaskHandle(),
        .msgs = getSoftwareRevisionMsgs,
        .n = 1,
        .error = 0,
    };

    // begin ranging command
    uint8_t beginRangingCommand[2] = {0, 0x51};
    uint8_t rangingResultRegister = 2;
    uint8_t rangingResult[2] = {0, 0};

    i2c_msg_t startRangingMsgs[] = {
        {
            .data = beginRangingCommand,
            .len = sizeof beginRangingCommand - 1,
            .address = address,
        }
    };

    i2c_transfer_t beginRanging = {
        .notify = xTaskGetCurrentTaskHandle(),
        .msgs = startRangingMsgs,
        .n = 0,
        .error = 0,
    };

    i2c_msg_t getRangingResultMsgs[] = {
        {
            .data = &rangingResultRegister,
            .len = sizeof rangingResultRegister - 1,
            .address = address,
        },
        {
            .data = rangingResult,
            .len = sizeof rangingResult - 1,
            .address = address + 1,
        }
    };

    i2c_transfer_t getRangingResult = {
        .notify = xTaskGetCurrentTaskHandle(),
        .msgs = getRangingResultMsgs,
        .n = 1,
        .error = 0,
    };

    // get the software revision
    vTaskDelay(100 / portTICK_PERIOD_MS);
    queueI2CTransfer(&getSoftwareRevision);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    if (!getSoftwareRevision.error) {
        CONSOLE(printf_P(PSTR("sonar @ %02xh software revision = %u\r\n"), address, softwareRevision));
    } else {
        CONSOLE(printf_P(PSTR("sonar @ %02xh not found\r\n"), address));
        vTaskSuspend(xTaskGetCurrentTaskHandle());
    }

    TickType_t wakeUpTime = xTaskGetTickCount();
    while (1) {
        // submit get transfer
        do {
            queueI2CTransfer(&beginRanging);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        } while (beginRanging.error);

        // get the range
        do {
            queueI2CTransfer(&getRangingResult);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        } while (getRangingResult.error);

        uint16_t range = rangingResult[0];
        range <<= 8;
        range |= rangingResult[1];

        // display range
        CONSOLE(printf_P(PSTR("sonar @ %02xh = %u cm\r\n"), address, range));

        // wait for ringing to stop
        vTaskDelayUntil(&wakeUpTime, 66 / portTICK_PERIOD_MS);
    }
}

void main(void) {
    setupLED();
    setupI2C();
    setupUART0();
    setupConsole();

    uint8_t sonar1 = 0xE0;
    uint8_t sonar2 = 0xE2;
    uint8_t sonar3 = 0xE4;

    xTaskCreate(TaskHeartbeat, "Blink", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(TaskSonars, "Sonar1", configMINIMAL_STACK_SIZE, &sonar1, 2, NULL);
    xTaskCreate(TaskSonars, "Sonar2", configMINIMAL_STACK_SIZE, &sonar2, 2, NULL);
    xTaskCreate(TaskSonars, "Sonar3", configMINIMAL_STACK_SIZE, &sonar3, 2, NULL);
    vTaskStartScheduler();
}
