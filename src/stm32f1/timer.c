// STM32F1 timer interrupt scheduling code.
//
// Copyright (C) 2018 Grigori Goronzy <greg@kinoho.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include "board/misc.h" // timer_from_us
#include "stm32f1xx.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h"
#include "command.h" // shutdown
#include "board/irq.h" // irq_save
#include "sched.h" // sched_timer_dispatch


/****************************************************************
 * Low level timer code
 ****************************************************************/

DECL_CONSTANT(CLOCK_FREQ, CONFIG_CLOCK_FREQ);

union u32_u {
    struct { uint8_t b0, b1, b2, b3; };
    struct { uint16_t lo, hi; };
    uint32_t val;
};

static inline uint32_t
timer_get(void)
{
    return LL_TIM_GetCounter(TIM2);
}

static inline void
timer_set(uint32_t next)
{
    LL_TIM_OC_SetCompareCH1(TIM2, next);
}

static inline void
timer_repeat_set(uint32_t next)
{
    LL_TIM_OC_SetCompareCH2(TIM2, next);
    LL_TIM_ClearFlag_CC2(TIM2);
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    timer_set(timer_get() + 50);
    LL_TIM_ClearFlag_CC1(TIM2);
}

static struct timer wrap_timer;

void
timer_reset(void)
{
    sched_add_timer(&wrap_timer);
}
DECL_SHUTDOWN(timer_reset);

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
    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 0);
    timer_kick();
    timer_repeat_set(timer_get() + 50);
    timer_reset();
    LL_TIM_EnableCounter(TIM2);
    irq_restore(flag);
}
DECL_INIT(timer_init);


/****************************************************************
 * 32bit timer wrappers
 ****************************************************************/

static uint32_t timer_high;

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void)
{
    irqstatus_t flag = irq_save();
    union u32_u calc = { .val = timer_get() };
    calc.hi = timer_high;
    if (unlikely(LL_TIM_IsActiveFlag_UPDATE(TIM2))) {
        irq_restore(flag);
        if (calc.b1 < 0xff)
            calc.hi++;
        return calc.val;
    }
    irq_restore(flag);
    return calc.val;
}

// Timer that runs every cycle - allows 16bit comparison optimizations
static uint_fast8_t
timer_event(struct timer *t)
{
    union u32_u *nextwake = (void*)&wrap_timer.waketime;
    if (LL_TIM_IsActiveFlag_UPDATE(TIM2)) {
        // Hardware timer has overflowed - update overflow counter
        LL_TIM_ClearFlag_UPDATE(TIM2);
        timer_high++;
        *nextwake = (union u32_u){ .hi = timer_high, .lo = 0x8000 };
    } else {
        *nextwake = (union u32_u){ .hi = timer_high + 1, .lo = 0x0000 };
    }
    return SF_RESCHEDULE;
}
static struct timer wrap_timer = {
    .func = timer_event,
    .waketime = 0x8000,
};

#define TIMER_IDLE_REPEAT_TICKS timer_from_us(500)
#define TIMER_REPEAT_TICKS timer_from_us(100)

#define TIMER_MIN_TRY_TICKS timer_from_us(1)
#define TIMER_DEFER_REPEAT_TICKS timer_from_us(5)

// Hardware timer IRQ handler - dispatch software timers
void __visible __aligned(16)
TIM2_IRQHandler(void)
{
    irq_disable();
    LL_TIM_ClearFlag_CC1(TIM2);
    uint32_t next;
    for (;;) {
        // Run the next software timer
        next = sched_timer_dispatch();

        for (;;) {
            int16_t diff = timer_get() - next;
            if (likely(diff >= 0)) {
                // Another timer is pending - briefly allow irqs and then run it
                irq_enable();
                if (unlikely(LL_TIM_IsActiveFlag_CC2(TIM2)))
                    goto check_defer;
                irq_disable();
                break;
            }

            if (likely(diff <= -TIMER_MIN_TRY_TICKS))
                // Schedule next timer normally
                goto done;

            irq_enable();
            if (unlikely(LL_TIM_IsActiveFlag_CC2(TIM2)))
                goto check_defer;
            irq_disable();
            continue;

        check_defer:
            // Check if there are too many repeat timers
            irq_disable();
            uint32_t now = timer_get();
            if ((int16_t)(next - now) < (int16_t)(-timer_from_us(1000)))
                try_shutdown("Rescheduled timer in the past");
            if (sched_tasks_busy()) {
                timer_repeat_set(now + TIMER_REPEAT_TICKS);
                next = now + TIMER_DEFER_REPEAT_TICKS;
                goto done;
            }
            timer_repeat_set(now + TIMER_IDLE_REPEAT_TICKS);
            timer_set(now);
        }
    }

done:
    timer_set(next);
    irq_enable();
}
