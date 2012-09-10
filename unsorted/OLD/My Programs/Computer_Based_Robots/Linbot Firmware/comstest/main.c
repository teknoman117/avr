#define F_CPU 11059200UL
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include "avr40pio.h"
#include "avrstdio.h"

int main () {
    enable_uart(5);
	char instr[100];
	setDIRB(0,1);
	setDIRB(1,1);
	setDIRB(2,1);
	setIOB(0,0);
	setIOB(1,0);
	setIOB(2,0);
	while (1) {
	    uscanf(instr,100,ON);
	    if(!(strncmp(instr,">>0,1:LGH1=ON",13))){setIOB(0,1);}
	    if(!(strncmp(instr,">>0,1:LGH2=ON",13))){setIOB(1,1);}
		if(!(strncmp(instr,">>0,1:LGH3=ON",13))){setIOB(2,1);}
	    if(!(strncmp(instr,">>0,1:LGH1=OFF",14))){setIOB(0,0);}
	    if(!(strncmp(instr,">>0,1:LGH2=OFF",14))){setIOB(1,0);}
        if(!(strncmp(instr,">>0,1:LGH3=OFF",14))){setIOB(2,0);}
	    strcpy(instr,"");
	}
	return 0;
}
	