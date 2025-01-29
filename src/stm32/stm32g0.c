// Code to setup clocks on stm32g0
//
// Copyright (C) 2019-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_REF_FREQ
#include "board/armcm_boot.h" // armcm_main
#include "board/armcm_reset.h" // try_request_canboot
#include "board/irq.h" // irq_disable
#include "board/misc.h" // bootloader_request
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // sched_main


/****************************************************************
 * Clock setup
 ****************************************************************/

#define FREQ_PERIPH 64000000
#define FREQ_USB 48000000

// Map a peripheral address to its enable bits
struct cline
lookup_clock_line(uint32_t periph_base)
{
    if (periph_base >= IOPORT_BASE) {
        uint32_t bit = 1 << ((periph_base - IOPORT_BASE) / 0x400);
        return (struct cline){.en=&RCC->IOPENR, .rst=&RCC->IOPRSTR, .bit=bit};
    } else if (periph_base >= AHBPERIPH_BASE) {
        uint32_t bit = 1 << ((periph_base - AHBPERIPH_BASE) / 0x400);
        return (struct cline){.en=&RCC->AHBENR, .rst=&RCC->AHBRSTR, .bit=bit};
    }
#ifdef USART5_BASE
    if (periph_base == USART5_BASE)
        return (struct cline){.en=&RCC->APBENR1,.rst=&RCC->APBRSTR1,.bit=1<<8};
#endif
#ifdef USART6_BASE
    if (periph_base == USART6_BASE)
        return (struct cline){.en=&RCC->APBENR1,.rst=&RCC->APBRSTR1,.bit=1<<9};
#endif
#if defined(FDCAN1_BASE) || defined(FDCAN2_BASE)
    if ((periph_base == FDCAN1_BASE) || (periph_base == FDCAN2_BASE))
        return (struct cline){.en=&RCC->APBENR1,.rst=&RCC->APBRSTR1,.bit=1<<12};
#endif
#ifdef USB_BASE
    if (periph_base == USB_BASE)
        return (struct cline){.en=&RCC->APBENR1,.rst=&RCC->APBRSTR1,.bit=1<<13};
#endif
#ifdef CRS_BASE
    if (periph_base == CRS_BASE)
        return (struct cline){.en=&RCC->APBENR1,.rst=&RCC->APBRSTR1,.bit=1<<16};
#endif
#ifdef I2C3_BASE
    if (periph_base == I2C3_BASE)
        return (struct cline){.en=&RCC->APBENR1,.rst=&RCC->APBRSTR1,.bit=1<<23};
#endif
    if (periph_base == TIM1_BASE)
        return (struct cline){.en=&RCC->APBENR2,.rst=&RCC->APBRSTR2,.bit=1<<11};
    if (periph_base == SPI1_BASE)
        return (struct cline){.en=&RCC->APBENR2,.rst=&RCC->APBRSTR2,.bit=1<<12};
    if (periph_base == USART1_BASE)
        return (struct cline){.en=&RCC->APBENR2,.rst=&RCC->APBRSTR2,.bit=1<<14};
    if (periph_base == TIM14_BASE)
        return (struct cline){.en=&RCC->APBENR2,.rst=&RCC->APBRSTR2,.bit=1<<15};
    if (periph_base == TIM15_BASE)
        return (struct cline){.en=&RCC->APBENR2,.rst=&RCC->APBRSTR2,.bit=1<<16};
    if (periph_base == TIM16_BASE)
        return (struct cline){.en=&RCC->APBENR2,.rst=&RCC->APBRSTR2,.bit=1<<17};
    if (periph_base == TIM17_BASE)
        return (struct cline){.en=&RCC->APBENR2,.rst=&RCC->APBRSTR2,.bit=1<<18};
    if (periph_base == ADC1_BASE)
        return (struct cline){.en=&RCC->APBENR2,.rst=&RCC->APBRSTR2,.bit=1<<20};
    if (periph_base >= APBPERIPH_BASE
        && periph_base < APBPERIPH_BASE + 32*0x400) {
        uint32_t bit = 1 << ((periph_base - APBPERIPH_BASE) / 0x400);
        return (struct cline){.en=&RCC->APBENR1, .rst=&RCC->APBRSTR1, .bit=bit};
    }
    // unknown peripheral. returning .bit=0 makes this a no-op
    return (struct cline){.en=&RCC->APBENR1, .rst=NULL, .bit=0};
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
    uint32_t rcc_pos = ((uint32_t)regs - IOPORT_BASE) / 0x400;
    RCC->IOPENR |= 1 << rcc_pos;
    RCC->IOPENR;
}

