#include "i2c.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <avr/io.h>
#include <avr/interrupt.h>

static QueueHandle_t uTransferQueue;

static i2c_transfer_t *cTransfer = NULL;
static i2c_msg_t *cMsg;
static uint8_t cN;
static uint8_t cI;

ISR(TWI_vect) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t complete = 0;

    uint8_t status = (TWSR >> 3) & 0x1f;
    switch (status) {
        case 1:
            // START
            /* FALLTHROUGH */
        case 2:
            // REPEATED-START
            if (!cTransfer) {
                // load transfer object
                if (!xQueueReceiveFromISR(uTransferQueue, &cTransfer, &xHigherPriorityTaskWoken)) {
                    complete = 1;
                    break;
                }
                cTransfer->error = 0;
                cN = 0;
            }

            // load slave address
            cMsg = &cTransfer->msgs[cN];
            cI = 0;
            TWDR = cMsg->address;
            TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);
            break;

        case 3:
            // SLA+W transmitted, ACK received
            /* FALLTHROUGH */
        case 5:
            // Data transmitted, ACK received
            if (cMsg) {
                TWDR = cMsg->data[cI];
                TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);

                if (cI++ == cMsg->len)
                    cMsg = NULL;
            } else {
                complete = 1;
            }
            break;

        case 11:
            // Data received, NACK transmitted
            cMsg->data[cI++] = TWDR;
            complete = 1;
            break;

        case 10:
            // Data received, ACK transmitted
            cMsg->data[cI++] = TWDR;
            /* FALLTHROUGH */
        case 8:
            // SLA+R transmitted, ACK received
            if (cI != cMsg->len) {
                TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
            } else {
                TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);
            }
            break;

            // Errors
        case 4:
            // SLA+W transmitted, NACK received
            /* FALLTHROUGH */
        case 9:
            // SLA+R transmitted, NACK received
            /* FALLTHROUGH */
        case 6:
            // Data transmitted, NACK received
            /* FALLTHROUGH */
        case 7:
            // Arbitration loss
            /* FALLTHROUGH */
        default:
            cTransfer->error = 1;
            complete = 1;
            break;
    }

    if (complete) {
        if (!cTransfer || cTransfer->error || cTransfer->n == cN) {
            // notify task which submitted transfer
            if (cTransfer && cTransfer->notify) {
                vTaskNotifyGiveFromISR(cTransfer->notify, &xHigherPriorityTaskWoken);
            }

            // start next transfer if present
            cTransfer = NULL;
            if (!xQueueIsQueueEmptyFromISR(uTransferQueue)) {
                // send stop then start
                TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWSTO) | _BV(TWEN) | _BV(TWIE);
            } else {
                // send stop
                TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
            }
        } else {
            cN++;

            // send start
            TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWIE);
        }
    }

    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

void setupI2C(void) {
    uTransferQueue = xQueueCreate(16, sizeof(i2c_transfer_t*));
    TWBR = (F_CPU / (F_I2C * 8)) - 2;
    TWCR = _BV(TWINT) | _BV(TWEN);
}

void queueI2CTransfer(i2c_transfer_t *transfer) {
    while (!xQueueSend(uTransferQueue, &transfer, 1));

    // kick off i2c if idle
    taskENTER_CRITICAL();
    if (!(TWCR & _BV(TWIE))) {
        cTransfer = NULL;
        TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWIE);
    }
    taskEXIT_CRITICAL();
}
