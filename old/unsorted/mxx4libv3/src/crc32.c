/* This is the crc32 generator */

#include <crc32.h>

uint32_t generate_crc32 (uint32_t crc, unsigned char *buffer, int length) { 
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
