// Basic scheduling functions and startup/shutdown code.
//
// Copyright (C) 2016-2024  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <setjmp.h> // setjmp
#include "autoconf.h" // CONFIG_*
#include "basecmd.h" // stats_update
#include "board/io.h" // readb
#include "board/irq.h" // irq_save
#include "board/misc.h" // timer_from_us
#include "board/pgm.h" // READP
#include "command.h" // shutdown
#include "sched.h" // sched_check_periodic
#include "stepper.h" // stepper_event

static struct timer periodic_timer, sentinel_timer, deleted_timer;

static struct {
    struct timer *timer_list, *last_insert;
    int8_t tasks_status, tasks_busy;
    uint8_t shutdown_status, shutdown_reason;
} SchedStatus = {.timer_list = &periodic_timer, .last_insert = &periodic_timer};


/****************************************************************
 * Timers
 ****************************************************************/

// The periodic_timer simplifies the timer code by ensuring there is
// always a timer on the timer list and that there is always a timer
// not far in the future.
static uint_fast8_t
periodic_event(struct timer *t)
{
    // Make sure the stats task runs periodically
    sched_wake_tasks();
    // Reschedule timer
    periodic_timer.waketime += timer_from_us(100000);
    sentinel_timer.waketime = periodic_timer.waketime + 0x80000000;
    return SF_RESCHEDULE;
}

static struct timer periodic_timer = {
    .func = periodic_event,
    .next = &sentinel_timer,
};

// The sentinel timer is always the last timer on timer_list - its
// presence allows the code to avoid checking for NULL while
// traversing timer_list.  Since sentinel_timer.waketime is always
// equal to (periodic_timer.waketime + 0x80000000) any added timer
// must always have a waketime less than one of these two timers.
static uint_fast8_t
sentinel_event(struct timer *t)
{
    shutdown("sentinel timer called");
}

static struct timer sentinel_timer = {
    .func = sentinel_event,
    .waketime = 0x80000000,
};

// Find position for a timer in timer_list and insert it
static void __always_inline
insert_timer(struct timer *pos, struct timer *t, uint32_t waketime)
{
    struct timer *prev;
    for (;;) {
        prev = pos;
        if (CONFIG_MACH_AVR)
            // micro optimization for AVR - reduces register pressure
            asm("" : "+r"(prev));
        pos = pos->next;
        if (timer_is_before(waketime, pos->waketime))
            break;
    }
    t->next = pos;
    prev->next = t;
}

// Schedule a function call at a supplied time.
void
sched_add_timer(struct timer *add)
{
    uint32_t waketime = add->waketime;
    irqstatus_t flag = irq_save();
    struct timer *tl = SchedStatus.timer_list;
    if (unlikely(timer_is_before(waketime, tl->waketime))) {
        // This timer is before all other scheduled timers
        if (timer_is_before(waketime, timer_read_time()))
            try_shutdown("Timer too close");
        if (tl == &deleted_timer)
            add->next = deleted_timer.next;
        else
            add->next = tl;
        deleted_timer.waketime = waketime;
        deleted_timer.next = add;
        SchedStatus.timer_list = &deleted_timer;
        timer_kick();
    } else {
        insert_timer(tl, add, waketime);
    }
    irq_restore(flag);
}

// The deleted timer is used when deleting an active timer.
static uint_fast8_t
deleted_event(struct timer *t)
{
    return SF_DONE;
}

static struct timer deleted_timer = {
    .func = deleted_event,
};

// Remove a timer that may be live.
void
sched_del_timer(struct timer *del)
{
    irqstatus_t flag = irq_save();
    if (SchedStatus.timer_list == del) {
        // Deleting the next active timer - replace with deleted_timer
        deleted_timer.waketime = del->waketime;
        deleted_timer.next = del->next;
        SchedStatus.timer_list = &deleted_timer;
    } else {
        // Find and remove from timer list (if present)
        struct timer *pos;
        for (pos = SchedStatus.timer_list; pos->next; pos = pos->next) {
            if (pos->next == del) {
                pos->next = del->next;
                break;
            }
        }
    }
    if (SchedStatus.last_insert == del)
        SchedStatus.last_insert = &periodic_timer;
    irq_restore(flag);
}

// Invoke the next timer - called from board hardware irq code.
unsigned int
sched_timer_dispatch(void)
{
    // Invoke timer callback
    struct timer *t = SchedStatus.timer_list;
    uint_fast8_t res;
    uint32_t updated_waketime;
    if (CONFIG_INLINE_STEPPER_HACK && likely(!t->func)) {
        res = stepper_event(t);
        updated_waketime = t->waketime;
    } else {
        res = t->func(t);
        updated_waketime = t->waketime;
    }

    // Update timer_list (rescheduling current timer if necessary)
    unsigned int next_waketime = updated_waketime;
    if (unlikely(res == SF_DONE)) {
        next_waketime = t->next->waketime;
        SchedStatus.timer_list = t->next;
        if (SchedStatus.last_insert == t)
            SchedStatus.last_insert = t->next;
    } else if (!timer_is_before(updated_waketime, t->next->waketime)) {
        next_waketime = t->next->waketime;
        SchedStatus.timer_list = t->next;
        struct timer *pos = SchedStatus.last_insert;
        if (timer_is_before(updated_waketime, pos->waketime))
            pos = SchedStatus.timer_list;
        insert_timer(pos, t, updated_waketime);
        SchedStatus.last_insert = t;
    }

    return next_waketime;
}

