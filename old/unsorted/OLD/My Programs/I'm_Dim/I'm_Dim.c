#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define OC1 PD5
#define DDROC DDRD
#define OCR OCR1A
#define PWM10 WGM10
#define PWM11 WGM11
#if defined(COM11)
#  define XCOM11 COM11
#elif defined(COM1A1)
#  define XCOM11 COM1A1
#else
#  error "need either COM1A1 or COM11"
#endif
void
ioinit (void) /* Note [5] */
{
    /* tmr1 is 10-bit PWM */
    TCCR1A = _BV (PWM10) | _BV (PWM11) | _BV (XCOM11);

    /* tmr1 running on full MCU clock */
    TCCR1B = _BV (CS10);

    /* set PWM value to 0 */
    OCR = 0;

    /* enable OC1 and PB2 as output */
    DDROC = _BV (OC1);

    timer_enable_int (_BV (TOIE1));

    /* enable interrupts */
    sei ();
}
 
void main (void)
{
  ioinit ();
  int pwm;
  int direction;
  pwm = 0;
  direction = 0;
  while (true)
  {
    if(direction = 0){pwm++;}
	if(direction = 1){pwm--;}
	if(pwm == 1023){direction = 1;)
    if(pwm == 0   ){direction = 0;)
  }
  return (0);
}
  