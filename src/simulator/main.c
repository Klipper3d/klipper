// Main starting point for host simulator.
//
// Copyright (C) 2014  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "sched.h" // sched_kick

uint8_t Interrupt_off;


/****************************************************************
 * Timers
 ****************************************************************/

uint32_t
timer_from_ms(uint32_t ms)
{
    return 0; // XXX
}

uint32_t
timer_read_time(void)
{
    return 0; // XXX
}

void
timer_set_next(uint32_t next)
{
}


/****************************************************************
 * Turn stdin/stdout into serial console
 ****************************************************************/

static char lastread;

uint8_t
serial_canpop(void)
{
    int c = read(STDIN_FILENO, &lastread, 1);
    return c >= 1;
}

char
serial_pop(void)
{
    return lastread;
}

void
serial_push(char data)
{
    putchar(data);
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

    sched_kick();
    return 0;
}
