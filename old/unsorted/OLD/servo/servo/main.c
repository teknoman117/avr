/*
 * $Id: main.c,v 1.13 2007/08/02 01:17:11 bsd Exp $
 *
 */

/*
 * SERVO test program for the BDMICRO MAVRIC series board
 *
 * See: http://www.bdmicro.com/
 *
 * Command 6 servos, use the first UART (UART0) to accept simple
 * commands from the serial port to display and set the servo
 * positions.  Wire the servo signal pins to PORTE3, PORTE4, PORTE5,
 * PORTB5, PORTB6, and PORTB7.  These drive servos SERVO1, SERVO2,
 * SERVO3, SERVO4, SERVO5, and SERVO6 respectively.  Be sure and use a
 * seperate power supply for the MAVRIC board from the servo power,
 * otherwise the power supply may brown-out due to the heavy current
 * draw of the servos and cause the CPU to reset.  Also, be sure and
 * use a good power supply for the servos because they tend to be
 * jittery if their power supply is not strong enough.
 *
 * For each servo, the pulse width is set as follows corresponding to
 * the output compare register:
 * 
 *     Pulse Width    OCR3x/OCR1x
 *       1.0 ms         2000
 *       1.5 ms         3000
 *       2.0 ms         4000
 *
 * The nominal servo center position is 1.5 ms.  This is achieved by
 * setting up timers 1 and 3 to run in normal mode with a prescale of
 * 8 and generate an interrupt on overflow, which at 16 MHz external
 * clock, interrupts about 30 times per second - standard servos want
 * the pulse repeated 20 - 50 times per second, so the prescale was
 * chosen to be within this range so that the timer overflows
 * approximately 30 times per second.  When the timer overflow occurs,
 * the servo signal pin is set to high by the time overflow interrupt
 * handler.  This starts the servo pulse.  When the output compare
 * match occurs as defined by the current time value and the content
 * of the servo's output compare register (one for each servo), then
 * the hardware automatically sets the servo signal pin low precisely
 * ending the servo pulse.  The timer continues to count until
 * overflow occurs and the cycle repeats.
 *
 * This all happens in the background - once configured, your program
 * never needs to initiate a pulse.  To change a servo's position,
 * just assign a new value to that servo's output compare register.
 * For convenience, these are alias'd to the names SERVO1 ... SERVO6.
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define BAUD     103 /* 9600 BAUD at 16 MHz */

#define SERVO1   OCR3A
#define SERVO2   OCR3B
#define SERVO3   OCR3C
#define SERVO4   OCR1A
#define SERVO5   OCR1B
#define SERVO6   OCR1C

#define SERVO_MIN 2000
#define SERVO_MID 3000
#define SERVO_MAX 4000

#define SERVO_CLIP(x) (x<SERVO_MIN ? SERVO_MIN : (x>SERVO_MAX ? SERVO_MAX : x))

#define OFFSET(x) (SERVO_CLIP(SERVO_MID + x))

volatile uint8_t urx;
volatile uint8_t urx_recv;

const char s_annc[]              PROGMEM = "\n\n\nSERVO TEST PROGRAM\n\n";
const char s_prompt[]            PROGMEM = ">>> ";
const char s_invalid[]           PROGMEM = "INVALID\n";
const char s_bksp[]              PROGMEM = "\b \b";

const char s_usage[]             PROGMEM =
  "\n"
  "Valid Commands:\n"
  "\n"
  "  status - prints current servo raw and relative-from-center\n"
  "           positions\n"
  "\n"
  "  servo <s1> <s2> <s3> <s4> <s5> <s6>\n"
  "         - set all six servos to the specified positions.\n"
  "           Zero = center, max = 1000, min = -1000\n"
  "\n"
  "  ?       - this help message\n"
  "\n";


/*
 * UART character output routine (polled)
 */
int uart_putc(char c)
{
  while ((UCSR0A & _BV(UDRE)) == 0)
    ;
  UDR0 = c;
  return 0;
}


int uputc(char c, FILE * f)
{
  return uart_putc(c);
}


/*
 * UART0 byte received interrupt handler - fetch the character, check
 * for framing error, flag the event, return.
 */
SIGNAL(SIG_UART0_RECV)
{
  uint8_t s;

  s = UCSR0A;
  urx = UDR0;
  if (bit_is_clear(s, FE)) {
    urx_recv = 1;
  }
}


/* SIG_OVERFLOW3 - this interrupt handler starts the pulse for servos
 * 1, 2, & 3; the timer 3 output compare function automatically ends
 * the pulse precisely as specified by the OCR3x register which
 * represents the servo position */
