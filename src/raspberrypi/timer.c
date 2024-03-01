// Handling of timers on linux systems
//
// Copyright (C) 2017-2021  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2024 Liam Powell <klipper@liampwll.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <time.h> // struct timespec
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/io.h" // readl
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_from_us
#include "command.h" // DECL_CONSTANT
#include "internal.h" // console_sleep
#include "sched.h" // DECL_INIT

// Global storage for timer handling
static struct {
    uint32_t start;
    // Last time reported by timer_read_time()
    uint32_t last_read_time;
    // Flags for tracking irq_enable()/irq_disable()
    uint32_t must_wake_timers;
    // Time of next software timer (also used to convert from ticks to systime)
    uint32_t next_wake;
} TimerInfo;

/****************************************************************
 * Timers
 ****************************************************************/

DECL_CONSTANT("CLOCK_FREQ", CONFIG_CLOCK_FREQ);

// Check if a given time has past
int
timer_check_periodic(uint32_t *ts)
{
    uint32_t lrt = TimerInfo.last_read_time;
    if (timer_is_before(lrt, *ts))
        return 0;
    *ts = lrt + timer_from_us(2000000);
    return 1;
}

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
    uint64_t cntvct;
    asm volatile ("isb; mrs %0, cntvct_el0;" : "=r"(cntvct) :: "memory");
    TimerInfo.last_read_time = (uint32_t)cntvct - TimerInfo.start;
    return TimerInfo.last_read_time;
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    TimerInfo.must_wake_timers = 1;
}

#define TIMER_IDLE_REPEAT_COUNT 100
#define TIMER_REPEAT_COUNT 20

#define TIMER_MIN_TRY_TICKS timer_from_us(2)

// Invoke timers
static void
timer_dispatch(void)
{
    uint32_t repeat_count = TIMER_REPEAT_COUNT, next;
    for (;;) {
        // Run the next software timer
        next = sched_timer_dispatch();

        repeat_count--;
        uint32_t lrt = TimerInfo.last_read_time;
        if (!timer_is_before(lrt, next) && repeat_count)
            // Can run next timer without overhead of calling timer_read_time()
            continue;

        uint32_t now = timer_read_time();
        int32_t diff = next - now;
        if (diff > (int32_t)TIMER_MIN_TRY_TICKS)
            // Schedule next timer normally.
            break;

        if (unlikely(!repeat_count)) {
            // Check if there are too many repeat timers
            if (diff < (int32_t)(-timer_from_us(100000)))
                try_shutdown("Rescheduled timer in the past");
            if (sched_tasks_busy())
                return;
            repeat_count = TIMER_IDLE_REPEAT_COUNT;
        }

        // Next timer in the past or near future - wait for it to be ready
        while (unlikely(diff > 0))
            diff = next - timer_read_time();
    }

    TimerInfo.next_wake = next;
    TimerInfo.must_wake_timers = 0;
}

void
timer_init(void)
{
    TimerInfo.start = 0;
    TimerInfo.start = timer_read_time();
    TimerInfo.next_wake = timer_read_time();
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
    uint32_t current_time = timer_read_time();
    if (!timer_is_before(current_time, TimerInfo.next_wake)) {
        TimerInfo.must_wake_timers = 1;
    }

    if (!TimerInfo.must_wake_timers) {
        console_sleep();
    }

    irq_poll();
}

void
irq_poll(void)
{
    if (TimerInfo.must_wake_timers) {
        timer_dispatch();
    }
}
