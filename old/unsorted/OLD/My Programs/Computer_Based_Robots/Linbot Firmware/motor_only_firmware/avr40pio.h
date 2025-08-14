#define I 0
#define O 1
#include <stdint.h>
uint8_t prta;
uint8_t prtb;
uint8_t prtc;
uint8_t prtd;
uint8_t dira;
uint8_t dirb;
uint8_t dirc;
uint8_t dird;

void   initIO( void );
void   setDIRA(uint8_t pin , uint8_t direction);
void   setDIRB(uint8_t pin , uint8_t direction);
void   setDIRC(uint8_t pin , uint8_t direction);
void   setDIRD(uint8_t pin , uint8_t direction);
void   setIOA(uint8_t pin , uint8_t state);
void   setIOB(uint8_t pin , uint8_t state);
void   setIOC(uint8_t pin , uint8_t state);
void   setIOD(uint8_t pin , uint8_t state);
uint8_t getIOA(uint8_t pin);
uint8_t getIOB(uint8_t pin);
uint8_t getIOC(uint8_t pin);
uint8_t getIOD(uint8_t pin);