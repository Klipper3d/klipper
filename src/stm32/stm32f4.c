// Code to setup clocks and gpio on stm32f4
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_REF_8M
#include "command.h" // DECL_CONSTANT_STR
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

// Check if a peripheral clock has been enabled
int
is_enabled_pclock(uint32_t periph_base)
{
    if (periph_base < APB2PERIPH_BASE) {
        uint32_t pos = (periph_base - APB1PERIPH_BASE) / 0x400;
        return RCC->APB1ENR & (1<<pos);
    } else if (periph_base < AHB1PERIPH_BASE) {
        uint32_t pos = (periph_base - APB2PERIPH_BASE) / 0x400;
        return RCC->APB2ENR & (1<<pos);
    } else if (periph_base < AHB2PERIPH_BASE) {
        uint32_t pos = (periph_base - AHB1PERIPH_BASE) / 0x400;
        return RCC->AHB1ENR & (1<<pos);
    }
    return 0;
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
    uint32_t rcc_pos = ((uint32_t)regs - AHB1PERIPH_BASE) / 0x400;
    RCC->AHB1ENR |= 1 << rcc_pos;
    RCC->AHB1ENR;
}

// Set the mode and extended function of a pin
void
gpio_peripheral(uint32_t gpio, uint32_t mode, int pullup)
{
    GPIO_TypeDef *regs = digital_regs[GPIO2PORT(gpio)];

    // Enable GPIO clock
    gpio_clock_enable(regs);

    // Configure GPIO
    uint32_t mode_bits = mode & 0xf, func = (mode >> 4) & 0xf, od = mode >> 8;
    uint32_t pup = pullup ? (pullup > 0 ? 1 : 2) : 0;
    uint32_t pos = gpio % 16, af_reg = pos / 8;
    uint32_t af_shift = (pos % 8) * 4, af_msk = 0x0f << af_shift;
    uint32_t m_shift = pos * 2, m_msk = 0x03 << m_shift;

    regs->AFR[af_reg] = (regs->AFR[af_reg] & ~af_msk) | (func << af_shift);
    regs->MODER = (regs->MODER & ~m_msk) | (mode_bits << m_shift);
    regs->PUPDR = (regs->PUPDR & ~m_msk) | (pup << m_shift);
    regs->OTYPER = (regs->OTYPER & ~(1 << pos)) | (od << pos);
    regs->OSPEEDR = (regs->OSPEEDR & ~m_msk) | (0x02 << m_shift);
}

#if CONFIG_CLOCK_REF_8M
DECL_CONSTANT_STR("RESERVE_PINS_crystal", "PH0,PH1");
#endif

// Clock configuration
static void
enable_clock_stm32f40x(void)
{
#if CONFIG_MACH_STM32F405 || CONFIG_MACH_STM32F407
    if (CONFIG_CLOCK_REF_8M) {
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
    if (CONFIG_CLOCK_REF_8M) {
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

    // Enable 48Mhz USB clock
    if (CONFIG_USBSERIAL) {
        if (CONFIG_CLOCK_REF_8M) {
            RCC->PLLSAICFGR = (
                (4 << RCC_PLLSAICFGR_PLLSAIM_Pos)
                | (96 << RCC_PLLSAICFGR_PLLSAIN_Pos)
                | (1 << RCC_PLLSAICFGR_PLLSAIP_Pos)
                | (4 << RCC_PLLSAICFGR_PLLSAIQ_Pos));
        } else {
            RCC->PLLSAICFGR = (
                (8 << RCC_PLLSAICFGR_PLLSAIM_Pos)
                | (96 << RCC_PLLSAICFGR_PLLSAIN_Pos)
                | (1 << RCC_PLLSAICFGR_PLLSAIP_Pos)
                | (4 << RCC_PLLSAICFGR_PLLSAIQ_Pos));
        }
        RCC->CR |= RCC_CR_PLLSAION;
        while (!(RCC->CR & RCC_CR_PLLSAIRDY))
            ;

        RCC->DCKCFGR2 = RCC_DCKCFGR2_CK48MSEL;
    }
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
}