SIGNAL(SIG_OVERFLOW3)
{
  TCNT3   = 0;

  /* configure to set outputs on compare match so we can turn on the
   * pulse in the next statement */
  TCCR3A |= _BV(COM3A1)|_BV(COM3A0)|_BV(COM3B1)|_BV(COM3B0)|_BV(COM3C1)|_BV(COM3C0);

  /* force compare match to set outputs */
  TCCR3C |= _BV(FOC3A)|_BV(FOC3B)|_BV(FOC3C);

  /* configure to clear outputs on compare match so that the output
   * compare function ends the pulse */
  TCCR3A &= ~(_BV(COM3A0)|_BV(COM3B0)|_BV(COM3C0));
}


/* SIG_OVERFLOW1 - this interrupt handler starts the pulse for servos
 * 4, 5, & 6; the timer 1 output compare function automatically ends
 * the pulse precisely as specified by the OCR1x register which
 * represents the servo position */
SIGNAL(SIG_OVERFLOW1)
{
  TCNT1 = 0;

  /* configure to set outputs on compare match so we can turn on the
   * pulse in the next statement */
  TCCR1A |= _BV(COM1A1)|_BV(COM1A0)|_BV(COM1B1)|_BV(COM1B0)|_BV(COM1C1)|_BV(COM1C0);

  /* force compare match to set outputs */
  TCCR1C |= _BV(FOC1A)|_BV(FOC1B)|_BV(FOC1C);

  /* configure to clear outputs on compare match so that the output
   * compare function ends the pulse */
  TCCR1A &= ~(_BV(COM1A0)|_BV(COM1B0)|_BV(COM1C0));
}


void servo(int16_t s1, int16_t s2, int16_t s3, int16_t s4, int16_t s5, int16_t s6)
{
  SERVO1 = OFFSET(s1);
  SERVO2 = OFFSET(s2);
  SERVO3 = OFFSET(s3);
  SERVO4 = OFFSET(s4);
  SERVO5 = OFFSET(s5);
  SERVO6 = OFFSET(s6);
}


/* parse an integer from a string */
uint16_t get_num(char * s, char ** next, int8_t * err)
{
  char * e;
  uint16_t n;

  n = strtoul(s, &e, 0);
  if (e == s) {
    printf("error: get_num(): can't scan arg: \"%s\"\n", s);
    *next = s;
    if (err)
      *err = 1;
    return 0xffff;
  }

  while (*e == ' ')
    e++;

  *next = e;

  if (err)
    *err = 0;
  
  return n;
}


/*
 * decode and execute simple commands
 */
static char do_cmdbuf[20];
static __inline__ void do_cmd(char * s)
{
  static char * cmd = do_cmdbuf;
  uint8_t       index;
  char        * args, * p;
  int8_t rc;

  if (s[0] == 0)
    return;

  /* parse the command line, seperating the command from arguments */
  cmd[0] = 0;
  index = 0;
  while ((index < sizeof(do_cmdbuf)) && s[index] && (s[index] != ' ')) {
    cmd[index] = s[index];
    index++;
  }
  if (index < sizeof(do_cmdbuf)) {
    cmd[index] = 0;
    args = &s[index];
    while (*args && (*args == ' '))
      args++;
    if (*args == 0)
      args = NULL;
  }
  else {
    cmd[sizeof(do_cmdbuf)-1] = 0;
    args = NULL;
  }

  if (cmd[0] == 0) {
    return;
  }

  if (strcmp(cmd, "?") == 0) {
    printf_P(s_usage);
  }
  else if (strcmp(cmd, "status") == 0) {
    printf("SERVO1 = %u (%d)\n", SERVO1, SERVO1-SERVO_MID);
    printf("SERVO2 = %u (%d)\n", SERVO2, SERVO2-SERVO_MID);
    printf("SERVO3 = %u (%d)\n", SERVO3, SERVO3-SERVO_MID);
    printf("SERVO4 = %u (%d)\n", SERVO4, SERVO4-SERVO_MID);
    printf("SERVO5 = %u (%d)\n", SERVO5, SERVO5-SERVO_MID);
    printf("SERVO6 = %u (%d)\n", SERVO6, SERVO6-SERVO_MID);
  }
  else if (strcmp(cmd, "servo") == 0) {
    if (args) {
      int16_t s1, s2, s3, s4, s5, s6, err=0;
      p = args;
      s1 = get_num(p, &p, &rc);
      printf("setting to %u\n", s1);
      err += rc;
      s2 = get_num(p, &p, &rc);
      err += rc;
      s3 = get_num(p, &p, &rc);
      err += rc;
      s4 = get_num(p, &p, &rc);
      err += rc;
      s5 = get_num(p, &p, &rc);
      err += rc;
      s6 = get_num(p, &p, &rc);
      err += rc;
      if (err == 0)
        servo(s1, s2, s3, s4, s5, s6);
    }
  }
  else {
    printf_P(s_invalid);
    printf_P(s_usage);
  }

  return;
}

