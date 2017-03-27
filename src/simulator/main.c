// Main starting point for host simulator.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "board/misc.h" // timer_from_us
#include "board/irq.h" // irq_disable
#include "sched.h" // sched_main


/****************************************************************
 * Interrupts
 ****************************************************************/

irqstatus_t Interrupt_off;

void
irq_disable(void)
{
    Interrupt_off = 1;
    barrier();
}

void
irq_enable(void)
{
    barrier();
    Interrupt_off = 0;
}

irqstatus_t
irq_save(void)
{
    irqstatus_t flag = Interrupt_off;
    irq_disable();
    return flag;
}

void
irq_restore(irqstatus_t flag)
{
    barrier();
    Interrupt_off = flag;
}


/****************************************************************
 * Timers
 ****************************************************************/

uint32_t
timer_from_us(uint32_t us)
{
    return 0; // XXX
}

uint8_t
timer_is_before(uint32_t time1, uint32_t time2)
{
    return (int32_t)(time1 - time2) < 0;
}

void
timer_periodic(void)
{
}

uint32_t
timer_read_time(void)
{
    return 0; // XXX
}


/****************************************************************
 * Turn stdin/stdout into serial console
 ****************************************************************/

// XXX
char *
console_get_input(uint8_t *plen)
{
    *plen = 0;
    return NULL;
}

void
console_pop_input(uint8_t len)
{
}

// Return an output buffer that the caller may fill with transmit messages
char *
console_get_output(uint8_t len)
{
    return NULL;
}

// Accept the given number of bytes added to the transmit buffer
void
console_push_output(uint8_t len)
{
}


/****************************************************************
 * Startup
 ****************************************************************/

// Periodically sleep so we don't consume all CPU
static void
simu_pause(void)
{
    // XXX - should check that no timers are present.
    usleep(1);
}
DECL_TASK(simu_pause);

// Main entry point for simulator.
int
main(void)
{
    // Make stdin non-blocking
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);

    sched_main();
    return 0;
}
