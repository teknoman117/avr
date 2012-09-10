#define F_CPU 11059200UL
#include <util/delay.h>
#include "i2c.h"

int main () {
	uint8_t reg0_value;
	i2c_init();
	i2c_start(0xE0);
	i2c_write(0x00);
	i2c_rep_start(0xE0);
	reg0_value = i2c_readNak();
	i2c_stop();
    return 0;
}
