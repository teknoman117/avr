#include <avr/io.h>
#include <stdint.h>
#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define SLEFT 3
#define SRIGHT 4
#define TLEFT 5
#define TRIGHT 6

uint8_t status;

void initMotors( void );
void forward( uint8_t power );
void backward( uint8_t power );
void sleft( uint8_t power );
void sright( uint8_t power );
void tleft( uint8_t power );
void tright( uint8_t power );
void stop( uint8_t fltbrk );
uint8_t getStatus( void );