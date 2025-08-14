#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer1PWM.h"

void initPWM( void ) {
  initPWM08();
}
void initPWM08( void ) {
  TCCR1A = ( 1 << WGM10 ) | ( 1 << COM1A1 ) | ( 1 << COM1B1 );
  TCCR1B = ( 1 << CS10  ) | ( 1 << WGM12  );
}
void initPWM09( void ) {
  TCCR1A = ( 1 << WGM11 ) | ( 1 << COM1A1 ) | ( 1 << COM1B1 );
  TCCR1B = ( 1 << CS10  ) | ( 1 << WGM12  );
}
void initPWM10( void ) {
  TCCR1A = ( 1 << WGM10 ) | ( 1 << WGM11 ) | ( 1 << COM1A1 ) | ( 1 << COM1B1 );
  TCCR1B = ( 1 << CS10  ) | ( 1 << WGM12  );
}
void setCHA( uint16_t valueA )
{
  setCHA08(valueA);
}

void setCHB( uint16_t valueB )
{
  setCHB08(valueB);
}
void setCHA08( uint16_t valueA ) {
  OCR1A = valueA;
}
void setCHB08( uint16_t valueB ) {
  OCR1B = valueB;
}
void setCHA09( uint16_t valueA) {
  OCR1AL = valueA & 0xFF;
  OCR1AH = ((valueA >> 8) & 0x01);
}
void setCHB09( uint16_t valueB) {
  OCR1BL = valueB & 0xFF;
  OCR1BH = ((valueB >> 8) & 0x01);
}
void setCHA10( uint16_t valueA) {
  OCR1AL = valueA & 0xFF;
  OCR1AH = ((valueA >> 8) & 0x03);
}
void setCHB10( uint16_t valueB) {
  OCR1AL = valueB & 0xFF;
  OCR1BH = ((valueB >> 8) & 0x03);
}
    