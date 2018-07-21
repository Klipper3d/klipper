// Sam4e8e timer interrupt scheduling
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// Klipper
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "board/timer_irq.h" // timer_dispatch_many
#include "sched.h" // DECL_INIT

// ASF
#include "tc/tc.h" // tc_*
#include "pmc/pmc.h" // pmc_enable_periph_clk

// Set the next irq time
static void
timer_set(uint32_t value)
{
    tc_write_ra(TC0, 0, value);
}

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void)
{
    return tc_read_cv(TC0, 0);
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    timer_set(timer_read_time() + 50);
    tc_get_status(TC0, 0);
}

void
timer_init(void)
{
    pmc_enable_periph_clk(ID_TC0);
    tc_init(TC0, 0, TC_CMR_WAVE | TC_CMR_WAVSEL_UP | TC_CMR_TCCLKS_TIMER_CLOCK1);
    tc_enable_interrupt(TC0, 0, TC_IER_CPAS);
    NVIC_SetPriority(TC0_IRQn, 1);
    NVIC_EnableIRQ(TC0_IRQn);
    timer_kick();
    tc_start(TC0, 0);
}
DECL_INIT(timer_init);

// IRQ handler
void __visible __aligned(16) // aligning helps stabilize perf benchmarks
TC0_Handler(void)
{
    irq_disable();
    uint32_t status = tc_get_status(TC0, 0);
    if (likely(status & TC_SR_CPAS)) {
        uint32_t next = timer_dispatch_many();
        timer_set(next);
    }
    irq_enable();
}
