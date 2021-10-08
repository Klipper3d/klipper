// STM32H7 timer support
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/armcm_timer.h" // udelay
#include "board/internal.h" // TIM3
#include "board/io.h" // readl
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "sched.h" // DECL_INIT
#include "command.h" // DECL_SHUTDOWN
#include "board/timer_irq.h" // timer_dispatch_many


/****************************************************************
 * Low level timer code
 ****************************************************************/

#ifdef TIM2
#define TIMx TIM2
#define TIMx_IRQn TIM2_IRQn
#elif defined TIM5
#define TIMx TIM5
#define TIMx_IRQn TIM5_IRQn
#else
#error No Timers to enable!
#endif

static inline uint32_t
timer_get(void)
{
    return TIMx->CNT;
}

static inline void
timer_set(uint32_t next)
{
    TIMx->CCR1 = next;
    TIMx->SR = 0;
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    timer_set(timer_get() + 50);
}


// Return the current time (in absolute clock ticks).
uint32_t __always_inline
timer_read_time(void)
{
    return timer_get();
}

void
timer_reset(void)
{
}
DECL_SHUTDOWN(timer_reset);


/****************************************************************
 * Setup and irqs
 ****************************************************************/

// Hardware timer IRQ handler - dispatch software timers
void __aligned(16)
TIMx_IRQHandler(void)
{
    irq_disable();
    uint32_t next = timer_dispatch_many();
    timer_set(next);
    irq_enable();
}

void
timer_init(void)
{
    irqstatus_t flag = irq_save();
    enable_pclock((uint32_t)TIMx);
    TIMx->CNT = 0;
    TIMx->DIER = TIM_DIER_CC1IE;
    TIMx->CCER = TIM_CCER_CC1E;
    armcm_enable_irq(TIMx_IRQHandler, TIMx_IRQn, 2);
    timer_kick();
    timer_reset();
    TIMx->CR1 = TIM_CR1_CEN;
    irq_restore(flag);
}
DECL_INIT(timer_init);
