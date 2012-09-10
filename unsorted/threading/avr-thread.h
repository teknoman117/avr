//
// avr-thread
//
// Copyright 2000, 2001, 2002, 2003, 2004 Dean Ferreyra
//
// $Id: avr-thread.h,v 1.3 2008-09-04 01:10:01 dean Exp $
// Dean Ferreyra

// Threads for AVR.

/*
This file is part of AVR-THREADS.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; see the file COPYING.LIB.  If not,
write to the Free Software Foundation, Inc., 59 Temple Place, Suite
330, Boston, MA 02111-1307  USA

Contact information:

Dean Ferreyra
12902 Malena Drive
Santa Ana, CA 92705-1102  USA

dean@octw.com
*/

#ifndef AVR_THREAD_INCLUDED
#define AVR_THREAD_INCLUDED 1

#include <inttypes.h>

/*
 * avr_thread_context
 *
 * This structure maintains the context for a thread.  Instantiate
 * one for each thread you create.
 *
 * The context must remain valid for the life of the thread it
 * refers to.
 */

typedef enum {
    ats_normal,
    ats_wait,

    ats_clear = 0x40,
    ats_tick = 0x80
} __attribute__((packed)) avr_thread_state;

typedef struct avr_thread_context {
    volatile avr_thread_state state;
    uint8_t* stack_ptr;
    volatile struct avr_thread_context* next;
    volatile int16_t timeout;
    volatile void* waiting_for;
    volatile struct avr_thread_context* next_waiting;
    volatile struct avr_thread_context* prev_waiting;

#if defined(DEBUG_STACK_USAGE)
    uint8_t* stack_end;
    uint8_t* min_stack_ptr;
#endif
} avr_thread_context;

extern volatile uint8_t avr_thread_disabled;
extern avr_thread_context* avr_thread_active;

extern avr_thread_context avr_thread_main_context;
extern avr_thread_context avr_thread_idle_context;

/*
 * avr_thread_init()
 *
 * Must be called before any other avr_thread function.
 */
void avr_thread_init(void);

static inline avr_thread_context* avr_thread_running(void)
{
    return avr_thread_active;
}

static inline void avr_thread_enable(void)
{
    uint8_t sreg = SREG;
    cli();
    avr_thread_disabled--;
    SREG = sreg;
}

static __inline__ void avr_thread_disable(void)
{
    uint8_t sreg = SREG;
    cli();
    avr_thread_disabled++;
    SREG = sreg;
}

/*
 * avr_thread_start()
 *
 * Starts a new thread.
 */
void avr_thread_start(avr_thread_context* context,
		      void (*fn)(void),
		      uint8_t* stack,
		      uint16_t stack_size);

/*
 * avr_thread_stop()
 *
 * Stops the current thread.
 */
void avr_thread_stop(void);

/*
 * avr_thread_sleep()
 *
 * Puts the current thread to sleep for the given number of ticks.
 * Passing zero ticks forces a task switch without sleeping.
 */
void avr_thread_sleep(uint16_t ticks);

inline static void avr_thread_yield(void)
{
    avr_thread_sleep(0);
}

/*
 * avr_thread_mutex
 *
 * Used by the mutex functions.
 */

typedef struct {
    uint8_t lock_count;
    avr_thread_context* owner;
} avr_thread_mutex;

/*
 * avr_thread_mutex_gain()
 *
 * Tries to gain ownership of the given mutex.  If the mutex is
 * already owned by another thread, this thread blocks.  If the
 * mutex is unowned, the current thread gets ownership and the
 * function returns immediately.  If the mutex is owned by the
 * current thread, the lock-count is incremented and the
 * function returns immediately.
 *
 * If the thread blocks, it will block until the mutex becomes
 * available or until the given number of ticks have elapsed.
 * If you pass zero ticks, the function will wait indefinitely until
 * the mutex becomes available.
 *
 * If ownership of the mutex is gained, the function returns 1.
 * Otherwise, it returns 0.
 */
uint8_t avr_thread_mutex_gain(volatile avr_thread_mutex* mutex,
                              uint16_t ticks);

/*
 * avr_thread_mutex_release()
 *
 * Releases ownership of the given mutex.
 *
 * The mutex is not released until this function is called the
 * same number of times as the avr_thread_mutex_gain() function
 * was called.
 *
 * If multiple threads are waiting on this mutex, the thread
 * that has been waiting the longest is released.
 */