/*
 * accept characters and build a command line, when return is pressed,
 * pass the command to 'do_cmd()' 
 */
#define CMD_BUF_LEN 128
static char recv_input_cmdbuf[CMD_BUF_LEN];
void recv_input(uint8_t ch)
{
  static uint8_t idx=0;

  if ((ch == '\r')||(ch == '\n')) {
    uart_putc('\n');
    recv_input_cmdbuf[idx] = 0;
    do_cmd(recv_input_cmdbuf);
    printf_P(s_prompt);
    idx = 0;
  }
  else if ((ch == '\b') && idx) {
    printf_P(s_bksp);
    if (idx > 0)
      idx--;
    recv_input_cmdbuf[idx] = 0;
  }
  else {
    uart_putc(ch);
    recv_input_cmdbuf[idx++] = ch;
    if (idx == CMD_BUF_LEN) {
      idx = 0;
      recv_input_cmdbuf[idx] = 0;
      printf_P(s_invalid);
      printf_P(s_prompt);
    }
  }
}


void init_servos(void)
{
  /* Use Timers 1 and 3 to generate the pulses for 6 R/C servos; each
   * timer can do up to 3 servos. */
  
  /*
   * configure OC3A for mode 0: normal, top=0xffff prescale=8 (f~=30):
   *
   * WGM33=0, WGM23=0, WGM13=0, WGM03=0, CS32=0, CS31=1, CS30=0 
   */
  DDRE   |= _BV(PORTE3) | _BV(PORTE4) | _BV(PORTE5);
  TCCR3A &= ~(_BV(WGM31) | _BV(WGM30) | _BV(COM3A1) | _BV(COM3B1) | _BV(COM3C1));
  TCCR3A |= _BV(COM3A0) | _BV(COM3B0) | _BV(COM3C0);
  TCCR3B &= ~(_BV(WGM33) | _BV(WGM32) | _BV(CS32) | _BV(CS30));
  TCCR3B |= _BV(CS31);
  TCNT3   = 0;
  TCCR3C |= _BV(FOC3A) | _BV(FOC3B) | _BV(FOC3C);
  ETIMSK |= _BV(TOIE3);

  /*
   * configure OC1A for mode 0: normal, top=0xffff prescale=8 (f~=30):
   *
   * WGM33=0, WGM23=0, WGM13=0, WGM03=0, CS32=0, CS31=1, CS30=0 
   */
  DDRB   |= _BV(PORTB5) | _BV(PORTB6) | _BV(PORTB7);
  TCCR1A &= ~(_BV(WGM11) | _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1) | _BV(COM1C1));
  TCCR1A |= _BV(COM1A0) | _BV(COM1B0) | _BV(COM1C0);
  TCCR1B &= ~(_BV(WGM13) | _BV(WGM12) | _BV(CS12) | _BV(CS10));
  TCCR1B |= _BV(CS11);
  TCNT1   = 0;
  TCCR1C |= _BV(FOC1A) | _BV(FOC1B) | _BV(FOC1C);
  TIMSK  |= _BV(TOIE1);

  /* set all servos to their center positions */
  SERVO1 = SERVO_MID;
  SERVO2 = SERVO_MID;
  SERVO3 = SERVO_MID;
  SERVO4 = SERVO_MID;
  SERVO5 = SERVO_MID;
  SERVO6 = SERVO_MID;
}


int main(void)
{
  uint8_t ch;

  /* enable UART0 */
  UBRR0H = 0;
  UBRR0L = BAUD;
  UCSR0B = _BV(RXEN)|_BV(TXEN)|_BV(RXCIE);

  /* initialize servos */
  init_servos();

  /* enable interrupts */
  sei();

  /* initialize stdio for printf */
  fdevopen(uputc, NULL);

  /* output startup announcement message */ 
  printf_P(s_annc);

  /* output a prompt */
  printf_P(s_prompt);

  /* loop - wait for commands from the serial interface */
  while (1) {
    if (urx_recv) {
      cli();
      urx_recv = 0;
      ch = urx;
      sei();
      /* build a command line and execute commands when complete */
      recv_input(ch);
    }
  }

}

