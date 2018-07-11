// Example code for running timers in a polling mode
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <time.h> // struct timespec
#include <unistd.h> // usleep
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_from_us
#include "board/timer_irq.h" // timer_dispatch_many
#include "command.h" // DECL_CONSTANT
#include "sched.h" // DECL_INIT

// Helper function that returns the system time as a 32bit counter
static uint32_t
get_system_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    double t = (double)ts.tv_sec + (double)ts.tv_nsec * .000000001;
    return (uint32_t)(t * CONFIG_CLOCK_FREQ);
}


/****************************************************************
 * Timers
 ****************************************************************/

static uint32_t next_wake_time;

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void)
{
    return get_system_time();
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    next_wake_time = timer_read_time();
}

// Invoke timers - called below from irq_poll()
static void
do_timer_dispatch(void)
{
    next_wake_time = timer_dispatch_many();
}

void
timer_init(void)
{
    timer_kick();
}
DECL_INIT(timer_init);


/****************************************************************
 * Interrupts
 ****************************************************************/

// Disable hardware interrupts
void
irq_disable(void)
{
}

// Enable hardware interrupts
void
irq_enable(void)
{
}

// Disable hardware interrupts in not already disabled
irqstatus_t
irq_save(void)
{
    return 0;
}

// Restore hardware interrupts to state from flag returned by irq_save()
void
irq_restore(irqstatus_t flag)
{
}

// Atomically enable hardware interrupts and sleep processor until next irq
void
irq_wait(void)
{
    // XXX - sleep to prevent excessive cpu usage in simulator
    usleep(1);

    irq_poll();
}

// Check if an interrupt is active (used only on architectures that do
// not have hardware interrupts)
void
irq_poll(void)
{
    uint32_t now = timer_read_time();
    if (!timer_is_before(now, next_wake_time))
        do_timer_dispatch();
}
