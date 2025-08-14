/*
 *      eetest.c
 *      
 *      Copyright 2009 Nathaniel Robert Lewis <linux.robotdude@gmail.com>
 *      
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */
#define F_CPU 16000000UL

#include <util/delay.h>
#include <stdusart0.h>
#include <i2c.h>
#include <stdio.h>

int main ( void ) {
	init_stdusart0(51, DB8 | P_N | SB1);
	init_i2c(128, I2C_PRESCALER_4);
	printf("I2C EEprom & Sonar Testing Program\r:");
	char cmd = '\0';
	uint16_t data = 0;
	uint16_t address = 0;
	while (1) {
	    scanf("%c", &cmd);
	    if(cmd == 'r') {          //reads a byte from a 24C type I2C eeprom
	        printf("\rLocation? ");
	        scanf("%d", &address);
	        i2c_start(0xa0);
	        i2c_txb((uint8_t)(address & 0xFF));
	        i2c_start(0xa1);
	        data = i2c_rxbNack();
	        i2c_stop();
	        printf("Location %d: %d\r:", address, data);	
	    }
	    if(cmd == 'w') {          //writes a byte to a 24C type I2C eeprom
	    	printf("\rLocation? ");
	    	scanf("%d", &address);
	    	printf("Data? ");
	    	scanf("%d", &data);
	    	i2c_start(0xa0);
	    	i2c_txb((uint8_t)(address & 0xFF));
	        i2c_txb((uint8_t)(data & 0xFF));
	        i2c_stop();
	        putchar(':');
        }
        if(cmd == 's') {           //reads a SRF02 sonar programed to be at E0
        	printf("\rReading Sonar at 0xE0...\r");
        	i2c_start(0xe0);       //access sonar at 0xE0
        	i2c_txb(0x00);         //select proper address
        	i2c_txb(0x52);         //tell to range in microseconds
        	i2c_stop();            //send stop
        	_delay_ms(70);
        	i2c_start(0xe0);
        	i2c_txb(0x02);
        	i2c_start(0xe1);
        	data = i2c_rxbAck();
        	data <<= 8;
        	data |= i2c_rxbNack();
        	i2c_stop();
        	printf("Sonar E0 in CM: %d\r:", data);
        }
        if(cmd == 'a') {          //write an ATmega8 register from the ATmega644
            printf("\rRegister? ");
            scanf("%d", &address);
            printf("Value? ");
            scanf("%d", &data);
            if(i2c_start(0xca) < 0) {
            	printf("error accessing\r");
            }
            else {
                i2c_txb((uint8_t)(address & 0xFF));
                i2c_txb((uint8_t)(data & 0xFF));
                i2c_stop();	
            }	
            printf("done.\r:");
        }
        if(cmd == 'c') {          //read an ATmega8 register from the ATmega644
            printf("\rRegister? ");
            scanf("%d", &address);
            printf("Setting Address...\r");
            data = 0;
            if(i2c_start(0xca) < 0) {
            	printf("error accessing\r");
            }
            else {
            	printf("Transmitting Address...\r");
                i2c_txb((uint8_t)(address & 0xFF));
                printf("Stopping Transfer...\r");
                i2c_stop();
                printf("Done\r");
            }
            printf("Going for read...\r");
            if(i2c_start(0xcb) < 0) {
            	printf("error accessing\r");
            }
            else {
            	printf("Receiving Data...\r");
                data = i2c_rxbNack();
                printf("Stopping Transfer...\r");
                i2c_stop();
                printf("Done\r");
            }
            printf("Address %d = %d\r:", address, data);
        }
	}
	return 0;
}
