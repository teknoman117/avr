#include <avr/io.h>
#include <stdint.h>
#include "M32io2.h"

void SetDir(char port,uint8_t pin,char dir) {
  switch (port) {
   case 'A': 
    if(dir == 'I') {
	  if(((DDRA >> pin) & 1) == 1){DDRA -= (1 << pin);}
	}
	if(dir == 'O') {
	  if(((DDRA >> pin) & 1) == 0){DDRA += (1 << pin);}
	}
   break;
   case 'B':    
    if(dir == 'I') {
	  if(((DDRB >> pin) & 1) == 1){DDRB -= (1 << pin);}
	}
	if(dir == 'O') {
	  if(((DDRB >> pin) & 1) == 0){DDRB += (1 << pin);}
	}
   break;
   case 'C':    
    if(dir == 'I') {
	  if(((DDRC >> pin) & 1) == 1){DDRC -= (1 << pin);}
	}
	if(dir == 'O') {
	  if(((DDRC >> pin) & 1) == 0){DDRC += (1 << pin);}
	}
   break;
   case 'D':    
    if(dir == 'I') {
	  if(((DDRD >> pin) & 1) == 1){DDRD -= (1 << pin);}
	}
	if(dir == 'O') {
	  if(((DDRD >> pin) & 1) == 0){DDRD += (1 << pin);}
	}
   break;
  }
} 
 
void SetPin(char port,uint8_t pin,uint8_t state) {
  switch (port) {
   case 'A': 
    if(state == 0) {
	  if(((PORTA >> pin) & 1) == 1){PORTA -= (1 << pin);}
	}
	if(state == 1) {
	  if(((PORTA >> pin) & 1) == 0){PORTA += (1 << pin);}
	}
   break;
   case 'B':    
    if(state == 1) {
	  if(((PORTB >> pin) & 1) == 1){PORTB -= (1 << pin);}
	}
	if(state == 0) {
	  if(((PORTB >> pin) & 1) == 0){PORTB += (1 << pin);}
	}
   break;
   case 'C':    
    if(state == 1) {
	  if(((PORTC >> pin) & 1) == 1){PORTC -= (1 << pin);}
	}
	if(state == 0) {
	  if(((PORTC >> pin) & 1) == 0){PORTC += (1 << pin);}
	}
   break;
   case 'D':    
    if(state == 1) {
	  if(((PORTD >> pin) & 1) == 1){PORTD -= (1 << pin);}
	}
	if(state == 0) {
	  if(((PORTD >> pin) & 1) == 0){PORTD += (1 << pin);}
	}
   break;
  }
}

uint8_t GetPin (char port,uint8_t pin) {
  switch (port) {
    case 'A':return ((PINA >> pin) & 1);break;
	case 'B':return ((PINB >> pin) & 1);break;
	case 'C':return ((PINC >> pin) & 1);break;
	case 'D':return ((PIND >> pin) & 1);break;
  }
} 