void avr_thread_mutex_release(volatile avr_thread_mutex* mutex);

/*
 * avr_thread_mutex_basic
 *
 * Used by the basic-mutex functions.
 */
typedef struct {
    uint8_t locked;
} avr_thread_mutex_basic;

/*
 * avr_thread_mutex_basic_gain()
 *
 * Tries to lock the basic-mutex.  If it is already locked, the
 * thread blocks on the basic-mutex.
 *
 * Ownership and lock counts are not supported by the basic-mutex,
 * so once a thread locks a mutex, if it calls this function again
 * without an intervening call to avr_thread_mutex_basic_release(),
 * the thread will block.
 */
void avr_thread_mutex_basic_gain(volatile avr_thread_mutex_basic* mutex);

/*
 * avr_thread_mutex_basic_test_and_gain()
 *
 * Tries to lock the basic-mutex.  If it is not already locked
 * by another thread, the mutex is locked and the function
 * returns 1.  If it is already locked by another thread,
 * the function returns 0 immediately.
 */
uint8_t avr_thread_mutex_basic_test_and_gain(volatile avr_thread_mutex_basic*
                                             mutex);

/* avr_thread_mutex_basic_release()
 *
 * Releases a locked mutex.
 *
 * For basic-mutexes when multiple threads are waiting,
 * there is no preference for releasing the thread that has been
 * waiting the longest.
 */
void avr_thread_mutex_basic_release(volatile avr_thread_mutex_basic* mutex);

/*
 * avr_thread_event
 *
 * Used by event functions.
 */
typedef struct {
    volatile uint8_t state;
} avr_thread_event;

/*
 * avr_thread_event_set_wake_one()
 *
 * Signals that the event has occurred.  If there are threads
 * waiting on this event, the thread that has been waiting the longest
 * is awakened.
 *
 * While the event is set, a thread that calls avr_thread_event_wait()
 * or avr_thread_event_wait_and_clear() will return immediately.
 */
void avr_thread_event_set_wake_one(volatile avr_thread_event* event);

/*
 * avr_thread_event_set_wake_all()
 *
 * Signals that the event has occurred.  If there are threads
 * waiting on this event, all of them are awakened.
 *
 * While the event is set, a thread that calls avr_thread_event_wait()
 * or avr_thread_event_wait_and_clear() will return immediately.
 */
void avr_thread_event_set_wake_all(volatile avr_thread_event* event);

/*
 * avr_thread_event_clear()
 *
 * Clears the event.
 *
 * While the event is clear, a thread that calls
 * avr_thread_event_wait() or avr_thread_event_wait_and_clear() will
 * block until the event is set.
 */
void avr_thread_event_clear(volatile avr_thread_event* event);

/*
 * avr_thread_event_wait()
 *
 * If the event is set when this function is called this function
 * returns 1 immediately.  Otherwise, the thread is blocked until the
 * event is set by another thread.  If the event is set by another
 * thread while blocked, this function will return 1.  While blocked,
 * if the given number of ticks elapses without the event being set by
 * another thread, the function returns 0.
 *
 * If you pass zero ticks, the function will wait indefinitely until
 * the event becomes set.
 */
uint8_t	avr_thread_event_wait(volatile avr_thread_event* event,
                              uint16_t ticks);

/*
 * avr_thread_event_wait_and_clear()
 *
 * If the event is set when this function is called, the event is
 * cleared and this function returns 1 immediately.  Otherwise, the
 * thread is blocked until the event is set by another thread.  If the
 * event is set by another thread while blocked, the event will be
 * cleared and this function will return 1.  While blocked, if the given
 * number of ticks elapses without the event being set by another
 * thread, the function returns 0.
 *
 * If you pass zero ticks, the function will wait indefinitely until
 * the event becomes set.
 */
uint8_t	avr_thread_event_wait_and_clear(volatile avr_thread_event* event,
					uint16_t ticks);

/*
 * avr_thread_isr_start()
 *
 * Call this function at the beginning of the interrupt service routine
 * used to provide the preemptive multitasking.
 *
 * This function saves registers on the stack in preparation for
 * a task switch.
 *
 * The isr must be declared with the attribute "naked".
 */
void avr_thread_isr_start(void);

/*
 * avr_thread_isr_end()
 *
 * Call this function at the end of the interrupt service routine
 * used to provide the preemptive multitasking.
 *
 * This function does not return.
 */
void avr_thread_isr_end(void);

#endif
