// Code to setup clocks on stm32f4
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_STM32F4_CLOCK_REF_8M
#include "internal.h" // enable_pclock

#define FREQ_PERIPH (CONFIG_CLOCK_FREQ / 4)

// Enable a peripheral clock
void
enable_pclock(uint32_t periph_base)
{
    if (periph_base < APB2PERIPH_BASE) {
        uint32_t pos = (periph_base - APB1PERIPH_BASE) / 0x400;
        RCC->APB1ENR |= (1<<pos);
        RCC->APB1ENR;
    } else if (periph_base < AHB1PERIPH_BASE) {
        uint32_t pos = (periph_base - APB2PERIPH_BASE) / 0x400;
        RCC->APB2ENR |= (1<<pos);
        RCC->APB2ENR;
    } else if (periph_base < AHB2PERIPH_BASE) {
        uint32_t pos = (periph_base - AHB1PERIPH_BASE) / 0x400;
        RCC->AHB1ENR |= (1<<pos);
        RCC->AHB1ENR;
    }
}

// Return the frequency of the given peripheral clock
uint32_t
get_pclock_frequency(uint32_t periph_base)
{
    return FREQ_PERIPH;
}

// Clock configuration
static void
enable_clock_stm32f40x(void)
{
#if CONFIG_MACH_STM32F405 || CONFIG_MACH_STM32F407
    if (CONFIG_STM32F4_CLOCK_REF_8M) {
        // Configure 168Mhz PLL from external 8Mhz crystal (HSE)
        RCC->CR |= RCC_CR_HSEON;
        RCC->PLLCFGR = (
            RCC_PLLCFGR_PLLSRC_HSE | (4 << RCC_PLLCFGR_PLLM_Pos)
            | (168 << RCC_PLLCFGR_PLLN_Pos) | (0 << RCC_PLLCFGR_PLLP_Pos)
            | (7 << RCC_PLLCFGR_PLLQ_Pos));
    } else {
        // Configure 168Mhz PLL from internal 16Mhz oscillator (HSI)
        RCC->PLLCFGR = (
            RCC_PLLCFGR_PLLSRC_HSI | (8 << RCC_PLLCFGR_PLLM_Pos)
            | (168 << RCC_PLLCFGR_PLLN_Pos) | (0 << RCC_PLLCFGR_PLLP_Pos)
            | (7 << RCC_PLLCFGR_PLLQ_Pos));
    }
    RCC->CR |= RCC_CR_PLLON;
#endif
}

static void
enable_clock_stm32f446(void)
{
#if CONFIG_MACH_STM32F446
    if (CONFIG_STM32F4_CLOCK_REF_8M) {
        // Configure 180Mhz PLL from external 8Mhz crystal (HSE)
        RCC->CR |= RCC_CR_HSEON;
        RCC->PLLCFGR = (
            RCC_PLLCFGR_PLLSRC_HSE | (4 << RCC_PLLCFGR_PLLM_Pos)
            | (180 << RCC_PLLCFGR_PLLN_Pos) | (0 << RCC_PLLCFGR_PLLP_Pos)
            | (7 << RCC_PLLCFGR_PLLQ_Pos) | (6 << RCC_PLLCFGR_PLLR_Pos));
    } else {
        // Configure 180Mhz PLL from internal 16Mhz oscillator (HSI)
        RCC->PLLCFGR = (
            RCC_PLLCFGR_PLLSRC_HSI | (8 << RCC_PLLCFGR_PLLM_Pos)
            | (180 << RCC_PLLCFGR_PLLN_Pos) | (0 << RCC_PLLCFGR_PLLP_Pos)
            | (7 << RCC_PLLCFGR_PLLQ_Pos) | (6 << RCC_PLLCFGR_PLLR_Pos));
    }
    RCC->CR |= RCC_CR_PLLON;

    // Enable "over drive"
    enable_pclock(PWR_BASE);
    PWR->CR = (3 << PWR_CR_VOS_Pos) | PWR_CR_ODEN;
    while (!(PWR->CSR & PWR_CSR_ODRDY))
        ;
    PWR->CR = (3 << PWR_CR_VOS_Pos) | PWR_CR_ODEN | PWR_CR_ODSWEN;
    while (!(PWR->CSR & PWR_CSR_ODSWRDY))
        ;
#endif
}

// Main clock setup called at chip startup
void
clock_setup(void)
{
    if (CONFIG_MACH_STM32F405 || CONFIG_MACH_STM32F407)
        enable_clock_stm32f40x();
    else
        enable_clock_stm32f446();

    // Set flash latency
    FLASH->ACR = (FLASH_ACR_LATENCY_5WS | FLASH_ACR_ICEN | FLASH_ACR_DCEN
                  | FLASH_ACR_PRFTEN);

    // Wait for PLL lock
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;

    // Switch system clock to PLL
    RCC->CFGR = RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV4 | RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL)
        ;

    // Enable GPIO clocks
    enable_pclock(GPIOA_BASE);
    enable_pclock(GPIOB_BASE);
    enable_pclock(GPIOC_BASE);
}
