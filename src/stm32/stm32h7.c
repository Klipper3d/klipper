// Code to setup clocks on stm32h7
//
// Copyright (C) 2020 Konstantin Vogel <konstantin.vogel@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_REF_FREQ
#include "board/armcm_boot.h" // VectorTable
#include "board/armcm_reset.h" // try_request_canboot
#include "board/irq.h" // irq_disable
#include "board/misc.h" // bootloader_request
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // get_pclock_frequency
#include "sched.h" // sched_main


/****************************************************************
 * Clock setup
 ****************************************************************/

#define FREQ_PERIPH (CONFIG_CLOCK_FREQ / 4)

// Map a peripheral address to its enable bits
struct cline
lookup_clock_line(uint32_t periph_base)
{
    if (periph_base >= D3_AHB1PERIPH_BASE) {
        uint32_t bit = 1 << ((periph_base - D3_AHB1PERIPH_BASE) / 0x400);
        return (struct cline){.en=&RCC->AHB4ENR, .rst=&RCC->AHB4RSTR, .bit=bit};
    } else if (periph_base >= D3_APB1PERIPH_BASE) {
        uint32_t bit = 1 << ((periph_base - D3_APB1PERIPH_BASE) / 0x400);
        return (struct cline){.en=&RCC->APB4ENR, .rst=&RCC->APB4RSTR, .bit=bit};
    } else if (periph_base >= D1_AHB1PERIPH_BASE) {
        uint32_t bit = 1 << ((periph_base - D1_AHB1PERIPH_BASE) / 0x400);
        return (struct cline){.en=&RCC->AHB3ENR, .rst=&RCC->AHB3RSTR, .bit=bit};
    } else if (periph_base >= D1_APB1PERIPH_BASE) {
        uint32_t bit = 1 << ((periph_base - D1_APB1PERIPH_BASE) / 0x400);
        return (struct cline){.en=&RCC->APB3ENR, .rst=&RCC->APB3RSTR, .bit=bit};
    } else if (periph_base >= D2_AHB2PERIPH_BASE) {
        uint32_t bit = 1 << ((periph_base - D2_AHB2PERIPH_BASE) / 0x400);
        return (struct cline){.en=&RCC->AHB2ENR, .rst=&RCC->AHB2RSTR, .bit=bit};
    } else if (periph_base >= D2_AHB1PERIPH_BASE) {
        if (periph_base == ADC12_COMMON_BASE)
            return (struct cline){.en = &RCC->AHB1ENR, .rst = &RCC->AHB1RSTR,
                                  .bit = RCC_AHB1ENR_ADC12EN};
        uint32_t bit = 1 << ((periph_base - D2_AHB1PERIPH_BASE) / 0x400);
        return (struct cline){.en=&RCC->AHB1ENR, .rst=&RCC->AHB1RSTR, .bit=bit};
    } else if (periph_base >= D2_APB2PERIPH_BASE) {
        uint32_t bit = 1 << ((periph_base - D2_APB2PERIPH_BASE) / 0x400);
        return (struct cline){.en=&RCC->APB2ENR, .rst=&RCC->APB2RSTR, .bit=bit};
    } else {
        if (periph_base == FDCAN2_BASE)
            // FDCAN1 and FDCAN2 share same clock enable
            return (struct cline){.en=&RCC->APB1HENR, .rst=&RCC->APB1HRSTR,
                                  .bit = RCC_APB1HENR_FDCANEN};
        uint32_t offset = ((periph_base - D2_APB1PERIPH_BASE) / 0x400);
        if (offset < 32) {
            uint32_t bit = 1 << offset;
            return (struct cline){
                .en=&RCC->APB1LENR, .rst=&RCC->APB1LRSTR, .bit=bit};
        } else {
            uint32_t bit = 1 << (offset - 32);
            return (struct cline){
                .en=&RCC->APB1HENR, .rst=&RCC->APB1HRSTR, .bit=bit};
        }
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
    uint32_t pos = ((uint32_t)regs - D3_AHB1PERIPH_BASE) / 0x400;
    RCC->AHB4ENR |= (1<<pos);
    RCC->AHB4ENR;
}

// PLL1 (h723) input: 2 to 16Mhz, vco: 192 to 836Mhz, output: 1.5 to 550Mhz
// PLL1 (h743v) input: 2 to 16Mhz, vco: 192 to 960Mhz, output: 1.5 to 480Mhz

#if !CONFIG_STM32_CLOCK_REF_INTERNAL
DECL_CONSTANT_STR("RESERVE_PINS_crystal", "PH0,PH1");
#endif

// Main clock and power setup called at chip startup
static void
clock_setup(void)
{
    // Enable low dropout regulator
    PWR->CR3 = PWR_CR3_LDOEN;
    while (!(PWR->CSR1 & PWR_CSR1_ACTVOSRDY))
        ;

    // Setup pll1 frequency
    uint32_t pll_base = CONFIG_STM32_CLOCK_REF_25M ? 5000000 : 4000000;
    uint32_t pll_freq = CONFIG_CLOCK_FREQ * (CONFIG_MACH_STM32H723 ? 1 : 2);
    uint32_t rcc_cr = RCC_CR_HSION;
    if (!CONFIG_STM32_CLOCK_REF_INTERNAL) {
        // Configure PLL from external crystal (HSE)
        RCC->CR = rcc_cr = rcc_cr | RCC_CR_HSEON;
        while (!(RCC->CR & RCC_CR_HSERDY))
            ;
        RCC->PLLCKSELR = RCC_PLLCKSELR_PLLSRC_HSE
            | ((CONFIG_CLOCK_REF_FREQ/pll_base) << RCC_PLLCKSELR_DIVM1_Pos);
    } else {
        // Configure PLL from internal 64Mhz oscillator (HSI)
        // HSI frequency of 64Mhz is integer divisible with 4Mhz
        pll_base = 4000000;
        RCC->PLLCKSELR = RCC_PLLCKSELR_PLLSRC_HSI
            | ((64000000/pll_base) << RCC_PLLCKSELR_DIVM1_Pos);
    }
    // Set input frequency range of PLL1 according to pll_base
    // 3 = 8-16Mhz, 2 = 4-8Mhz
    RCC->PLLCFGR = (2 << RCC_PLLCFGR_PLL1RGE_Pos)
        // Enable PLL1Q (used by some peripherals)
        | RCC_PLLCFGR_DIVQ1EN
        // Enable PLL1P (for cpu clock)
        | RCC_PLLCFGR_DIVP1EN;
    // Set multiplier DIVN1 and post divider DIVP1
    RCC->PLL1DIVR = ((pll_freq/pll_base - 1) << RCC_PLL1DIVR_N1_Pos)
        // Set PLL1Q frequency (some peripherals directly use pll1_q_ck)
        | ((pll_freq/FREQ_PERIPH - 1) << RCC_PLL1DIVR_Q1_Pos)
        // Set PLL1P cpu clock frequency
        | ((pll_freq/CONFIG_CLOCK_FREQ - 1) << RCC_PLL1DIVR_P1_Pos);

    // Enable VOS1 power mode (or VOS0 if freq>400Mhz)
    if (CONFIG_MACH_STM32H723) {
        PWR->D3CR = (CONFIG_CLOCK_FREQ > 400000000 ? 0 : 3) << PWR_D3CR_VOS_Pos;
        while (!(PWR->D3CR & PWR_D3CR_VOSRDY))
            ;
    } else {
        PWR->D3CR = 3 << PWR_D3CR_VOS_Pos;
        while (!(PWR->D3CR & PWR_D3CR_VOSRDY))
            ;
        if (CONFIG_CLOCK_FREQ > 400000000) {
            enable_pclock((uint32_t)SYSCFG);
#ifdef SYSCFG_PWRCR_ODEN
            SYSCFG->PWRCR |= SYSCFG_PWRCR_ODEN;
#endif
            while (!(PWR->D3CR & PWR_D3CR_VOSRDY))
                ;
        }
    }

    SCB_EnableICache();
    SCB_EnableDCache();

    // Set flash latency according to clock frequency
    uint32_t flash_acr_latency = (CONFIG_CLOCK_FREQ > 450000000) ?
        FLASH_ACR_LATENCY_4WS : FLASH_ACR_LATENCY_2WS;
    FLASH->ACR = flash_acr_latency | (2 << FLASH_ACR_WRHIGHFREQ_Pos);
    while (!(FLASH->ACR & flash_acr_latency))
        ;

    // Set HPRE clock to div 2 (CONFIG_CLOCK_FREQ/2) and set
    // peripheral clocks another div 2 (CONFIG_CLOCK_FREQ/4)
    RCC->D1CFGR = RCC_D1CFGR_HPRE_DIV2 | RCC_D1CFGR_D1PPRE_DIV2;
    RCC->D2CFGR = RCC_D2CFGR_D2PPRE1_DIV2 | RCC_D2CFGR_D2PPRE2_DIV2;
    RCC->D3CFGR = RCC_D3CFGR_D3PPRE_DIV2;

    // Switch on PLL1
    RCC->CR = rcc_cr = rcc_cr | RCC_CR_PLL1ON;
    while (!(RCC->CR & RCC_CR_PLL1RDY))
        ;

    // Switch system clock source (SYSCLK) to PLL1
    RCC->CFGR = RCC_CFGR_SW_PLL1;
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL1)
        ;

    // Set the source of FDCAN clock to pll1_q_ck
    RCC->D2CCIP1R = 1 << RCC_D2CCIP1R_FDCANSEL_Pos;

    // Configure USB to use HSI48 clock
    if (CONFIG_USB) {
        RCC->CR = rcc_cr = rcc_cr | RCC_CR_HSI48ON;
        while ((RCC->CR & RCC_CR_HSI48RDY) == 0)
            ;
        enable_pclock((uint32_t)CRS);
        CRS->CFGR &= ~CRS_CFGR_SYNCSRC;
        CRS->CR = (CRS->CR & CRS_CR_TRIM) | CRS_CR_CEN | CRS_CR_AUTOTRIMEN;
        RCC->D2CCIP2R = RCC_D2CCIP2R_USBSEL;
    }
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
    RCC->D1CCIPR = 0x00000000;
    RCC->D2CCIP1R = 0x00000000;
    RCC->D2CCIP2R = 0x00000000;
    RCC->D3CCIPR = 0x00000000;
    RCC->APB1LENR = 0x00000000;
    RCC->APB1HENR = 0x00000000;
    RCC->APB2ENR = 0x00000000;
    RCC->APB3ENR = 0x00000000;
    RCC->APB4ENR = 0x00000000;
    RCC->AHB1ENR = 0x00000000;

    SCB->VTOR = (uint32_t)VectorTable;

    dfu_reboot_check();

    clock_setup();

    sched_main();
}