// Remove all user timers
void
sched_timer_reset(void)
{
    SchedStatus.timer_list = &deleted_timer;
    deleted_timer.waketime = periodic_timer.waketime;
    deleted_timer.next = SchedStatus.last_insert = &periodic_timer;
    periodic_timer.next = &sentinel_timer;
    timer_kick();
}


/****************************************************************
 * Tasks
 ****************************************************************/

#define TS_IDLE      -1
#define TS_REQUESTED 0
#define TS_RUNNING   1

// Note that at least one task is ready to run
void
sched_wake_tasks(void)
{
    SchedStatus.tasks_status = TS_REQUESTED;
}

// Check if tasks busy (called from low-level timer dispatch code)
uint8_t
sched_check_set_tasks_busy(void)
{
    // Return busy if tasks never idle between two consecutive calls
    if (SchedStatus.tasks_busy >= TS_REQUESTED)
        return 1;
    SchedStatus.tasks_busy = SchedStatus.tasks_status;
    return 0;
}

// Note that a task is ready to run
void
sched_wake_task(struct task_wake *w)
{
    sched_wake_tasks();
    writeb(&w->wake, 1);
}

// Check if a task is ready to run (as indicated by sched_wake_task)
uint8_t
sched_check_wake(struct task_wake *w)
{
    if (!readb(&w->wake))
        return 0;
    writeb(&w->wake, 0);
    return 1;
}

// Main task dispatch loop
static void
run_tasks(void)
{
    uint32_t start = timer_read_time();
    for (;;) {
        // Check if can sleep
        irq_poll();
        if (SchedStatus.tasks_status != TS_REQUESTED) {
            start -= timer_read_time();
            irq_disable();
            if (SchedStatus.tasks_status != TS_REQUESTED) {
                // Sleep processor (only run timers) until tasks woken
                SchedStatus.tasks_status = SchedStatus.tasks_busy = TS_IDLE;
                do {
                    irq_wait();
                } while (SchedStatus.tasks_status != TS_REQUESTED);
            }
            irq_enable();
            start += timer_read_time();
        }
        SchedStatus.tasks_status = TS_RUNNING;

        // Run all tasks
        extern void ctr_run_taskfuncs(void);
        ctr_run_taskfuncs();

        // Update statistics
        uint32_t cur = timer_read_time();
        stats_update(start, cur);
        start = cur;
    }
}


/****************************************************************
 * Shutdown processing
 ****************************************************************/

// Return true if the machine is in an emergency stop state
uint8_t
sched_is_shutdown(void)
{
    return !!SchedStatus.shutdown_status;
}

// Transition out of shutdown state
void
sched_clear_shutdown(void)
{
    if (!SchedStatus.shutdown_status)
        shutdown("Shutdown cleared when not shutdown");
    if (SchedStatus.shutdown_status == 2)
        // Ignore attempt to clear shutdown if still processing shutdown
        return;
    SchedStatus.shutdown_status = 0;
}

// Invoke all shutdown functions (as declared by DECL_SHUTDOWN)
static void
run_shutdown(int reason)
{
    irq_disable();
    uint32_t cur = timer_read_time();
    if (!SchedStatus.shutdown_status)
        SchedStatus.shutdown_reason = reason;
    SchedStatus.shutdown_status = 2;
    sched_timer_reset();
    extern void ctr_run_shutdownfuncs(void);
    ctr_run_shutdownfuncs();
    SchedStatus.shutdown_status = 1;
    irq_enable();

    sendf("shutdown clock=%u static_string_id=%hu", cur
          , SchedStatus.shutdown_reason);
}

// Report the last shutdown reason code
void
sched_report_shutdown(void)
{
    sendf("is_shutdown static_string_id=%hu", SchedStatus.shutdown_reason);
}

// Shutdown the machine if not already in the process of shutting down
void __always_inline
sched_try_shutdown(uint_fast8_t reason)
{
    if (!SchedStatus.shutdown_status)
        sched_shutdown(reason);
}

static jmp_buf shutdown_jmp;

// Force the machine to immediately run the shutdown handlers
void
sched_shutdown(uint_fast8_t reason)
{
    irq_disable();
    longjmp(shutdown_jmp, reason);
}


/****************************************************************
 * Startup
 ****************************************************************/

// Main loop of program
void
sched_main(void)
{
    extern void ctr_run_initfuncs(void);
    ctr_run_initfuncs();

    sendf("starting");

    irq_disable();
    int ret = setjmp(shutdown_jmp);
    if (ret)
        run_shutdown(ret);
    irq_enable();

    run_tasks();
}
