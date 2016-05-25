// Basic scheduling functions and startup/shutdown code.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <setjmp.h> // setjmp
#include <stdarg.h> // va_list
#include <stddef.h> // NULL
#include "autoconf.h" // CONFIG_*
#include "board/irq.h" // irq_save
#include "board/timer.h" // timer_from_ms
#include "command.h" // shutdown
#include "sched.h" // sched_from_ms
#include "stepper.h" // stepper_event


/****************************************************************
 * Timers
 ****************************************************************/

static uint16_t millis;

// Default millisecond timer.  This timer counts milliseconds.  It
// also simplifies the timer code by ensuring there is always at least
// one timer on the timer list and that there is always a timer not
// more than 1 ms in the future.
static uint8_t
ms_event(struct timer *t)
{
    millis++;
    timer_periodic();
    t->waketime += sched_from_ms(1);
    return SF_RESCHEDULE;
}

static struct timer ms_timer = {
    .func = ms_event
};

// Check if ready for a recurring periodic event
uint8_t
sched_check_periodic(uint16_t time, uint16_t *pnext)
{
    uint16_t next = *pnext, cur;
    uint8_t flag = irq_save();
    cur = millis;
    irq_restore(flag);
    if ((int16_t)(cur - next) < 0)
        return 0;
    *pnext = cur + time;
    return 1;
}

// Return the number of clock ticks for a given number of milliseconds
uint32_t
sched_from_ms(uint32_t ms)
{
    return timer_from_ms(ms);
}

// Return the current time (in clock ticks)
uint32_t
sched_read_time(void)
{
    return timer_read_time();
}

// Return true if time1 is before time2.  Always use this function to
// compare times as regular C comparisons can fail if the counter
// rolls over.
uint8_t
sched_is_before(uint32_t time1, uint32_t time2)
{
    return (int32_t)(time1 - time2) < 0;
}

static struct timer *timer_list = &ms_timer;

// Add a timer to timer list.
static __always_inline void
add_timer(struct timer *add)
{
    struct timer **timep = &timer_list, *t = timer_list;
    while (t && !sched_is_before(add->waketime, t->waketime)) {
        timep = &t->next;
        t = t->next;
    }
    add->next = t;
    *timep = add;
}

// Schedule a function call at a supplied time.
void
sched_timer(struct timer *add)
{
    uint8_t flag = irq_save();
    add_timer(add);

    // Reschedule timer if necessary.
    if (timer_list == add) {
        uint8_t ret = timer_set_next(add->waketime);
        if (ret)
            shutdown("Timer too close");
    }

    irq_restore(flag);
}

// Remove a timer that may be live.
void
sched_del_timer(struct timer *del)
{
    uint8_t flag = irq_save();

    if (timer_list == del) {
        timer_list = del->next;
        timer_set_next(timer_list->waketime);
        irq_restore(flag);
        return;
    }

    // Find and remove from timer list.
    struct timer *prev = timer_list;
    for (;;) {
        struct timer *t = prev->next;
        if (!t)
            break;
        if (t == del) {
            prev->next = del->next;
            break;
        }
        prev = t;
    }

    irq_restore(flag);
}

// Invoke timers - called from board timer irq code.
void
sched_timer_kick(void)
{
    struct timer *t = timer_list;
    for (;;) {
        // Invoke timer callback
        uint8_t res;
        if (CONFIG_INLINE_STEPPER_HACK && likely(!t->func))
            res = stepper_event(t);
        else
            res = t->func(t);

        // Update timer_list (rescheduling current timer if necessary)
        timer_list = t->next;
        if (likely(res))
            add_timer(t);
        t = timer_list;

        // Schedule next timer event (or run next timer if it's ready)
        res = timer_try_set_next(t->waketime);
        if (res)
            break;
    }
}

// Shutdown all user timers on an emergency stop.
static void
timer_shutdown(void)
{
    timer_list = &ms_timer;
    ms_timer.next = NULL;
    timer_set_next(timer_list->waketime);
}
DECL_SHUTDOWN(timer_shutdown);


/****************************************************************
 * Shutdown processing
 ****************************************************************/

static uint16_t shutdown_reason;
static uint8_t shutdown_status;

// Return true if the machine is in an emergency stop state
uint8_t
sched_is_shutdown(void)
{
    return !!shutdown_status;
}

uint16_t
sched_shutdown_reason(void)
{
    return shutdown_reason;
}

// Transition out of shutdown state
void
sched_clear_shutdown(void)
{
    if (!shutdown_status)
        shutdown("Shutdown cleared when not shutdown");
    if (shutdown_status == 2)
        // Ignore attempt to clear shutdown if still processing shutdown
        return;
    shutdown_status = 0;
}

// Invoke all shutdown functions (as declared by DECL_SHUTDOWN)
static void
run_shutdown(void)
{
    shutdown_status = 2;
    struct callback_handler *p;
    foreachdecl(p, shutdownfuncs) {
        void (*func)(void) = READP(p->func);
        func();
    }
    shutdown_status = 1;
    irq_enable();

    sendf("shutdown static_string_id=%hu", shutdown_reason);
}

// Shutdown the machine if not already in the process of shutting down
void
sched_try_shutdown(uint16_t reason)
{
    if (shutdown_status != 2)
        sched_shutdown(reason);
}

static jmp_buf shutdown_jmp;

// Force the machine to immediately run the shutdown handlers
void
sched_shutdown(uint16_t reason)
{
    irq_disable();
    shutdown_reason = reason;
    longjmp(shutdown_jmp, 1);
}


/****************************************************************
 * Startup and background task processing
 ****************************************************************/

// Invoke all init functions (as declared by DECL_INIT)
static void
run_init(void)
{
    struct callback_handler *p;
    foreachdecl(p, initfuncs) {
        void (*func)(void) = READP(p->func);
        func();
    }
}

// Invoke all background task functions (as declared by DECL_TASK)
static void
run_task(void)
{
    struct callback_handler *p;
    foreachdecl(p, taskfuncs) {
        void (*func)(void) = READP(p->func);
        func();
    }
}

// Main loop of program
void
sched_main(void)
{
    run_init();

    int ret = setjmp(shutdown_jmp);
    if (ret)
        run_shutdown();

    for (;;)
        run_task();
}
