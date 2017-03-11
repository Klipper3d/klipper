// SAM3x8e timer interrupt scheduling
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_SHUTDOWN
#include "sam3x8e.h" // TC0
#include "sched.h" // sched_timer_kick

// IRQ handler
void __visible
TC0_Handler(void)
{
    irq_disable();
    uint32_t status = TC0->TC_CHANNEL[0].TC_SR; // read to clear irq pending
    if (likely(status & TC_SR_CPAS))
        sched_timer_kick();
    irq_enable();
}

// Set the next irq time
void
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

static void
timer_init(void)
{
    TcChannel *tc = &TC0->TC_CHANNEL[0];
    // Reset the timer
    tc->TC_CCR = TC_CCR_CLKDIS;
    tc->TC_IDR = 0xFFFFFFFF;
    tc->TC_SR;
    // Enable it
    PMC->PMC_PCER0 = 1 << ID_TC0;
    tc->TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP | TC_CMR_TCCLKS_TIMER_CLOCK1;
    tc->TC_IER = TC_IER_CPAS;
    NVIC_SetPriority(TC0_IRQn, 1);
    NVIC_EnableIRQ(TC0_IRQn);
    timer_set(1);
    tc->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}
DECL_INIT(timer_init);

static void
timer_shutdown(void)
{
    // Reenable timer irq
    timer_set(timer_read_time() + 50);
    TC0->TC_CHANNEL[0].TC_SR; // read to clear irq pending
}
DECL_SHUTDOWN(timer_shutdown);
