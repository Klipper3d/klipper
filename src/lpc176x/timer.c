// lpc176x timer interrupt scheduling
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "LPC17xx.h" // LPC_TIM0
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "board/timer_irq.h" // timer_dispatch_many
#include "internal.h" // enable_peripheral_clock
#include "sched.h" // DECL_INIT

// Set the next irq time
static void
timer_set(uint32_t value)
{
    LPC_TIM0->MR0 = value;
    LPC_TIM0->IR = 0x01;
}

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void)
{
    return LPC_TIM0->TC;
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
    // Disable timer
    LPC_TIM0->TCR = 0x02;
    // Setup clock and prescaler (divide sys clock by 4)
    enable_peripheral_clock(PCLK_TIMER0);
    LPC_TIM0->PR = 3;
    // Enable interrupts
    NVIC_SetPriority(TIMER0_IRQn, 2);
    NVIC_EnableIRQ(TIMER0_IRQn);
    LPC_TIM0->MCR = 0x01;
    // Clear counter value
    LPC_TIM0->TC = 0;
    timer_kick();
    // Start timer
    LPC_TIM0->TCR = 0x01;
}
DECL_INIT(timer_init);

void __visible __aligned(16) // aligning helps stabilize perf benchmarks
TIMER0_IRQHandler(void)
{
    irq_disable();
    uint32_t next = timer_dispatch_many();
    timer_set(next);
    irq_enable();
}
