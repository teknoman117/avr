/*
 *  main.c
 *  Simple testing program (blinks lights) for AVR-RTOS
 *
 *  Copyright (C) 2012 Nathaniel Lewis
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

#include "avr-rtos.h"

struct _avr_rtos_task_attr led_1_attr;
struct _avr_rtos_task_attr led_2_attr;
struct _avr_rtos_task_attr led_3_attr;

uint8_t                    led_1_stack[128];
uint8_t                    led_2_stack[128];
uint8_t                    led_3_stack[128];

void led_task(struct _avr_rtos_task_attr *attr)
{
    // To demonstate the passing of operand to the tasks, use the thread id - 1 as the pin to change
    uint8_t pin = attr->id - 1;
    DDRB |= (1 << pin);
    while(1)
    {
        PORTB |= (1 << pin);
        avr_rtos_task_yield();
        PORTB &= ~(1 << pin);
        avr_rtos_task_yield();
    }
}

int main(void)
{
    // Make ready the RTOS!
    avr_rtos_init();
    
    // We will be starting three tasks, all use the same function - although they have independent stacks and 
    // take their ID - 1 as the pin to toggle
 
    // LED 1 Task
    led_1_attr.priority = 1;
    led_1_attr.update_interval = (1000 * 8);  // Execute every 1000 ms
    avr_rtos_add_task(&led_task, &led_1_stack[127], &led_1_attr);
    
    // LED 2 Task
    led_2_attr.priority = 2;
    led_2_attr.update_interval = (500 * 8);  // Execute every 500 ms
    avr_rtos_add_task(&led_task, &led_2_stack[127], &led_2_attr);
    
    // LED 3 Task
    led_3_attr.priority = 3;
    led_3_attr.update_interval = (250 * 8);  // Execute every 250 ms
    avr_rtos_add_task(&led_task, &led_3_stack[127], &led_3_attr);
    
    // Set up the timer for interrupts (this program was written to run at 11.0592 MHz.  Finagle with
    // the prescaler settings and the compare match register.  The goal is to have a compare match 8000 times 
    // per second
    //
    // interrupts per second = F_CPU / (OCR0A * prescaler)
    //
    
    TIMSK0 = _BV(OCIE0A);
    TCCR0A = _BV(WGM01);
    OCR0A = 0xAD;
    TCCR0B = _BV(CS01);
    sei();
    
    while (1)
    {
        // Since this is the thread when there is nothing to do, and we don't have anything to do,
        // just call thread yield.  Though we could do something useful....
        //avr_rtos_task_yield();    // cause a lock up, check in simulator later....
     }
    
    // return control to libgcc.S
    return 0;
}

ISR(TIMER0_COMPA_vect)
{
    // Only call the context switcher every 8'th interrupt (1 KHz)
    if(!(avr_rtos_tick() % 8))
        avr_rtos_isr();
}
