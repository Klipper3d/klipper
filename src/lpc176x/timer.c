// SAM3x8e timer interrupt scheduling
//
// Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "board/timer_irq.h" // timer_dispatch_many
#include "command.h" // DECL_SHUTDOWN
#include "sched.h" // DECL_INIT

#include "pins_MKS.h"

#include <LPC17xx.h>
#include <lpc17xx_timer.h>
#include <lpc17xx_clkpwr.h>
#include <cmsis_nvic.h>

// Note: Timer counter channel 0 is used

void timer_isr(void);

// Set the next irq time
static void
timer_set(uint32_t const value) {
    LPC_TIM0->MR0 = value;
}

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void) {
    return LPC_TIM0->TC; // Return current timer counter value
}

// Activate timer dispatch as soon as possible
void
timer_kick(void) {
    timer_set(LPC_TIM0->TC + 50);
    LPC_TIM0->IR = 0xFFFFFFFF; // Reset interrupt
    NVIC_ClearPendingIRQ(TIMER0_IRQn);
}

void
timer_init(void)
{
    NVIC_DisableIRQ(TIMER0_IRQn);

    /* Enable clocks just in case */
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM0, ENABLE);
    CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_TIMER0, CLKPWR_PCLKSEL_CCLK_DIV_1); // SystemClock / 1

    LPC_TIM0->TCR  = (1 << 1); // Set timer to reset

    // Init Count and Capture Control Register
    LPC_TIM0->CTCR = 0;
    LPC_TIM0->CCR  = 0; // = timer mode

    /********************************
      PR = (2 - 1) , actual value is X-1
      FCPU / ( CCLK_DIV_1 (1) * PR ) -->
      LPC1768 -> 100MHz / 2 = 50MHz
      LPC1769 -> 120MHz / 2 = 60MHz
    *********************************/
    LPC_TIM0->PR   = 1;
    LPC_TIM0->TC   = 0;
    LPC_TIM0->PC   = 0;
    LPC_TIM0->MCR  = 0b001;              // ISR on match, no reset
    LPC_TIM0->EMR  = 0;                  // No external actions
    LPC_TIM0->IR = 0xFFFFFFFF;           // Reset interrupt
    NVIC_SetPriority(TIMER0_IRQn, 0);    // Highest priority
    NVIC_ClearPendingIRQ(TIMER0_IRQn);   // Clear existings
    //NVIC_SetVector(TIMER0_IRQn, (uint32_t)&timer_isr);
    LPC_TIM0->MR0 = LPC_TIM0->TC + 1000; // Setup initial ISR to start system
    NVIC_EnableIRQ(TIMER0_IRQn);         // Enable IRQ
    LPC_TIM0->TCR = 0x1;                 // Release reset and enable timer
}
DECL_INIT(timer_init);

// IRQ handler
void TIMER0_IRQHandler(void)
//void timer_isr(void)
{
    irq_disable();

    uint32_t const next = timer_dispatch_many();
    timer_set(next);

    LPC_TIM0->IR = 0xFFFFFFFF; // Reset interrupt
    NVIC_ClearPendingIRQ(TIMER0_IRQn);

    irq_enable();
}
