#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct i2c_msg {
    uint8_t *data;
    uint8_t len;
    uint8_t address;
} i2c_msg_t ;

typedef struct _i2c_transfer {
    void *notify;
    i2c_msg_t *msgs;
    uint8_t n;
    uint8_t error;
} i2c_transfer_t ;

extern void setupI2C(void);
extern void queueI2CTransfer(i2c_transfer_t *transfer);