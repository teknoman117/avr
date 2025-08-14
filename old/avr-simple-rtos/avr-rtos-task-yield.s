/*
 *  avr-rtos-task-yield.s
 *  Functions that yield the execution of tasks
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

.include "avr-rtos-asm-defs.inc"

.text

; Execution yield function
.global avr_rtos_task_yield
    .type avr_rtos_task_yield, @function
avr_rtos_task_yield:
    push __zero_reg__   ; Push the context (this part is what the ISR does)
    push r0
    in r0,__SREG__      ; Push SREG
    push r0
    clr __zero_reg__
    push r18
    push r19
    push r20
    push r21
    push r22
    push r23
    push r24            ; Generally we push the registers, but in this case, r25:r24 of the task is a pointer
    push r25            ; the attr, so we need to copy that pointer
    push r26
    push r27
    push r30
    push r31
    push r28          
    push r29
    push r2
    push r3
    push r4
    push r5
    push r6
    push r7
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push r16
    push r17

    ; Since registers will be lost, use them for working
    lds r2, (_current_task)           ; Get the currently running task
    ldi r28, lo8(_rtos_table)          ; Load in the base address of the table
    ldi r29, hi8(_rtos_table)
    ldi r16, task_page_size
    mul r2, r16                       ; Get the pointer
    add r28, r0
    add r29, r1
    clr r1

    ; Store the stack pointer
    in r4, __SP_L__
    in r5, __SP_H__
    st  Y,   r4
    std Y+1, r5

    ; Calculate the next run time
    tst r2
    breq skip_timeshift2
    lds r8 , _system_counter      ; Load the system counter
    lds r9 ,(_system_counter)+1
    lds r10,(_system_counter)+2
    lds r11,(_system_counter)+3
    ldd r12, Y+6                   ; Load the update interval
    ldd r13, Y+7
    ldd r14, Y+8
    ldd r15, Y+9
    add r8 , r12                  ; Add the current system time to the update interval 
    adc r9 , r13
    adc r10, r14
    adc r11, r15     
    std Y+2, r8                     ; Store the next run time
    std Y+3, r9
    std Y+4, r10
    std Y+5, r11
skip_timeshift2:

    ; Get the next task and its pointer
    call avr_rtos_select_next_task
    sts (_current_task), r24
    ldi r28, lo8(_rtos_table)          ; Load in the base address of the table
    ldi r29, hi8(_rtos_table)
    ldi r16, task_page_size
    mul r24, r16                      ; Get the pointer
    add r28, r0
    add r29, r1
    clr r1

    ; Move the stack pointer
    ld r2, Y+
    ld r3, Y+
    out __SP_L__, r2
    out __SP_H__, r3

    ; Restore context
    pop r17
    pop r16
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop r7
    pop r6
    pop r5
    pop r4
    pop r3
    pop r2
    pop r29
    pop r28
    pop r31
    pop r30
    pop r27
    pop r26
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    pop r20
    pop r19
    pop r18
    pop r0
    out __SREG__, r0
    pop r0
    pop r1

    ; Return
    reti

