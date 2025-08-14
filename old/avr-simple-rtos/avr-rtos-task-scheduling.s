/*
 *  avr-rtos-task-control.s
 *  Functions that control tash scheduling (derp)
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

; Function to add a task to the RTOS table
.global avr_rtos_add_task
    .type avr_rtos_add_task, @function
avr_rtos_add_task:
    ; Prologue - Back up registers the compiler loves so much
    push r14
    push r15
    push r16
    push r17
    ; Prepare the stack of the new task 
    in r26, __SP_L__    ; Back up our stack pointer
    in r27, __SP_H__
    out __SP_L__, r22   ; Move stack pointer to new stack
    out __SP_H__, r23 
    push r24            ; Push the return address (the function pointer)
    push r25
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
    push r20            ; Generally we push the registers, but in this case, r25:r24 of the task is a pointer
    push r21            ; the attr, so we need to copy that pointer
    push r26
    push r27
    push r30
    push r31
    push r28            ; Push what the context switch will do
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
    
    ; Load the new stack base pointer for the task -> goes in RTOS table
    in r22, __SP_L__
    in r23, __SP_H__
    out __SP_L__, r26     ; Restore our stack    
    out __SP_H__, r27

    ; Fetch the task id and the priority
    movw r30, r20       ; move the attributes pointer into Z register
    ld r18, Z+          ; store the priority in r18 (we don't need the attr pointer anymore)
    call avr_rtos_next_task_slot ; call function that returns the next available task slot
    tst r24             ; test for zero
    breq skip_add_task  ; if its zero, skip the function, otherwise store the id and continue
    st Z+, r24 

    ; Calculate offset pointer into _rtos_table (store in X)
    ldi r26, lo8(_rtos_table)
    ldi r27, hi8(_rtos_table)
    ldi r25, 0x0c    ; size of an rtos page
    mul r25, r24     ; calculate offset into table
    add r26, r0      ; add offset to base
    adc r27, r1
    clr __zero_reg__ ; Reset r1 to 0

    ; Make _rtos_table page
    st X+, r22                    ; Copy stack pointer
    st X+, r23
    lds r20, _system_counter      ; Load the system counter
    lds r21,(_system_counter)+1
    lds r22,(_system_counter)+2
    lds r23,(_system_counter)+3
    ld r14, Z+                   ; Load the update interval
    ld r15, Z+
    ld r16, Z+
    ld r17, Z+
    add r20, r14                  ; Add the current system time to the update interval 
    adc r21, r15
    adc r22, r16
    adc r23, r17     
    st X+, r20                    ; Store the next run time
    st X+, r21
    st X+, r22
    st X+, r23
    st X+, r14                    ; Store the update interval
    st X+, r15
    st X+, r16
    st X+, r17
    st X+, r18                    ; Store the priority
    ldi r19, 0x01                 ; Valid task
    st X+, r19
    clr r25
skip_add_task:
    ; Epilogue - restore the registers previously backed up
    pop r17
    pop r16
    pop r15
    pop r14
    ret

.global avr_rtos_select_next_task
    .type avr_rtos_select_next_task, @function
avr_rtos_select_next_task:
    push r14                    ; Push registers that we are modifying without permission (hehe)
    push r15
    push r16
    push r17
    lds r14,_system_counter     ; Load the current system counter value
    lds r15,(_system_counter)+1
    lds r16,(_system_counter)+2
    lds r17,(_system_counter)+3
    ldi r21, 0x00               ; storing incrementing number (init to zero)
    ldi r19, 0x00               ; main had zero priority
    ldi r20, 0x00               ; main is currently the selected task
    ldi r30, lo8(_rtos_table)
    ldi r31, hi8(_rtos_table)
select_task:
    inc r21                     ; increment the counter register
    ;andi r21,max_tasks_mask     ; mask unused bits
    ;tst r21                     ; if its zero, we've looped back to main, exit
    cpi r21, max_tasks          ; Compare with the maximum task count
    breq done_selecting_task    ; If they are equal it means we have finished searching for tasks
    adiw r30, task_page_size    ; add the page size to get the next pointr
    ldd r0, Z+11                ; test if the task is valid
    tst r0
    breq select_task
    ldd r24, Z+2                ; it is, load its next run time
    ldd r25, Z+3
    ldd r26, Z+4
    ldd r27, Z+5
    cp r14, r24                 ; compare it with the system timer
    cpc r15, r25
    cpc r16, r26
    cpc r17, r27
    brcs select_task            ; if the system timer is less, try another task
    ldd r0, Z+10               ; test the priority, see if its higher than any other requesting task
    cp r0, r19
    brlt select_task
    mov r20, r21                 ; it was, so choose this task
    mov r19, r0
    rjmp select_task
done_selecting_task:
    mov r24,r20
    ; Restore registers
    pop r17
    pop r16
    pop r15
    pop r14

    ; Return
    ret
