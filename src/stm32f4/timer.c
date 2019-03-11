// STM32F4 timer interrupt scheduling code.
//
// Copyright (C) 2018 Grigori Goronzy <greg@kinoho.net>
// Copyright (C) 2018 Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2019 Sergei Solodskikh <s.solodskih@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include "board/misc.h" // timer_from_us
#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_tim.h"
#include "command.h" // shutdown
#include "board/io.h" // readl
#include "board/irq.h" // irq_save
#include "generic/timer_irq.h" // timer_dispatch_many
#include "sched.h" // sched_timer_dispatch

/****************************************************************
 * Low level timer code
 ****************************************************************/

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
    timer_set(timer_read_time() + 50);
}

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void)
{
    return LL_TIM_GetCounter(TIM2);
}

/****************************************************************
 * Timer init
 ****************************************************************/

void
timer_init(void)
{
    irqstatus_t flag = irq_save();
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    NVIC_SetPriority(TIM2_IRQn, 2);
    NVIC_EnableIRQ(TIM2_IRQn);
    LL_TIM_SetPrescaler(TIM2, 0);
    LL_TIM_SetCounter(TIM2, 0);
    LL_TIM_SetAutoReload(TIM2, 0xFFFFFFFF);
    LL_TIM_EnableIT_CC1(TIM2);

    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);

    timer_kick();
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
