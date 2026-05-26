// Code to setup clocks on stm32f7
//
// Copyright (C) 2023  Frederic Morin <frederic.morin.8@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_REF_FREQ
#include "board/armcm_boot.h" // VectorTable
#include "board/armcm_reset.h" // try_request_canboot
#include "board/irq.h" // irq_disable
#include "board/misc.h" // bootloader_request
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // sched_main


/****************************************************************
 * Clock setup
 ****************************************************************/

#define FREQ_PERIPH_DIV 4
#define FREQ_PERIPH (CONFIG_CLOCK_FREQ / FREQ_PERIPH_DIV)
#define FREQ_USB 48000000

// Map a peripheral address to its enable bits
struct cline
lookup_clock_line(uint32_t periph_base)
{
    if (periph_base >= AHB1PERIPH_BASE) {
        uint32_t bit = 1 << ((periph_base - AHB1PERIPH_BASE) / 0x400);
        return (struct cline){.en=&RCC->AHB1ENR, .rst=&RCC->AHB1RSTR, .bit=bit};
    } else if (periph_base >= APB2PERIPH_BASE) {
        uint32_t bit = 1 << ((periph_base - APB2PERIPH_BASE) / 0x400);
        if (bit & 0x700)
            // Skip ADC peripheral reset as they share a bit
            return (struct cline){.en=&RCC->APB2ENR, .bit=bit};
        return (struct cline){.en=&RCC->APB2ENR, .rst=&RCC->APB2RSTR, .bit=bit};
    } else {
        uint32_t bit = 1 << ((periph_base - APB1PERIPH_BASE) / 0x400);
        return (struct cline){.en=&RCC->APB1ENR, .rst=&RCC->APB1RSTR, .bit=bit};
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
    uint32_t rcc_pos = ((uint32_t)regs - AHB1PERIPH_BASE) / 0x400;
    RCC->AHB1ENR |= 1 << rcc_pos;
    RCC->AHB1ENR;
}

// PLL (f765) input: 0.95 to 2.1Mhz, vco: 100 to 432Mhz, output: 24 to 216Mhz

#if !CONFIG_STM32_CLOCK_REF_INTERNAL
DECL_CONSTANT_STR("RESERVE_PINS_crystal", "PH0,PH1");
#endif

// Main clock setup called at chip startup
static void
clock_setup(void)
{
    // Configure and enable PLL
    const uint32_t pll_base = 2000000, pll_freq = CONFIG_CLOCK_FREQ * 2;
    uint32_t pllcfgr;
    if (!CONFIG_STM32_CLOCK_REF_INTERNAL) {
        // Configure 216Mhz PLL from external crystal (HSE)
        const uint32_t div = CONFIG_CLOCK_REF_FREQ / pll_base;
        RCC->CR |= RCC_CR_HSEON;
        pllcfgr = RCC_PLLCFGR_PLLSRC_HSE | (div << RCC_PLLCFGR_PLLM_Pos);
    } else {
        // Configure 216Mhz PLL from internal 16Mhz oscillator (HSI)
        const uint32_t div = 16000000 / pll_base;
        pllcfgr = RCC_PLLCFGR_PLLSRC_HSI | (div << RCC_PLLCFGR_PLLM_Pos);
    }
    RCC->PLLCFGR = (pllcfgr | ((pll_freq/pll_base) << RCC_PLLCFGR_PLLN_Pos)
                    | (0 << RCC_PLLCFGR_PLLP_Pos)  //  /2
                    | ((pll_freq/FREQ_USB) << RCC_PLLCFGR_PLLQ_Pos)
                    | (2 << RCC_PLLCFGR_PLLR_Pos));
    RCC->CR |= RCC_CR_PLLON;

    // Enable "over drive"
    enable_pclock(PWR_BASE);
    PWR->CR1 = (3 << PWR_CR1_VOS_Pos) | PWR_CR1_ODEN;
    while (!(PWR->CSR1 & PWR_CSR1_ODRDY))
        ;
    PWR->CR1 = (3 << PWR_CR1_VOS_Pos) | PWR_CR1_ODEN | PWR_CR1_ODSWEN;
    while (!(PWR->CSR1 & PWR_CSR1_ODSWRDY))
        ;

    // Enable 48Mhz USB clock
    if (CONFIG_USB) {
        // setup PLLSAI
        const uint32_t plls_base = 2000000, plls_freq = FREQ_USB * 4;
        RCC->PLLSAICFGR = (
            ((plls_freq/plls_base) << RCC_PLLSAICFGR_PLLSAIN_Pos)  // *96
            | (((plls_freq/FREQ_USB)/2 - 1) << RCC_PLLSAICFGR_PLLSAIP_Pos)// /4
            | ((plls_freq/FREQ_USB) << RCC_PLLSAICFGR_PLLSAIQ_Pos));
        // enable PLLSAI and wait for PLLSAI lock
        RCC->CR |= RCC_CR_PLLSAION;
        while (!(RCC->CR & RCC_CR_PLLSAIRDY))
            ;
        // set CLK48 source to PLLSAI
        RCC->DCKCFGR2 = RCC_DCKCFGR2_CK48MSEL;  // RCC_CLK48SOURCE_PLLSAIP
    }

    // Set flash latency
    MODIFY_REG(
        FLASH->ACR, FLASH_ACR_LATENCY, (uint32_t)(FLASH_ACR_LATENCY_7WS));

    // Wait for PLL lock
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;

    // Switch system clock to PLL
    RCC->CFGR = RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV4 | RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL)
        ;
}


/****************************************************************
 * Bootloader
 ****************************************************************/

// Handle reboot requests
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
    // Run SystemInit() and then restore VTOR
    SystemInit();
    SCB->VTOR = (uint32_t)VectorTable;

    // Reset peripheral clocks (for some bootloaders that don't)
    RCC->AHB1ENR = 0x00100000;
    RCC->AHB2ENR = 0x00000000;
    RCC->AHB3ENR = 0x00000000;
    RCC->APB1ENR = 0x00000400;
    RCC->APB2ENR = 0x00000000;

    dfu_reboot_check();

    // STM32F7 specific DWT unlock required prior to timer_init() DWT setup.
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->LAR = 0xC5ACCE55;

    clock_setup();

    sched_main();
}
