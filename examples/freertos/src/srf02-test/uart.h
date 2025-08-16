#pragma once

#include <stdint.h>

extern void setupUART0(void);
extern void putByteUART0(uint8_t b);
extern uint8_t getByteUART0(void);