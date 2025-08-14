#include <avr/io.h>
#include <stdint.h>
#include "motor.h"
#include "M32io2.h"

void initMotors( void ) {
  TCCR1A = ( 1 << WGM10 ) | ( 1 << COM1A1 ) | ( 1 << COM1B1 );
  TCCR1B = ( 1 << CS10  ) | ( 1 << WGM12  );
  SetDir('B',0,'O');
  SetDir('B',1,'O');
  SetDir('B',2,'O');
  SetDir('B',3,'O');
  OCR1A = OCR1B = 0;
  status = STOP;
}

void forward( uint8_t power ) {
  SetPin('B',0,1);
  SetPin('B',1,0);
  SetPin('B',2,1);
  SetPin('B',3,0);
  OCR1A = OCR1B = power;
  status = FORWARD;
}

void backward( uint8_t power ) {
  SetPin('B',0,0);
  SetPin('B',1,1);
  SetPin('B',2,0);
  SetPin('B',3,1);
  OCR1A = OCR1B = power;
  status = BACKWARD;
}

void sleft( uint8_t power ) {
  SetPin('B',0,0);
  SetPin('B',1,1);
  SetPin('B',2,1);
  SetPin('B',3,0);
  OCR1A = OCR1B = power;
  status = SLEFT;
}

void sright( uint8_t power ) {
  SetPin('B',0,1);
  SetPin('B',1,0);
  SetPin('B',2,0);
  SetPin('B',3,1);
  OCR1A = OCR1B = power;
  status = SRIGHT;
}

void tleft( uint8_t power ) {
  SetPin('B',0,0);
  SetPin('B',1,0);
  SetPin('B',2,1);
  SetPin('B',3,0);
  OCR1A = OCR1B = power;
  status = TLEFT;
}

void tright( uint8_t power ) {
  SetPin('B',0,0);
  SetPin('B',1,0);
  SetPin('B',2,1);
  SetPin('B',3,0);
  OCR1A = OCR1B = power;
  status = TRIGHT;
}

void stop( uint8_t fltbrk ) {
  OCR1A = OCR1B = 0;
  if(fltbrk == 0) {
    SetPin('B',0,1);
    SetPin('B',1,0);
    SetPin('B',2,1);
    SetPin('B',3,0);
  }
  if(fltbrk == 1) {
    SetPin('B',0,0);
    SetPin('B',1,0);
    SetPin('B',2,0);
    SetPin('B',3,0); 
  }
  status = STOP;
}

uint8_t getStatus( void ) {
  return status;
}
 