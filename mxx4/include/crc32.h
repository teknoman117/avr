/* This header provides CRC32 generation */

#include <stdint.h>

/**
 * This function generates a CRC32 from a data set
 * 
 * Here are some benchmarks
 * The following ran on an ATmega644 @ 11.0592 MHz
 * 1 byte in 0.023149 milliseconds (256 clock cycles)
 * 2 bytes in 0.046297 milliseconds (512 clock cycles)
 * 4 bytes in 0.081020 milliseconds (896 clock cycles)
 * 8 bytes in 0.150466 milliseconds (1664 clock cycles)
 * 16 bytes in 0.295144 milliseconds (3264 clock cycles)
 * 32 bytes in 0.590288 milliseconds (6528 clock cycles)
 * 64 bytes in 1.163216 milliseconds (12864 clock cycles)      
 * 128 bytes in 2.320644 milliseconds (25664 clock cycles)     
 * 256 bytes in 4.606565 milliseconds (50944 clock cycles)     
 * 512 bytes in 9.189981 milliseconds (101632 clock cycles)    
 * 1024 bytes in 18.385750 milliseconds (203328 clock cycles)
 *
 * The foloowing ran on an ATmega644 @ 20 MHz (Runs a little better than 10 us per byte on large sets)
 * 1 byte in 0.016000 milliseconds (320 clock cycles)
 * 2 bytes in 0.025600 milliseconds (512 clock cycles)
 * 4 bytes in 0.044800 milliseconds (896 clock cycles)
 * 8 bytes in 0.086400 milliseconds (1728 clock cycles)
 * 16 bytes in 0.163200 milliseconds (3264 clock cycles)
 * 32 bytes in 0.323200 milliseconds (6464 clock cycles)
 * 64 bytes in 0.643200 milliseconds (12864 clock cycles)
 * 128 bytes in 1.283200 milliseconds (25664 clock cycles)
 * 256 bytes in 2.547200 milliseconds (50944 clock cycles)
 * 512 bytes in 5.084800 milliseconds (101696 clock cycles)
 * 1024 bytes in 10.166400 milliseconds (203328 clock cycles)
 *
 * @param crc the CRC seed value (i generally use 0x01234567)
 * @param buffer the data set to use
 * @param the length of the data being processed
 * @return the CRC32 of the input data
 */

uint32_t generate_crc32 (uint32_t crc, unsigned char *buffer, int length);
