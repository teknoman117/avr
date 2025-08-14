#include <avr/interrupt.h>

#include <stdio.h>
#include <iusart0.h>
#include <timer0ctc.h>

#include "fakeuart.h"
#include "avr-thread.h"

unsigned char        stack1[128];
unsigned char        stack2[128];
avr_thread_context fn_context1;
avr_thread_context fn_context2;

uint32_t                   systemTime = 0;

void thread1 (void) {
    while(1) {
        txbi0('1');
        txbi0('\r');
        txbi0('\n');
        avr_thread_sleep(5000);
    }
}

void thread2 (void) {
    while(1) {
        txbi0('2');
        txbi0('\r');
        txbi0('\n');
        avr_thread_sleep(10000);
    }
}

int main () {
    initFakeUart();
    init_iusart0(129, DB8 | P_N | SB1);
    init_timer0_ctc(T0_PRESCALER_64, PIN_DISCONNECT, PIN_DISCONNECT);
    set_ocr0a(31);   // 8 if prescaler = 256
    avr_thread_init();
    enable_interrupt_t0a();
    avr_thread_start(&fn_context1, thread1, stack1, 128);
    avr_thread_start(&fn_context2, thread2, stack2, 128);
    while(1) {
        txbs('M');
        txbs('\r');
        txbs('\n');
        avr_thread_sleep(2500);
    }
    return 0;    
}

ISR(TIMER0_COMPA_vect, ISR_NAKED) {
    sei();
    avr_thread_isr_start();
    systemTime++;
    avr_thread_isr_end();
}
