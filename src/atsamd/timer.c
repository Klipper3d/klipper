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

#if CONFIG_MACH_SAMC21
 #define TCp TC0
 #define TCp_IRQn TC0_IRQn
 #define TCp_GCLK_ID TC0_GCLK_ID
 #define ID_TCp ID_TC0
 #define TCd_GCLK_ID TC1_GCLK_ID
 #define ID_TCd ID_TC1
#else
 #define TCp TC4
 #define TCp_IRQn TC4_IRQn
 #define TCp_GCLK_ID TC4_GCLK_ID
 #define ID_TCp ID_TC4
 #define TCd_GCLK_ID TC3_GCLK_ID
 #define ID_TCd ID_TC3
#endif

// Set the next irq time
static void
timer_set(uint32_t value)
{
    TCp->COUNT32.CC[0].reg = value;
    TCp->COUNT32.INTFLAG.reg = TC_INTFLAG_MC0;
}

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void)
{
#if CONFIG_MACH_SAMC21
    TCp->COUNT32.CTRLBSET.reg |= TC_CTRLBSET_CMD(TC_CTRLBCLR_CMD_READSYNC_Val);
    while (TCp->COUNT32.SYNCBUSY.reg & TC_SYNCBUSY_COUNT)
        ;
#endif
    return TCp->COUNT32.COUNT.reg;
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    timer_set(timer_read_time() + 50);
}

// IRQ handler
void __aligned(16) // aligning helps stabilize perf benchmarks
TCp_Handler(void)
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
    enable_pclock(TCd_GCLK_ID, ID_TCd);
    enable_pclock(TCp_GCLK_ID, ID_TCp);

    // Configure the timer
    TcCount32 *tc = &TCp->COUNT32;
    irqstatus_t flag = irq_save();
    tc->CTRLA.reg = 0;
    tc->CTRLA.reg = TC_CTRLA_MODE_COUNT32;
    armcm_enable_irq(TCp_Handler, TCp_IRQn, 2);
    tc->INTENSET.reg = TC_INTENSET_MC0;
    tc->COUNT.reg = 0;
    timer_kick();
    tc->CTRLA.reg = TC_CTRLA_MODE_COUNT32 | TC_CTRLA_ENABLE;
    irq_restore(flag);
}
DECL_INIT(timer_init);
