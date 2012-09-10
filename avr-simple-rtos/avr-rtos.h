/*
 *  avr-rtos.h
 *  AVR RTOS library and data definitions
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


#ifndef _AVR_RTOS_H_
#define _AVR_RTOS_H_

#include <inttypes.h>

/**
 *  The maximum number of tasks the RTOS can have scheduled.  This value must be shared with avr-rtos-asm-defs.inc 
 *  at compile time
 */
#define MAX_TASKS 4

// ------------------ RTOS Data Structures ------------------
/**
 *  Defines a task entry in the RTOS table
 */
struct _avr_rtos_task_entry
{
    /**
     *  Pointer to the current end of the stack.  Its valid only when the task is not running (after a context switch)
     */
    uint8_t  *stackptr;
    
    /**
     *  Holds the system time when the task should next execute
     */
    uint32_t  nextrun;
    
    /**
     *  Hold the amount of time that is added to the system counter to determine the task's next run time after
     *  an avr_rtos_task_yield() or a context switch
     */
    uint32_t  updateinterval;
    
    /** 
     *  The task's execution priority.  During task selection, of the task which can run, the one with the highest
     *  priority will be selected to execute.
     */
    uint8_t   priority;
    
    /**
     *  For internal use only - hold whether this task is a valid entry.  Used by the task selection function
     */
    uint8_t   valid;
};

/**
 *  Defines the attributes of a particular task
 */
struct _avr_rtos_task_attr
{
    /** 
     *  The task's execution priority.  During task selection, of the task which can run, the one with the highest
     *  priority will be selected to execute.
     */
    uint8_t  priority;
    
    /** 
     *  The ID of the task this attributes object was bound to.  This field will be set during the process of
     *  adding a task
     */
    uint8_t  id;
    
    /**
     *  Hold the amount of time that is added to the system counter to determine the task's next run time after
     *  an avr_rtos_task_yield() or a context switch
     */
    uint32_t update_interval;
};

// ------------------ RTOS global data ----------------------------------

/**
 *  The RTOS task table.  Holds all the information about tasks
 */
struct   _avr_rtos_task_entry _rtos_table[MAX_TASKS];  

/*
 *  The current system time
 */
volatile uint32_t             _system_counter;         
/*
 *  The currently running task
 */
volatile uint8_t              _current_task;

// ------------------ Task Execution Control ----------------------------

/**
 *  Yields execution of the current task to another available task
 */
extern void            avr_rtos_task_yield();          

// ------------------ Task Scheduling -----------------------------------

/**
 *  Schedules a task to be run 
 *
 *  @param task_pointer Pointer to the function to run as a task.  The function must not have a return type (void)
 *  and take a pointer to a _avr_rtos_task_attr structure.
 *
 *  @param stack_end_pointer Pointer to the end of the task's stack.  If you allocated a stack statically, such as 
 *  uint8_t task_stack[128];, you would pass "&task_stack[127]" for this parameter
 *
 *  @param attributes Pointer to an _avr_rtos_task_attr structure.  The priority and update intervals must be loaded 
 *  with the desired values and the id field will be assigned to the tasks it
 *
 *  @return the id of the task that was added.  Same value that will be assigned to the attributes structure
 */
extern uint8_t         avr_rtos_add_task(void (*task_pointer)(struct _avr_rtos_task_attr *),
                                         uint8_t *stack_end_pointer,
                                         struct _avr_rtos_task_attr *attributes);

extern void            avr_rtos_remove_task(uint8_t id);

extern uint8_t         avr_rtos_select_next_task();
extern uint8_t         avr_rtos_next_task_slot();

/* System timing */
static inline uint32_t avr_rtos_tick()
{
    return ++_system_counter;
}

static inline uint32_t get_system_time()
{
    return _system_counter;
}
                                         
/* RTOS library functions */
extern void            avr_rtos_init();              
extern void            avr_rtos_isr();              

#endif /* AVR-RTOS_H_ */