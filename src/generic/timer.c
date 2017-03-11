// Generic interrupt based timer helper functions
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_from_us
#include "command.h" // shutdown
#include "sched.h" // sched_timer_kick

// In order to use this code the board must still define
// timer_read_time().  In addition, it must also provide a timer_set()
// function.
void timer_set(uint32_t next);

DECL_CONSTANT(CLOCK_FREQ, CONFIG_CLOCK_FREQ);

// Return the number of clock ticks for a given number of microseconds
uint32_t
timer_from_us(uint32_t us)
{
    return us * (CONFIG_CLOCK_FREQ / 1000000);
}

// Called by main code once every millisecond.  (IRQs disabled.)
void
timer_periodic(void)
{
}

static uint32_t timer_repeat_until;
#define TIMER_IDLE_REPEAT_TICKS timer_from_us(500)
#define TIMER_REPEAT_TICKS timer_from_us(100)

#define TIMER_MIN_TRY_TICKS timer_from_us(1)
#define TIMER_DEFER_REPEAT_TICKS timer_from_us(5)

// Set the next timer wake time (in absolute clock ticks) or return 1
// if the next timer is too close to schedule.  Caller must disable
// irqs.
uint8_t
timer_try_set_next(uint32_t next)
{
    uint32_t now = timer_read_time();
    int32_t diff = next - now;
    if (diff > (int32_t)TIMER_MIN_TRY_TICKS)
        // Schedule next timer normally.
        goto done;

    // Next timer is in the past or near future - can't reschedule to it
    if (likely(sched_is_before(now, timer_repeat_until))) {
        // Can run more timers from this irq; briefly allow irqs
        irq_enable();
        while (diff >= 0) {
            // Next timer is in the near future - wait for time to occur
            now = timer_read_time();
            diff = next - now;
        }
        irq_disable();
        return 0;
    }
    if (diff < (int32_t)(-timer_from_us(1000)))
        goto fail;

    // Too many repeat timers from a single interrupt - force a pause
    timer_repeat_until = now + TIMER_REPEAT_TICKS;
    next = now + TIMER_DEFER_REPEAT_TICKS;

done:
    timer_set(next);
    return 1;
fail:
    shutdown("Rescheduled timer in the past");
}

// Periodic background task that temporarily boosts priority of
// timers.  This helps prioritize timers when tasks are idling.
static void
timer_task(void)
{
    irq_disable();
    timer_repeat_until = timer_read_time() + TIMER_IDLE_REPEAT_TICKS;
    irq_enable();
}
DECL_TASK(timer_task);
