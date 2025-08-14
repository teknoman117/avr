#include <stdio.h>
#include <util/delay.h>

#include "fakeuart.h"

// Main method
int main () {
    // Initalize the software uart
    initFakeUart();
    
    // Open std connections against our software uart
    fdevopen((void*)txbs, (void*)rxbs);	       //open the stdin and stdout connections for stdio	
    
    // Begin the example
    printf("AVR Software UART Test\r\n");
    while (1) {
        // Get characters in the buffer
        unsigned char c = rxbs();
        printf("%u bytes in buffer (char = %c (%u))\r\n", bytesInBufferS(), c, c);
        _delay_ms(1000);
    }
    return 0;
}
