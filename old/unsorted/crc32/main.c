#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <crc32.h>
#include <stdusart0.h>
#include <timer1.h>
 

int main ( int argc, char** argv ) {
    init_stdusart0(BAUD(38400), DB8 | P_N | SB1);
    init_timer1(T1_PRESCALER_64, 0xffff);
    unsigned char data[1024];
    unsigned int i;
	double msecs;
    uint32_t ticks;
    for(i = 0; i < 1024; i++) data[i] = (unsigned char)(i & 0x00ff);
	printf("Starting CRC32 Speed Test (%ld Hz)\r\n", F_CPU);
    for(i = 1; i <= 1024; i *= 2) {
    	reset_timer1();
        generate_crc32(0x01234567, data, i);
        ticks = get_timer1();
        msecs = (64 * (double)ticks) / (F_CPU / 1000);
        printf("CRC32 Ran on %d bytes in %lf milliseconds (%ld clock cycles)\r\n", i, msecs, ticks * 64);
    }
    while(1);
    return 0;
}

    /*uint32_t crc32 = generate_crc32(0x01234567, data, 256);
    unsigned ticks = get_timer1();
    printf("The CRC32 of the data is: %x %x in %u ticks\r\n", 
                                 (uint16_t)(crc32 & 0x0000ffff), 
                                 (uint16_t)((crc32 >> 16) & 0x0000ffff), ticks);*/
