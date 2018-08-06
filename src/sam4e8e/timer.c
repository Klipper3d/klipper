// SAM4e8e timer port
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// CMSIS
#include "sam.h"
// Klipper
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "board/timer_irq.h" // timer_dispatch_many
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
    TC0->TC_CHANNEL[0].TC_SR;
}

void
timer_init(void)
{
    if ((PMC->PMC_PCSR0 & (1u << ID_TC0)) == 0) {
        PMC->PMC_PCER0 = 1 << ID_TC0;
    }
    TcChannel *tc_channel = &TC0->TC_CHANNEL[0];
    tc_channel->TC_CCR = TC_CCR_CLKDIS;
    tc_channel->TC_IDR = 0xFFFFFFFF;
    tc_channel->TC_SR;
    tc_channel->TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP | TC_CMR_TCCLKS_TIMER_CLOCK1;
    tc_channel->TC_IER = TC_IER_CPAS;
    NVIC_SetPriority(TC0_IRQn, 1);
    NVIC_EnableIRQ(TC0_IRQn);
    timer_kick();
    tc_channel->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}
DECL_INIT(timer_init);

// IRQ handler
void __visible __aligned(16) // aligning helps stabilize perf benchmarks
TC0_Handler(void)
{
    irq_disable();
    uint32_t status = TC0->TC_CHANNEL[0].TC_SR;
    if (likely(status & TC_SR_CPAS)) {
        uint32_t next = timer_dispatch_many();
        timer_set(next);
    }
    irq_enable();
}
