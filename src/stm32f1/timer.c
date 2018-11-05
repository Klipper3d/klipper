// STM32F1 timer interrupt scheduling code.
//
// Copyright (C) 2018 Grigori Goronzy <greg@kinoho.net>
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include "board/misc.h" // timer_from_us
#include "stm32f1xx.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h"
#include "command.h" // shutdown
#include "board/io.h" // readl
#include "board/irq.h" // irq_save
#include "generic/timer_irq.h" // timer_dispatch_many
#include "sched.h" // sched_timer_dispatch


/****************************************************************
 * Low level timer code
 ****************************************************************/

static inline uint32_t
timer_get(void)
{
    return LL_TIM_GetCounter(TIM2);
}

static inline void
timer_set(uint32_t next)
{
    LL_TIM_OC_SetCompareCH1(TIM2, next);
    LL_TIM_ClearFlag_CC1(TIM2);
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    timer_set(timer_get() + 50);
}


/****************************************************************
 * 16bit hardware timer to 32bit conversion
 ****************************************************************/

// High bits of timer (top 17 bits)
static uint32_t timer_high;

// Return the current time (in absolute clock ticks).
__always_inline uint32_t
timer_read_time(void)
{
    uint32_t th = readl(&timer_high);
    uint32_t cur = timer_get();
    // Combine timer_high (high 17 bits) and current time (low 16
    // bits) using method that handles rollovers correctly.
    return (th ^ cur) + (th & 0x8000);
}

// Update timer_high every 0x8000 clock ticks
static uint_fast8_t
timer_event(struct timer *t)
{
    timer_high += 0x8000;
    t->waketime = timer_high + 0x8000;
    return SF_RESCHEDULE;
}
static struct timer wrap_timer = {
    .func = timer_event,
    .waketime = 0x8000,
};

void
timer_reset(void)
{
    sched_add_timer(&wrap_timer);
}
DECL_SHUTDOWN(timer_reset);


/****************************************************************
 * Timer init
 ****************************************************************/

void
timer_init(void)
{
    irqstatus_t flag = irq_save();
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_TIM_SetPrescaler(TIM2, __LL_TIM_CALC_PSC(SystemCoreClock, CONFIG_CLOCK_FREQ));
    LL_TIM_SetCounter(TIM2, 0);
    LL_TIM_SetAutoReload(TIM2, 0xFFFF);
    LL_TIM_EnableIT_CC1(TIM2);
    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
    NVIC_SetPriority(TIM2_IRQn, 2);
    NVIC_EnableIRQ(TIM2_IRQn);
    timer_kick();
    timer_reset();
    LL_TIM_EnableCounter(TIM2);
    irq_restore(flag);
}
DECL_INIT(timer_init);


/****************************************************************
 * Main timer dispatch irq handler
 ****************************************************************/

// Hardware timer IRQ handler - dispatch software timers
void __visible __aligned(16)
TIM2_IRQHandler(void)
{
    irq_disable();
    uint32_t next = timer_dispatch_many();
    timer_set(next);
    irq_enable();
}
