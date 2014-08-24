// AVR timer interrupt scheduling code.
//
// Copyright (C) 2014  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include <avr/interrupt.h>
#include "sched.h"
#include "timer.h"

// Return the number of clock ticks for a given number of milliseconds
uint32_t
timer_from_ms(uint32_t ms)
{
    return ms * (F_CPU / 1000);
}

static uint32_t timer_last;

// Return the current time (in absolute clock ticks).  Caller must
// disable irqs.
uint32_t
timer_read_time(void)
{
    uint16_t cur = TCNT1;
    uint32_t next = (timer_last & ~0xffffL) | cur;
    if (cur < (uint16_t)timer_last)
        next += 0x10000;
    timer_last = next;
    return next;
}

ISR(TIMER1_COMPA_vect)
{
    sched_timer_kick();
}

// Set the next timer wake time (in absolute clock ticks).  Caller
// must disable irqs.  The caller should not schedule a time more than
// a few milliseconds in the future.  The caller must not schedule a
// time in the past or within TIMER_MIN_TICKS of the current time.
void
timer_set_next(uint32_t next)
{
    OCR1A = (uint16_t)next;
}

static void
timer_init(void)
{
    // no outputs
    TCCR1A = 0;
    // Normal Mode
    TCCR1B = 1<<CS10;
    // enable interrupt
    TIMSK1 = 1<<OCIE1A;
}
DECL_INIT(timer_init);
