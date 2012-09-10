#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "avrstdio.h"
#include "encoders.h"
#include "M32io2.h"
#include "srf02.h"
#include "timer1PWM.h"
#include "adc.h"

char dumb[20];
char ReceiveStr[50];
uint8_t cnt = 0;
uint8_t op = 1;

//Movement Function Declarations
void Forward (void);
void Backward (void);
void SpinL (void);
void SpinR (void);
void TurnL (void);
void TurnR (void);
void Stop (void);
void tservop (void);
void pservop (void);

//Sensor Declarations
void lgh (void);
void snr (void);
void bmp (void);
void enc (void);

//Utility Declarations
uint16_t ExtractNum (uint8_t stacnt);

int main () {
  enable_uart(25);
  initENC();
  initservos(625,ON,ON);
  initPWM10();
  initADC();
  initcommsunit();
  SetDir('B',0,'O');
  SetDir('B',1,'O');
  SetDir('B',2,'O');
  SetDir('B',3,'O');
  SetDir('B',4,'I');
  SetDir('B',5,'I');
  uprintf("Type INIT <enter> to begin\r");
  while(strncmp(dumb,"INIT",4) == 1) {
    uscanf(dumb,10,ON);
  }
  //uprintf("AVR Initalized\r");
  op = 1;
  while (op == 1) {
    uscanf(ReceiveStr,50,ON);
    if(strncmp(ReceiveStr,"Forward",7) == 0) {Forward();}
	if(strncmp(ReceiveStr,"Backward",8) == 0) {Backward();}
	if(strncmp(ReceiveStr,"SpinL",5) == 0) {SpinL();}
	if(strncmp(ReceiveStr,"SpinR",5) == 0) {SpinR();}
	if(strncmp(ReceiveStr,"TurnL",5) == 0) {TurnL();}
	if(strncmp(ReceiveStr,"TurnR",5) == 0) {TurnR();}
	if(strncmp(ReceiveStr,"Stop",4) == 0) {Stop();}
    if(strncmp(ReceiveStr,"Tservop",7) == 0) {tservop();}
    if(strncmp(ReceiveStr,"Pservop",7) == 0) {pservop();}
    if(strncmp(ReceiveStr,"LGH",3) == 0) {lgh();}
    if(strncmp(ReceiveStr,"SNR",3) == 0) {snr();}
    if(strncmp(ReceiveStr,"BMP",3) == 0) {bmp();}
	if(strncmp(ReceiveStr,"ENC",3) == 0) {enc();}
	if(strncmp(ReceiveStr,"DISABLE",7) == 0) {op = 0;}
  }
  uprintf("Disaled\r");
  while (1) {}
}

uint16_t ExtractNum (uint8_t stacnt) {
  char tst[50];
  uint8_t cntt;
  cnt = stacnt;
  strcpy(tst,ReceiveStr);
  for(cntt=0;cntt!=cnt;cntt++){tst[cntt] = ' ';}
  return atoi(tst);
}
  
void Forward (void) {
  uprintf("Command Excepted\r");
  setDirL(1);
  setDirR(1);
  SetPin('B',0,1);
  SetPin('B',1,0);
  SetPin('B',2,1);
  SetPin('B',3,0);
  setCHA10(ExtractNum(7));
  setCHB10(ExtractNum(7));
}
    
void Backward (void) {
  uprintf("Command Excepted\r");
  setDirL(-1);
  setDirR(-1);
  SetPin('B',0,0);
  SetPin('B',1,1);
  SetPin('B',2,0);
  SetPin('B',3,1);
  setCHA10(ExtractNum(8));
  setCHB10(ExtractNum(8));
}

void SpinL (void) {
  uprintf("Command Excepted\r");
  setDirL(-1);
  setDirR(1);
  SetPin('B',0,0);
  SetPin('B',1,1);
  SetPin('B',2,1);
  SetPin('B',3,0);
  setCHA10(ExtractNum(5));
  setCHB10(ExtractNum(5));
}

void SpinR (void) {
  uprintf("Command Excepted\r");
  setDirL(1);
  setDirR(-1);
  SetPin('B',0,1);
  SetPin('B',1,0);
  SetPin('B',2,0);
  SetPin('B',3,1);
  setCHA10(ExtractNum(5));
  setCHB10(ExtractNum(5));
}

void TurnL (void) {
  uprintf("Command Excepted\r");
  setDirL(0);
  setDirR(1);
  SetPin('B',0,0);
  SetPin('B',1,0);
  SetPin('B',2,1);
  SetPin('B',3,0);
  setCHA10(ExtractNum(5));
  setCHB10(ExtractNum(5));
}

void TurnR (void) {
  uprintf("Command Excepted\r");
  setDirL(1);
  setDirR(0);
  SetPin('B',0,1);
  SetPin('B',1,0);
  SetPin('B',2,0);
  SetPin('B',3,0);
  setCHA10(ExtractNum(5));
  setCHB10(ExtractNum(5));
}

void Stop (void) {
  uprintf("Command Excepted\r");
  setDirL(0);
  setDirR(0);
  SetPin('B',0,0);
  SetPin('B',1,0);
  SetPin('B',2,0);
  SetPin('B',3,0);
  setCHA10(0);
  setCHB10(0);
}

void tservop (void) {
  uprintf("Command Excepted\r");
  setservo1(ExtractNum(7));
}

void pservop (void) {
  uprintf("Command Excepted\r");
  setservo2(ExtractNum(7));
}

void lgh (void) {
  uprintf("Command Excepted\r");
  uint8_t adcval;
  adcval = ADC08bit(ExtractNum(3));
  itoa(adcval,dumb,10);
  uprintf(dumb);
  uprintf("\r");
}

void snr (void) {
  uprintf("Command Excepted\r");
  switch (ExtractNum(3)) {
    case 0:itoa(getCM(0xE2),dumb,10);break;
	case 1:itoa(getCM(0xE4),dumb,10);break;
	case 2:itoa(getCM(0xE6),dumb,10);break;
  }
  uprintf(dumb);
  uprintf("\r");
}

void bmp (void) {
  uprintf("Command Excepted\r");
  switch (ExtractNum(3)) {
    case 0:itoa(GetPin('B',4),dumb,10);break;
    case 1:itoa(GetPin('B',5),dumb,10);break;
  }
  uprintf(dumb);
  uprintf("\r");
}

void enc (void) {
  uprintf("Command Excepted\r");
  //switch (ExtractNum(3)) {
  //  case 0:dtostrf(getDisL(),12,1,dumb);break;
 //   case 1:dtostrf(getDisR(),12,1,dumb);break;
 // }
  uprintf(dumb);
  uprintf("\r");
}
  
  