// Handling of timers on linux systems
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <time.h> // struct timespec
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/misc.h" // timer_from_us
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_CONSTANT
#include "internal.h" // console_sleep
#include "sched.h" // DECL_INIT


/****************************************************************
 * Timespec helpers
 ****************************************************************/

static uint32_t last_read_time_counter;
static struct timespec last_read_time, next_wake_time;
static time_t start_sec;

// Compare two 'struct timespec' times
static inline uint8_t
timespec_is_before(struct timespec ts1, struct timespec ts2)
{
    return (ts1.tv_sec < ts2.tv_sec
            || (ts1.tv_sec == ts2.tv_sec && ts1.tv_nsec < ts2.tv_nsec));
}

// Convert a 'struct timespec' to a counter value
static inline uint32_t
timespec_to_time(struct timespec ts)
{
    return ((ts.tv_sec - start_sec) * CONFIG_CLOCK_FREQ
            + ts.tv_nsec / NSECS_PER_TICK);
}

// Convert an internal time counter to a 'struct timespec'
static inline struct timespec
timespec_from_time(uint32_t time)
{
    int32_t counter_diff = time - last_read_time_counter;
    struct timespec ts;
    ts.tv_sec = last_read_time.tv_sec;
    ts.tv_nsec = last_read_time.tv_nsec + counter_diff * NSECS_PER_TICK;
    if ((unsigned long)ts.tv_nsec >= NSECS) {
        if (ts.tv_nsec < 0) {
            ts.tv_sec--;
            ts.tv_nsec += NSECS;
        } else {
            ts.tv_sec++;
            ts.tv_nsec -= NSECS;
        }
    }
    return ts;
}

// Add a given number of nanoseconds to a 'struct timespec'
static inline struct timespec
timespec_add(struct timespec ts, long ns)
{
    ts.tv_nsec += ns;
    if (ts.tv_nsec >= NSECS) {
        ts.tv_sec++;
        ts.tv_nsec -= NSECS;
    }
    return ts;
}

// Return the current time
static struct timespec
timespec_read(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts;
}

// Periodically update last_read_time / last_read_time_counter
void
timespec_update(void)
{
    last_read_time = timespec_read();
    last_read_time_counter = timespec_to_time(last_read_time);
}
DECL_TASK(timespec_update);

// Check if a given time has past
int
timer_check_periodic(struct timespec *ts)
{
    if (timespec_is_before(next_wake_time, *ts))
        return 0;
    *ts = next_wake_time;
    ts->tv_sec += 2;
    return 1;
}


/****************************************************************
 * Timers
 ****************************************************************/

DECL_CONSTANT("CLOCK_FREQ", CONFIG_CLOCK_FREQ);

// Return the number of clock ticks for a given number of microseconds
uint32_t
timer_from_us(uint32_t us)
{
    return us * (CONFIG_CLOCK_FREQ / 1000000);
}

// Return true if time1 is before time2.  Always use this function to
// compare times as regular C comparisons can fail if the counter
// rolls over.
uint8_t
timer_is_before(uint32_t time1, uint32_t time2)
{
    return (int32_t)(time1 - time2) < 0;
}

// Return the current time (in clock ticks)
uint32_t
timer_read_time(void)
{
    return timespec_to_time(timespec_read());
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    next_wake_time = last_read_time;
}

static struct timespec timer_repeat_until;
#define TIMER_IDLE_REPEAT_NS 500000
#define TIMER_REPEAT_NS 100000

#define TIMER_MIN_TRY_NS 1000
#define TIMER_DEFER_REPEAT_NS 5000

// Invoke timers
static void
timer_dispatch(void)
{
    struct timespec tru = timer_repeat_until;
    for (;;) {
        // Run the next software timer
        uint32_t next = sched_timer_dispatch();
        struct timespec nt = timespec_from_time(next);

        struct timespec now = timespec_read();
        if (!timespec_is_before(nt, timespec_add(now, TIMER_MIN_TRY_NS))) {
            // Schedule next timer normally.
            next_wake_time = nt;
            return;
        }

        if (unlikely(timespec_is_before(tru, now))) {
            // Check if there are too many repeat timers
            if (unlikely(timespec_is_before(timespec_add(nt, 100000000), now)))
                try_shutdown("Rescheduled timer in the past");
            if (sched_tasks_busy()) {
                timer_repeat_until = timespec_add(now, TIMER_REPEAT_NS);
                next_wake_time = timespec_add(now, TIMER_DEFER_REPEAT_NS);
                return;
            }
            timer_repeat_until = timespec_add(now, TIMER_IDLE_REPEAT_NS);
        }

        // Next timer in the past or near future - wait for it to be ready
        while (unlikely(timespec_is_before(now, nt)))
            now = timespec_read();
    }
}

void
timer_init(void)
{
    start_sec = timespec_read().tv_sec;
    timer_repeat_until.tv_sec = start_sec + 2;
    timespec_update();
    timer_kick();
}
DECL_INIT(timer_init);


/****************************************************************
 * Interrupt wrappers
 ****************************************************************/

void
irq_disable(void)
{
}

void
irq_enable(void)
{
}

irqstatus_t
irq_save(void)
{
    return 0;
}

void
irq_restore(irqstatus_t flag)
{
}

void
irq_wait(void)
{
    console_sleep(next_wake_time);
}

void
irq_poll(void)
{
    if (!timespec_is_before(timespec_read(), next_wake_time))
        timer_dispatch();
}