#if !CONFIG_STM32_CLOCK_REF_INTERNAL
DECL_CONSTANT_STR("RESERVE_PINS_crystal", "PF0,PF1");
#endif

// Configure and enable the PLL as clock source
static void
clock_setup(void)
{
    uint32_t pll_base = 4000000, pll_freq = 192000000, pllcfgr;
    if (!CONFIG_STM32_CLOCK_REF_INTERNAL) {
        // Configure PLL from external crystal (HSE)
        uint32_t div = CONFIG_CLOCK_REF_FREQ / pll_base;
        RCC->CR |= RCC_CR_HSEON;
        pllcfgr = RCC_PLLCFGR_PLLSRC_HSE | ((div - 1) << RCC_PLLCFGR_PLLM_Pos);
    } else {
        // Configure PLL from internal 16Mhz oscillator (HSI)
        uint32_t div = 16000000 / pll_base;
        pllcfgr = RCC_PLLCFGR_PLLSRC_HSI | ((div - 1) << RCC_PLLCFGR_PLLM_Pos);
    }
    pllcfgr |= (pll_freq/pll_base) << RCC_PLLCFGR_PLLN_Pos;
    pllcfgr |= (pll_freq/CONFIG_CLOCK_FREQ - 1) << RCC_PLLCFGR_PLLR_Pos;
#ifdef RCC_PLLCFGR_PLLQ
    pllcfgr |= ((pll_freq/FREQ_USB - 1) << RCC_PLLCFGR_PLLQ_Pos)
            | RCC_PLLCFGR_PLLQEN;
#endif
    RCC->PLLCFGR = pllcfgr | RCC_PLLCFGR_PLLREN;
    RCC->CR |= RCC_CR_PLLON;

    // Wait for PLL lock
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;

    // Switch system clock to PLL
    RCC->CFGR = (2 << RCC_CFGR_SW_Pos);
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != (2 << RCC_CFGR_SWS_Pos))
        ;

#ifdef USB_BASE
    // Use PLLQCLK for USB (setting USBSEL=2 works in practice)
    RCC->CCIPR2 = 2 << RCC_CCIPR2_USBSEL_Pos;
#endif
}


/****************************************************************
 * Bootloader
 ****************************************************************/

// Handle USB reboot requests
void
bootloader_request(void)
{
    try_request_canboot();
    dfu_reboot();
}


/****************************************************************
 * Startup
 ****************************************************************/

// Main entry point - called from armcm_boot.c:ResetHandler()
void
armcm_main(void)
{
    // Disable internal pull-down resistors on UCPDx CCx pins
    SYSCFG->CFGR1 |= (SYSCFG_CFGR1_UCPD1_STROBE | SYSCFG_CFGR1_UCPD2_STROBE);
    SCB->VTOR = (uint32_t)VectorTable;

    // Reset clock registers (in case bootloader has changed them)
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY))
        ;
    RCC->CFGR = 0x00000000;
    RCC->CR = RCC_CR_HSION;
    while (RCC->CR & RCC_CR_PLLRDY)
        ;
    RCC->PLLCFGR = 0x00001000;
    RCC->IOPENR = 0x00000000;
    RCC->AHBENR = 0x00000100;
    RCC->APBENR1 = 0x00000000;
    RCC->APBENR2 = 0x00000000;

    dfu_reboot_check();

    // Set flash latency, cache and prefetch; use reset value as base
    uint32_t acr = 0x00040600;
    acr = (acr & ~FLASH_ACR_LATENCY) | (2<<FLASH_ACR_LATENCY_Pos);
    acr |= FLASH_ACR_ICEN | FLASH_ACR_PRFTEN;
    FLASH->ACR = acr;

    // Configure main clock
    clock_setup();

    sched_main();
}
