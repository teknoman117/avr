#ifndef DRAM_H_
#define DRAM_H_

#include <stdint.h>

void setup_dram();

void dram_read(uint8_t* buffer, uint32_t address, uint16_t size);

void dram_write(uint8_t* buffer, uint32_t address, uint16_t size);

#endif /* DRAM_H_ */