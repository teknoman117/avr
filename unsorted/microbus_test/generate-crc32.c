/* This is the crc32 generator */

#include <stdio.h>
#include <string.h>

unsigned int generate_crc32 (unsigned int crc, unsigned char *buffer, int length) { 
    int i, j; 
    for (i=0; i<length; i++) { 
        crc = crc ^ *(buffer++); 
        for (j=0; j<8; j++) { 
		    if (crc & 1) 
		        crc = (crc>>1) ^ 0xEDB88320 ; 
		    else 
		        crc = crc >>1 ; 
        } 
    } 
    return crc; 
}

int main (int argc, char **argv) {
    unsigned int crc32 = generate_crc32(0x01234567, argv[1], strlen(argv[1]));
    printf("CRC32: %ld\n", crc32) ;
    printf("  LSB: %u\n", (unsigned)(crc32 & 0x000000FF));
    printf("       %u\n", (unsigned)((crc32 >> 8) & 0x000000FF));
    printf("       %u\n", (unsigned)((crc32 >> 16) & 0x000000FF));
    printf("  MSB: %u\n", (unsigned)((crc32 >> 24) & 0x000000FF));
    return 0;
}
