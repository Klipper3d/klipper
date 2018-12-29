// SAM3/SAM4 timer interrupt scheduling
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "board/timer_irq.h" // timer_dispatch_many
#include "command.h" // DECL_SHUTDOWN
#include "internal.h" // TC0
#include "sched.h" // DECL_INIT

// Set the next irq time
static void
timer_set(uint32_t value)
{
    TC0->TC_CHANNEL[0].TC_RA = value;
}

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void)
{
    return TC0->TC_CHANNEL[0].TC_CV;
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    timer_set(timer_read_time() + 50);
    TC0->TC_CHANNEL[0].TC_SR; // read to clear irq pending
}

void
timer_init(void)
{
    TcChannel *tc = &TC0->TC_CHANNEL[0];
    // Reset the timer
    tc->TC_CCR = TC_CCR_CLKDIS;
    tc->TC_IDR = 0xFFFFFFFF;
    // Enable it
    enable_pclock(ID_TC0);
    tc->TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP | TC_CMR_TCCLKS_TIMER_CLOCK1;
    tc->TC_IER = TC_IER_CPAS;
    NVIC_SetPriority(TC0_IRQn, 2);
    NVIC_EnableIRQ(TC0_IRQn);
    timer_kick();
    tc->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}
DECL_INIT(timer_init);

// IRQ handler
void __visible __aligned(16) // aligning helps stabilize perf benchmarks
TC0_Handler(void)
{
    irq_disable();
    uint32_t status = TC0->TC_CHANNEL[0].TC_SR; // read to clear irq pending
    if (likely(status & TC_SR_CPAS)) {
        uint32_t next = timer_dispatch_many();
        timer_set(next);
    }
    irq_enable();
}
