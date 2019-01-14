// SAMD51 timer interrupt scheduling
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "board/timer_irq.h" // timer_dispatch_many
#include "internal.h" // enable_pclock
#include "sched.h" // DECL_INIT

// Set the next irq time
static void
timer_set(uint32_t value)
{
    TC0->COUNT32.CC[0].reg = value;
    TC0->COUNT32.INTFLAG.reg = TC_INTFLAG_MC0;
}

// Return the current time (in absolute clock ticks).
uint32_t __always_inline
timer_read_time(void)
{
    // Need to request a COUNT update and then delay for it to be ready
    TC0->COUNT32.CTRLBSET.reg = TC_CTRLBSET_CMD_READSYNC;
    TC0->COUNT32.COUNT.reg;
    TC0->COUNT32.COUNT.reg;
    return TC0->COUNT32.COUNT.reg;
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    timer_set(timer_read_time() + 50);
}

void
timer_init(void)
{
    // Supply power and clock to the timer
    enable_pclock(TC0_GCLK_ID, ID_TC0);
    enable_pclock(TC1_GCLK_ID, ID_TC1);

    // Configure the timer
    TcCount32 *tc = &TC0->COUNT32;
    irqstatus_t flag = irq_save();
    tc->CTRLA.reg = 0;
    tc->CTRLA.reg = TC_CTRLA_MODE_COUNT32;
    NVIC_SetPriority(TC0_IRQn, 2);
    NVIC_EnableIRQ(TC0_IRQn);
    tc->INTENSET.reg = TC_INTENSET_MC0;
    tc->COUNT.reg = 0;
    timer_kick();
    tc->CTRLA.reg = (TC_CTRLA_MODE_COUNT32 | TC_CTRLA_PRESCALER_DIV8
                     | TC_CTRLA_ENABLE);
    irq_restore(flag);
}
DECL_INIT(timer_init);

// IRQ handler
void __visible __aligned(16) // aligning helps stabilize perf benchmarks
TC0_Handler(void)
{
    irq_disable();
    uint32_t next = timer_dispatch_many();
    timer_set(next);
    irq_enable();
}
