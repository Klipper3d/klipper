// GD32E23x 16-bit timer support for Klipper
//
// Copyright (C) 2026  Xiaoyue Cui <2508041672@qq.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/armcm_boot.h"
#include "board/io.h"
#include "board/irq.h"
#include "board/misc.h"
#include "board/timer_irq.h"
#include "command.h"
#include "internal.h"
#include "sched.h"

// TIMER2 is a 16-bit general-purpose timer on GD32E230.
#define SCHED_TIMER TIMER2
#define SCHED_TIMER_CLOCK RCU_TIMER2
#define SCHED_TIMER_IRQn TIMER2_IRQn

static inline uint32_t
timer_get(void)
{
    return TIMER_CNT(SCHED_TIMER) & 0xffff;
}

static inline void
timer_set(uint32_t next)
{
    TIMER_CH0CV(SCHED_TIMER) = next & 0xffff;
    // GD32 timer flags are cleared by writing zero to the selected flag bit.
    TIMER_INTF(SCHED_TIMER) = ~TIMER_INTF_CH0IF;
}

void
timer_kick(void)
{
    timer_set(timer_get() + 50);
}

// Extend the 16-bit hardware counter to the 32-bit Klipper clock domain.
// Updating at half-period keeps timer_read_time() unambiguous at rollover.
static uint32_t timer_high;

uint32_t __always_inline
timer_read_time(void)
{
    uint32_t high = readl(&timer_high);
    uint32_t low = timer_get();
    return (high ^ low) + (high & 0xffff);
}

// Early and peripheral setup delay using the same clock domain as Klipper.
void
udelay(uint32_t usecs)
{
    uint32_t end = timer_read_time() + timer_from_us(usecs);
    while (timer_is_before(timer_read_time(), end))
        ;
}

static uint_fast8_t
timer_wrap_event(struct timer *timer)
{
    timer_high += 0x8000;
    timer->waketime = timer_high + 0x8000;
    return SF_RESCHEDULE;
}

static struct timer wrap_timer = {
    .func = timer_wrap_event,
    .waketime = 0x8000,
};

void
timer_reset(void)
{
    sched_add_timer(&wrap_timer);
}
DECL_SHUTDOWN(timer_reset);

void __visible __aligned(16)
TIMER2_IRQHandler(void)
{
    irq_disable();
    uint32_t next = timer_dispatch_many();
    timer_set(next);
    irq_enable();
}

void
sched_timer_init(void)
{
    irqstatus_t flags = irq_save();
    enable_pclock(SCHED_TIMER_CLOCK);
    TIMER_CTL0(SCHED_TIMER) = 0;
    TIMER_PSC(SCHED_TIMER) = 0;       // 72 MHz timer tick = CLOCK_FREQ
    TIMER_CAR(SCHED_TIMER) = 0xffff;
    TIMER_CNT(SCHED_TIMER) = 0;
    TIMER_CH0CV(SCHED_TIMER) = 50;
    TIMER_INTF(SCHED_TIMER) = 0;
    TIMER_DMAINTEN(SCHED_TIMER) = TIMER_INT_CH0;
    armcm_enable_irq(TIMER2_IRQHandler, SCHED_TIMER_IRQn, 2);
    timer_reset();
    TIMER_CTL0(SCHED_TIMER) = TIMER_CTL0_CEN;
    irq_restore(flags);
}
DECL_INIT(sched_timer_init);
