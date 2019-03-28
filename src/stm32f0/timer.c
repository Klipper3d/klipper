/*
 * Timer and PWM functions for STM32F042 boards.
 * Also replacement for 'generic/armcm_timer.c', which not compatible
 * with Cortex-M0 CPUs
 *
 *  Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
 *  This file may be distributed under the terms of the GNU GPLv3 license.
 *
 *  DO NOT USE HAL! (Timer HAL extremely fat)
 */

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/internal.h" // SysTick
#include "board/irq.h" // irq_disable
#include "stm32f0xx.h"
#include "log.h"
#include "sched.h" // DECL_INIT
#include "command.h" // shutdown
#include "board/timer_irq.h" // timer_dispatch_many

// Set the next irq time
static void
timer_set_diff(uint32_t value)
{
    SysTick->LOAD = value & SysTick_LOAD_RELOAD_Msk;
    SysTick->VAL = 0;
    SysTick->LOAD = 0;
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;
}

/*
 * TIM2 (32bit) used instead of DWT->CYCCNT in original variant
 */
void TimerInit(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 0;
    //TIM2->ARR = 0xFFFFFFFF; reset value
    TIM2->DIER = TIM_DIER_UIE;
    TIM2->CR1 = TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM2_IRQn);

    // Enable SysTick
    irqstatus_t flag = irq_save();
    NVIC_SetPriority(SysTick_IRQn, 2);
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk
                     | SysTick_CTRL_ENABLE_Msk);
    timer_kick();
    irq_restore(flag);

}
DECL_INIT(TimerInit);

uint32_t
timer_read_time(void)
{
    return TIM2->CNT;
}

// IRQ handler
void __visible __aligned(16) // aligning helps stabilize perf benchmarks
SysTick_Handler(void)
{
    irq_disable();
    uint32_t next = timer_dispatch_many();
    timer_set_diff(next-timer_read_time());
    irq_enable();
}

void TIM2_IRQHandler(void)
{
    if(TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF; // clear UIF flag
    }
}
