// Basic scheduling functions and startup/shutdown code.
//
// Copyright (C) 2014  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include <stdarg.h> // va_list
#include <stddef.h> // NULL
#include "board/irq.h" // irq_disable
#include "board/timer.h" // timer_from_ms
#include "command.h" // voutput_P
#include "sched.h" // sched_from_ms


/****************************************************************
 * Timers
 ****************************************************************/

static uint32_t millis;

// Default millisecond timer.  This timer counts milliseconds.  It
// also simplifies the timer code by ensuring there is always at least
// one timer on the timer list and that there is always a timer not
// more than 1 ms in the future.
static void
ms_event(struct timer *t)
{
    millis++;
    t->waketime += sched_from_ms(1);
    sched_timer(t);
}

static struct timer ms_timer = {
    .func = ms_event
};

// Return the number of clock ticks for a given number of milliseconds
uint32_t
sched_from_ms(uint32_t ms)
{
    return timer_from_ms(ms);
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
static uint8_t in_timer_dispatch;

// Schedule a function call at a supplied time.
void
sched_timer(struct timer *add)
{
    uint8_t flag = irq_save();

    // Add to timer list.
    struct timer **timep = &timer_list;
    for (;;) {
        struct timer *t = *timep;
        if (!t || sched_is_before(add->waketime, t->waketime)) {
            add->next = t;
            *timep = add;
            break;
        }
        timep = &t->next;
    }

    // Reschedule timer if necessary.
    if (! in_timer_dispatch && timer_list == add) {
        if (sched_is_before(timer_read_time() + TIMER_MIN_TICKS, add->waketime))
            timer_set_next(add->waketime);
        else
            // Timer needs to run now.
            sched_timer_kick();
    }

    irq_restore(flag);
}

// Invoke timers - called from board timer irq code.
void
sched_timer_kick(void)
{
    in_timer_dispatch = 1;

    for (;;) {
        uint32_t curtime = timer_read_time();
        int32_t diff = curtime - timer_list->waketime;
        if (diff < 0) {
            // Done with timers - schedule next timer.
            if (diff > -TIMER_MIN_TICKS)
                // Timer too close to reschedule - retry.
                continue;
            timer_set_next(timer_list->waketime);
            break;
        }

        // Remove timer from list and invoke callback.
        struct timer *t = timer_list;
        timer_list = t->next;
        t->next = NULL;
        t->func(t);
    }

    in_timer_dispatch = 0;
}

// Shutdown all user timers on an emergency stop.
static void
timer_shutdown(void)
{
    timer_list = &ms_timer;
    ms_timer.next = NULL;
}
DECL_SHUTDOWN(timer_shutdown);


/****************************************************************
 * Shutdown processing
 ****************************************************************/

static uint8_t IsShutdown;

// Return true if the machine is in an emergency stop state
uint8_t
sched_is_shutdown(void)
{
    return IsShutdown;
}

// Invoke all init functions (as declared by DECL_SHUTDOWN)
static void
run_shutdown(void)
{
    struct callback_handler *p;
    foreachdecl(p, shutdownfuncs) {
        void (*func)(void) = READP(p->func);
        func();
    }
}

// Shutdown the machine and send a debug message.
void
shutdown_P(const char *fmt_P, ...)
{
    uint8_t flag = irq_save();
    IsShutdown = 1;
    run_shutdown();
    irq_restore(flag);

    va_list args;
    va_start(args, fmt_P);
    voutput_P(fmt_P, args);
    va_end(args);
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
sched_kick(void)
{
    run_init();
    output("XXX\n"); // XXX
    for (;;)
        run_task();
}
