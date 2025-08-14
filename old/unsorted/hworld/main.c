#include <stdio.h>
#include <stdusart0.h>

int main () {
	init_stdusart0(BAUD(38400), DB8 | P_N | SB1);
	printf("Hello World!\r");
	while(1);  // Never let embedded programs exit
	return 0;
}
