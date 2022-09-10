// Code to setup clocks on stm32h7
//
// Copyright (C) 2020 Konstantin Vogel <konstantin.vogel@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_REF_FREQ
#include "board/armcm_boot.h" // VectorTable
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // get_pclock_frequency
#include "sched.h" // sched_main


/****************************************************************
 * Clock setup
 ****************************************************************/

#define FREQ_PERIPH 64000000

// Map a peripheral address to its enable bits
struct cline
lookup_clock_line(uint32_t periph_base)
{
    if (periph_base >= MCU_AHB4_PERIPH_BASE) {
        uint32_t bit = 1 << ((periph_base - MCU_AHB4_PERIPH_BASE - 0x2000) /
                              0x1000);
        return (struct cline){.en=&RCC->MC_AHB4ENSETR, .rst=NULL, .bit=bit};
    }
    if (periph_base == USART2_BASE)
        return (struct cline){.en=&RCC->MC_APB1ENSETR,.rst=NULL,.bit=1<<14};
    if (periph_base == USART3_BASE)
        return (struct cline){.en=&RCC->MC_APB1ENSETR,.rst=NULL,.bit=1<<15};
    if (periph_base == UART7_BASE)
        return (struct cline){.en=&RCC->MC_APB1ENSETR,.rst=NULL,.bit=1<<18};
    if (periph_base == TIM15_BASE)
        return (struct cline){.en=&RCC->MC_APB2ENSETR,.rst=NULL,.bit=1<<2};
    if (periph_base == TIM16_BASE)
        return (struct cline){.en=&RCC->MC_APB2ENSETR,.rst=NULL,.bit=1<<3};
    if (periph_base == TIM17_BASE)
        return (struct cline){.en=&RCC->MC_APB2ENSETR,.rst=NULL,.bit=1<<4};

    return (struct cline){.en=NULL, .rst=NULL, .bit=0};
}

// Return the frequency of the given peripheral clock
uint32_t
get_pclock_frequency(uint32_t periph_base)
{
    return FREQ_PERIPH;
}

// Enable a GPIO peripheral clock
void
gpio_clock_enable(GPIO_TypeDef *regs)
{
    uint32_t pos = ((uint32_t)regs - MCU_AHB4_PERIPH_BASE - 0x2000) / 0x1000;
    RCC->MC_AHB4ENSETR |= (1 << pos);
    RCC->MC_AHB4ENSETR;
}

// Main clock and power setup called at chip startup
static void
clock_setup(void)
{
    RCC->ADCCKSELR = 1; //per_ck selected for ADC
    RCC->UART24CKSELR = 2; //hsi_ker_ck selected for USART
    RCC->UART35CKSELR = 2; //hsi_ker_ck selected for USART
    RCC->UART78CKSELR = 2; //hsi_ker_ck selected for USART
    RCC->MC_APB1ENSETR |= RCC_MC_APB1ENSETR_WWDG1EN;
}

/****************************************************************
 * USB bootloader
 ****************************************************************/

// Handle USB reboot requests
void
usb_request_bootloader(void)
{
}

/****************************************************************
 * Startup
 ****************************************************************/

// Main entry point - called from armcm_boot.c:ResetHandler()
void
armcm_main(void)
{
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    // set CP10 and CP11 Full Access
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    #endif

    /* Configure the Vector Table location add offset address */
    SCB->VTOR = (uint32_t)VectorTable;
    /* Disable all interrupts and events */
    EXTI_C2->IMR1 = 0;
    EXTI_C2->IMR2 = 0;
    EXTI_C2->IMR3 = 0;
    EXTI_C2->EMR1 = 0;
    EXTI_C2->EMR2 = 0;
    EXTI_C2->EMR3 = 0;

    clock_setup();

    sched_main();
}
