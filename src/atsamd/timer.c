// SAMD21 timer interrupt scheduling
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "board/timer_irq.h" // timer_dispatch_many
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT

// Set the next irq time
static void
timer_set(uint32_t value)
{
    TC4->COUNT32.CC[0].reg = value;
    TC4->COUNT32.INTFLAG.reg = TC_INTFLAG_MC0;
}

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void)
{
    return TC4->COUNT32.COUNT.reg;
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    timer_set(timer_read_time() + 50);
}

// IRQ handler
void __aligned(16) // aligning helps stabilize perf benchmarks
TC4_Handler(void)
{
    irq_disable();
    uint32_t next = timer_dispatch_many();
    timer_set(next);
    irq_enable();
}

void
timer_init(void)
{
    // Supply power and clock to the timer
    enable_pclock(TC3_GCLK_ID, ID_TC3);
    enable_pclock(TC4_GCLK_ID, ID_TC4);

    // Configure the timer
    TcCount32 *tc = &TC4->COUNT32;
    irqstatus_t flag = irq_save();
    tc->CTRLA.reg = 0;
    tc->CTRLA.reg = TC_CTRLA_MODE_COUNT32;
    armcm_enable_irq(TC4_Handler, TC4_IRQn, 2);
    tc->INTENSET.reg = TC_INTENSET_MC0;
    tc->COUNT.reg = 0;
    timer_kick();
    tc->CTRLA.reg = TC_CTRLA_MODE_COUNT32 | TC_CTRLA_ENABLE;
    irq_restore(flag);
}
DECL_INIT(timer_init);
