// Code to setup clocks and gpio on stm32g4
//
// Copyright (C) 2019-2025  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_REF_FREQ
#include "board/armcm_boot.h" // VectorTable
#include "board/irq.h" // irq_disable
#include "board/misc.h" // bootloader_request
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // sched_main

#define FREQ_PERIPH_DIV 2
#define FREQ_PERIPH (CONFIG_CLOCK_FREQ / FREQ_PERIPH_DIV)

// Map a peripheral address to its enable bits
struct cline
lookup_clock_line(uint32_t periph_base)
{
    if (periph_base < APB2PERIPH_BASE) {
        uint32_t pos = (periph_base - APB1PERIPH_BASE) / 0x400;
        if (pos < 32) {
#if defined(FDCAN2_BASE)
            if (periph_base == FDCAN2_BASE)
                return (struct cline){.en = &RCC->APB1ENR1,
                                      .rst = &RCC->APB1RSTR1,
                                      .bit = 1 << 25};
#endif
            return (struct cline){.en = &RCC->APB1ENR1,
                                  .rst = &RCC->APB1RSTR1,
                                  .bit = 1 << pos};
        } else {
            return (struct cline){.en = &RCC->APB1ENR2,
                                  .rst = &RCC->APB1RSTR2,
                                  .bit = 1 << (pos - 32)};
        }
    } else if (periph_base < AHB1PERIPH_BASE) {
        uint32_t pos = (periph_base - APB2PERIPH_BASE) / 0x400;
        return (struct cline){.en = &RCC->APB2ENR,
                              .rst = &RCC->APB2RSTR,
                              .bit = 1 << pos};

    } else if (periph_base < AHB2PERIPH_BASE) {
        uint32_t pos = (periph_base - AHB1PERIPH_BASE) / 0x400;
        return (struct cline){.en = &RCC->AHB1ENR,
                              .rst = &RCC->AHB1RSTR,
                              .bit = 1 << pos};

    } else {
        if (periph_base == ADC12_COMMON_BASE)
            return (struct cline){.en = &RCC->AHB2ENR,
                                  .rst = &RCC->AHB2RSTR,
                                  .bit = RCC_AHB2ENR_ADC12EN};
        uint32_t pos = (periph_base - AHB2PERIPH_BASE) / 0x400;
        return (struct cline){.en = &RCC->AHB2ENR,
                              .rst = &RCC->AHB2RSTR,
                              .bit = 1 << pos};
    }
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
    uint32_t rcc_pos = ((uint32_t)regs - GPIOA_BASE) / 0x400;
    RCC->AHB2ENR |= 1 << rcc_pos;
    RCC->AHB2ENR;
}

// PLL (g4) input: 2.66 to 16Mhz, vco: 96 to 344Mhz, output: 2.06 to 170Mhz

#if !CONFIG_STM32_CLOCK_REF_INTERNAL
DECL_CONSTANT_STR("RESERVE_PINS_crystal", "PF0,PF1");
#endif

static void
enable_clock_stm32g4(void)
{
    uint32_t pll_base = 4000000, pll_freq = CONFIG_CLOCK_FREQ * 2, pllcfgr;
    if (!CONFIG_STM32_CLOCK_REF_INTERNAL) {
        // Configure PLL from external crystal (HSE)
        uint32_t div = CONFIG_CLOCK_REF_FREQ / pll_base - 1;
        RCC->CR |= RCC_CR_HSEON;
        while (!(RCC->CR & RCC_CR_HSERDY))
            ;
        pllcfgr = RCC_PLLCFGR_PLLSRC_HSE | (div << RCC_PLLCFGR_PLLM_Pos);
    } else {
        // Configure PLL from internal 16Mhz oscillator (HSI)
        uint32_t div = 16000000 / pll_base - 1;
        pllcfgr = RCC_PLLCFGR_PLLSRC_HSI | (div << RCC_PLLCFGR_PLLM_Pos);
        RCC->CR |= RCC_CR_HSION;
        while (!(RCC->CR & RCC_CR_HSIRDY))
            ;
    }
    RCC->PLLCFGR = (pllcfgr | ((pll_freq/pll_base) << RCC_PLLCFGR_PLLN_Pos)
                    | (0 << RCC_PLLCFGR_PLLR_Pos));
    RCC->CR |= RCC_CR_PLLON;

    // Enable 48Mhz USB clock using clock recovery
    if (CONFIG_USB) {
        RCC->CRRCR |= RCC_CRRCR_HSI48ON;
        while (!(RCC->CRRCR & RCC_CRRCR_HSI48RDY))
            ;
        enable_pclock(CRS_BASE);
        CRS->CR |= CRS_CR_AUTOTRIMEN | CRS_CR_CEN;
    }

    // Use PCLK for FDCAN
    RCC->CCIPR = 2 << RCC_CCIPR_FDCANSEL_Pos;
}

// Main clock setup called at chip startup
static void
clock_setup(void)
{
    enable_clock_stm32g4();

    // Set flash latency
    uint32_t latency = ((CONFIG_CLOCK_FREQ>150000000) ? FLASH_ACR_LATENCY_5WS :
                       ((CONFIG_CLOCK_FREQ>120000000) ? FLASH_ACR_LATENCY_4WS :
                       ((CONFIG_CLOCK_FREQ>90000000) ? FLASH_ACR_LATENCY_3WS :
                       ((CONFIG_CLOCK_FREQ>60000000) ? FLASH_ACR_LATENCY_2WS :
                       ((CONFIG_CLOCK_FREQ>30000000) ? FLASH_ACR_LATENCY_1WS :
                                                    FLASH_ACR_LATENCY_0WS)))));
    FLASH->ACR = (latency | FLASH_ACR_ICEN | FLASH_ACR_DCEN
                  | FLASH_ACR_PRFTEN | FLASH_ACR_DBG_SWEN);

    enable_pclock(PWR_BASE);
    PWR->CR3 |= PWR_CR3_APC; // allow gpio pullup/down
    if (CONFIG_CLOCK_FREQ > 150000000)
        // Enable "range 1 boost" mode
        PWR->CR5 = 0;

    // Wait for PLL lock
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;

    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

    // Switch system clock to PLL
    RCC->CFGR = RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV2
                | RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL)
        ;
}


/****************************************************************
 * Bootloader
 ****************************************************************/

// Handle USB reboot requests
void
bootloader_request(void)
{
    dfu_reboot();
}


/****************************************************************
 * Startup
 ****************************************************************/

// Main entry point - called from armcm_boot.c:ResetHandler()
void
armcm_main(void)
{
    dfu_reboot_check();

    // Run SystemInit() and then restore VTOR
    SystemInit();
    SCB->VTOR = (uint32_t)VectorTable;

    clock_setup();

    sched_main();
}
