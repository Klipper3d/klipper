// GD32E230 timer support
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
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

// Use 32bit TIM2 timer if available (otherwise use 16bit TIM3 timer)
#if defined(TIMER2)
  #define TIMx TIMER2
  #define TIMx_CLK RCU_TIMER2
  #define TIMx_IRQn TIMER2_IRQn
  #define HAVE_TIMER_32BIT	0 
#elif defined(TIMER3)
  #define TIMx TIMER3
  #define TIMx_CLK RCU_TIMER3
  #define TIMx_IRQn TIMER3_IRQn
  #define HAVE_TIMER_32BIT 0
#endif


static inline uint32_t
timer_get(void)
{
    return TIMER_CNT(TIMx);
}

static inline void
timer_set(uint32_t next)
{
	TIMER_CAR(TIMx) = (uint32_t)next;	
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
uint32_t __always_inline
timer_read_time(void)
{
    if (HAVE_TIMER_32BIT)
        return timer_get();
    uint32_t th = readl(&timer_high);
    uint32_t cur = timer_get();
    // Combine timer_high (high 17 bits) and current time (low 16
    // bits) using method that handles rollovers correctly.
    return (th ^ cur) + (th & 0xffff);
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
    if (!HAVE_TIMER_32BIT)
        sched_add_timer(&wrap_timer);
}
DECL_SHUTDOWN(timer_reset);


/****************************************************************
 * Setup and irqs
 ****************************************************************/
// Hardware timer IRQ handler - dispatch software timers
void __aligned(16)
TIMx_IRQHandler(void)
{
	TIMER_INTF(TIMx) = (~(uint32_t)TIMER_INT_FLAG_UP);	
    irq_disable();
    uint32_t next = timer_dispatch_many();
    timer_set(next);
    irq_enable();
}

void
timer_Init(void)
{
    irqstatus_t flag = irq_save();
    enable_pclock((uint32_t)TIMx_CLK);
	
	TIMER_CNT(TIMx) = (uint32_t)0;
	TIMER_CTL0(TIMx) &= ~(uint32_t)(TIMER_CTL0_DIR|TIMER_CTL0_CAM);
	TIMER_CTL0(TIMx) |= (uint32_t)TIMER_COUNTER_EDGE;
	TIMER_CTL0(TIMx) |= (uint32_t)TIMER_COUNTER_UP;
	TIMER_PSC(TIMx) = (uint32_t)1;
    
	TIMER_CTL0(TIMx) &= ~(uint32_t)TIMER_CTL0_CKDIV;
    TIMER_CTL0(TIMx) |= (uint32_t)TIMER_CKDIV_DIV1;
	
	TIMER_SWEVG(TIMx) |= (uint32_t)TIMER_SWEVG_UPG;	
	
	TIMER_DMAINTEN(TIMx) |= (uint32_t)TIMER_INT_UP;
	
	armcm_enable_irq(TIMx_IRQHandler, TIMx_IRQn, 2);
    timer_kick();
    timer_reset();
	TIMER_CTL0(TIMx) |= (uint32_t)TIMER_CTL0_CEN;
   	
	irq_restore(flag);
}
DECL_INIT(timer_Init);

