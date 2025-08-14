#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define LCD_PORT PORTC
#define LCD_DDR DDRC
#define RS PORTC6
#define EN PORTC7
#define LCD_DATA_OFFSET 2
/* examples of offset
 * PORTnx is pin x on port n
 * PORTn0 - LCD DB4
 * PORTn1 - LCD DB5
 * PORTn2 - LCD DB6
 * PORTn3 - LCD DB7
 * ---DB4 is connected to lowest pin, offset is 0
 * PORTn2 - LCD DB4
 * PORTn3 - LCD DB5
 * PORTn4 - LCD DB6
 * PORTn5 - LCD DB7
 * ---DB4 is connected to 3rd pin, two from beginning, offset is 2
 */ 

#define line0 128
#define line1 192
#define line2 148
#define line3 212

char str[20];
char* strp = (char*) str;
volatile uint32_t enc=0;

/*void display (char str[], unsigned char location) {
    PORTC &= ~(1 << PORTC1);
	PORTA = location;
	PORTC |= (1 << PORTC0);
	_delay_us(10);
	PORTC &= ~(1 << PORTC0);
	_delay_ms(10);
    PORTC |= (1 << PORTC1);
    unsigned char cntr;
	for(cntr=0;cntr<strlen(str);cntr++) {
	    PORTA = str[cntr];
	    PORTC |= (1 << PORTC0);
	    _delay_us(10);
	    PORTC &= ~(1 << PORTC0); 
	    _delay_ms(1);  
	}
} 


int main ( void ) {
    DDRA = 0xFF;
	DDRC = 0xFF;
	PORTC &= 0xFC;
	_delay_ms(30 );
	PORTA = 56;
	//PORTA = 32;
	PORTC |= (1 << PORTC0);
	_delay_us(10);
	PORTC &= ~(1 << PORTC0);
    _delay_ms(10);   
	PORTA = 15;
	PORTC |= (1 << PORTC0);
	_delay_us(10);
	PORTC &= ~(1 << PORTC0);
	_delay_ms(10);
	display("Line 1 !", 0x80);
	display("Line 2 !", 0x80 + 64);
	display("Line 3 !", 0x80 + 20);
	display("Line 4 !", 0x80 + 84);			
    while (1) ;
	return 0;
}*/
 
void lcd_char ( uint8_t c ) {
    PORTC |= (1 << PORTC6);
    PORTC &= 0xc3;
	PORTC |= ((c >> 2) & 0x3c);
	PORTC |= (1 << PORTC7);
	_delay_us(10);
	PORTC &= ~(1 << PORTC7);
	_delay_ms(5);
    PORTC &= 0xc3;
	PORTC |= ((c << 2) & 0x3c);
	PORTC |= (1 << PORTC7);
	_delay_us(10);
	PORTC &= ~(1 << PORTC7);
	_delay_ms(5);
}	

void lcd_command ( uint8_t command ) {
    PORTC &= ~(1 << PORTC6);
    PORTC &= 0xc3;
	PORTC |= ((command >> 2) & 0x3c);
	PORTC |= (1 << PORTC7);
	_delay_us(10);
	PORTC &= ~(1 << PORTC7);
	_delay_ms(5);
    PORTC &= 0xc3;
	PORTC |= ((command << 2) & 0x3c);
	PORTC |= (1 << PORTC7);
	_delay_us(10);
	PORTC &= ~(1 << PORTC7);
	_delay_ms(5);		
}

void lcd_init ( void ) {
	DDRC |= 0xfc;
	PORTC &= 0x03;
	_delay_ms(30);
	PORTC |= (2 << 2);
	PORTC |= (1 << PORTC0);
	_delay_us(10);
	PORTC &= ~(1 << PORTC0);
    _delay_ms(10);  
}

void lcd_print (char str[], uint8_t location) {
    lcd_command(location);
    uint8_t cntr;
	for(cntr=0;cntr<strlen(str);cntr++) {
	    lcd_char(str[cntr]);
	}
}

void lcd_cursor (uint8_t on, uint8_t blink) {
	uint8_t cmd = 0x0C;
	if(on) {cmd+=2;}
	if(blink) {cmd++;}
	lcd_command(cmd);
}

void lcd_clear ( void ) {
	lcd_command(0x01);
}

int main ( void ) {
	_delay_ms(1000);
	uint8_t cchar[] = {128,138,138,128,145,142,128,128};
	lcd_init();
	lcd_command(0x28);
	lcd_cursor(0, 0);
	lcd_clear();
	_delay_ms(200);
	//lcd_print(cchar, 64);
	//lcd_command(0x80);
	//lcd_char(0);
	lcd_print("ATmega644 QENC Test", line0);
	DDRA = 0;
	PCICR = 1;
	PCMSK0 = 3;
	sei();
	while (1) {
		sprintf(strp, "Count: %ld   ", enc);
	    lcd_print(str, line1);
	    sprintf(strp, "Rotations: %ld", enc/2496);
	    lcd_print(str, line2);
	    sprintf(strp, "Dis Gone: %ld", (enc/2496)*7);
	    lcd_print(str, line3);
	}
	//lcd_print("   Wright Hobbies  ", line2);
	//lcd_print("      Robotics     ", line3);
    while (1) ;
	return 0;
}

uint8_t lastc ( uint8_t c ) {
	int8_t r = c - 1;
	if(r == -1) {r = 3;}
	return r;
}

uint8_t nextc ( uint8_t c ) {
	uint8_t r = c + 1;
	if(r == 4) {r = 0;}
	return r;
}

ISR (PCINT0_vect) {
	enc++;
}
	
