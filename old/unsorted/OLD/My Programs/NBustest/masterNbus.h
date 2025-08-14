//Includes for the Nbus unit
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include "avr40pio.h" 
//Data Declarations for the Nbus unit
unsigned int cnt;
unsigned long counter;
//Function prototypes for the Nbus unit
void init_nBus (void);                //Initalize the bus
void tx_nbyte (unsigned char daByte); //Transmit 8 bits
void tx_nword (unsigned int daWord);  //Transmit 16 bits
unsigned char rx_nbyte (unsigned char daByte); //Receive 8 bits
unsigned int rx_nword (unsigned int daWord); //Receive 16 bits

