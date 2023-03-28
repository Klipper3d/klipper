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

#if !CONFIG_STM32_CLOCK_REF_INTERNAL
DECL_CONSTANT_STR("RESERVE_PINS_crystal", "PH0,PH1");
#endif

// Main clock and power setup called at chip startup
static void
clock_setup(void)
{
#if !CONFIG_MACH_STM32H723
    // Ensure USB OTG ULPI is not enabled
    CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_USB2OTGHSULPIEN);
    CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_USB2OTGHSULPILPEN);
#endif
    // Set this despite correct defaults.
    // "The software has to program the supply configuration in PWR control
    // register 3" (pg. 259)
    // Only a single write is allowed (pg. 304)
    PWR->CR3 = (PWR->CR3 | PWR_CR3_LDOEN) & ~(PWR_CR3_BYPASS | PWR_CR3_SCUEN);
    while (!(PWR->CSR1 & PWR_CSR1_ACTVOSRDY))
        ;
    // (HSE 25mhz) /DIVM1(5) (pll_base 5Mhz) *DIVN1(192) (pll_freq 960Mhz)
    // /DIVP1(2) (SYSCLK 480Mhz)
    uint32_t pll_base = 5000000;
    // Only even dividers (DIVP1) are allowed
    uint32_t pll_freq = CONFIG_CLOCK_FREQ * 2;
    if (!CONFIG_STM32_CLOCK_REF_INTERNAL) {
        // Configure PLL from external crystal (HSE)
        RCC->CR |= RCC_CR_HSEON;
        while(!(RCC->CR & RCC_CR_HSERDY))
            ;
        MODIFY_REG(RCC->PLLCKSELR, RCC_PLLCKSELR_PLLSRC_Msk,
                                   RCC_PLLCKSELR_PLLSRC_HSE);
        MODIFY_REG(RCC->PLLCKSELR, RCC_PLLCKSELR_DIVM1_Msk,
            (CONFIG_CLOCK_REF_FREQ/pll_base) << RCC_PLLCKSELR_DIVM1_Pos);
    } else {
        // Configure PLL from internal 64Mhz oscillator (HSI)
        // HSI frequency of 64Mhz is integer divisible with 4Mhz
        pll_base = 4000000;
        MODIFY_REG(RCC->PLLCKSELR, RCC_PLLCKSELR_PLLSRC_Msk,
                                   RCC_PLLCKSELR_PLLSRC_HSI);
        MODIFY_REG(RCC->PLLCKSELR, RCC_PLLCKSELR_DIVM1_Msk,
          (64000000/pll_base) << RCC_PLLCKSELR_DIVM1_Pos);
    }
    // Set input frequency range of PLL1 according to pll_base
    // 3 = 8-16Mhz, 2 = 4-8Mhz
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL1RGE_Msk, RCC_PLLCFGR_PLL1RGE_2);
    // Disable unused PLL1 outputs
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_DIVR1EN_Msk, 0);
    // Enable PLL1Q and set to 100MHz for SPI 1,2,3
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_DIVQ1EN, RCC_PLLCFGR_DIVQ1EN);
    MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_Q1,
        (pll_freq / FREQ_PERIPH - 1) << RCC_PLL1DIVR_Q1_Pos);
    // This is necessary, default is not 1!
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_DIVP1EN_Msk, RCC_PLLCFGR_DIVP1EN);
    // Set multiplier DIVN1 and post divider DIVP1
    // 001 = /2, 010 = not allowed, 0011 = /4 ...
    MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_N1_Msk,
        (pll_freq/pll_base - 1) << RCC_PLL1DIVR_N1_Pos);
    MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_P1_Msk,
        (pll_freq/CONFIG_CLOCK_FREQ - 1) << RCC_PLL1DIVR_P1_Pos);

    // Pwr
    MODIFY_REG(PWR->D3CR, PWR_D3CR_VOS_Msk, PWR_D3CR_VOS);
    while (!(PWR->D3CR & PWR_D3CR_VOSRDY))
        ;

    // Enable VOS0 (overdrive)
    if (CONFIG_CLOCK_FREQ > 400000000) {
        RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;
#if !CONFIG_MACH_STM32H723
        SYSCFG->PWRCR |= SYSCFG_PWRCR_ODEN;
#else
        PWR->CR3 |= PWR_CR3_BYPASS;
#endif
        while (!(PWR->D3CR & PWR_D3CR_VOSRDY))
            ;
    }

    SCB_EnableICache();
    SCB_EnableDCache();

    // Set flash latency according to clock frequency (pg.159)
    uint32_t flash_acr_latency = (CONFIG_CLOCK_FREQ > 450000000) ?
        FLASH_ACR_LATENCY_4WS : FLASH_ACR_LATENCY_2WS;
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY_Msk, flash_acr_latency);
    MODIFY_REG(FLASH->ACR, FLASH_ACR_WRHIGHFREQ_Msk, FLASH_ACR_WRHIGHFREQ_1);
    while (!(FLASH->ACR & flash_acr_latency))
        ;

    // Set HPRE, D1PPRE, D2PPRE, D2PPRE2, D3PPRE dividers
    // 480MHz / 2 = 240MHz rcc_hclk3
    MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_HPRE,    RCC_D1CFGR_HPRE_3);
    // 240MHz / 2 = 120MHz rcc_pclk3
    MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_D1PPRE,  RCC_D1CFGR_D1PPRE_DIV2);
    // 240MHz / 2 = 120MHz rcc_pclk1
    MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE1, RCC_D2CFGR_D2PPRE1_DIV2);
    // 240MHz / 2 = 120MHz rcc_pclk2
    MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE2, RCC_D2CFGR_D2PPRE2_DIV2);
    // 240MHz / 2 = 120MHz rcc_pclk4
    MODIFY_REG(RCC->D3CFGR, RCC_D3CFGR_D3PPRE,  RCC_D3CFGR_D3PPRE_DIV2);

    // Switch on PLL1
    RCC->CR |= RCC_CR_PLL1ON;
    while (!(RCC->CR & RCC_CR_PLL1RDY))
        ;

    // Switch system clock source (SYSCLK) to PLL1
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL1);
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL1)
        ;

    // Set the source of FDCAN clock
    MODIFY_REG(RCC->D2CCIP1R, RCC_D2CCIP1R_FDCANSEL, RCC_D2CCIP1R_FDCANSEL_0);

    // Configure HSI48 clock for USB
    if (CONFIG_USB) {
        SET_BIT(RCC->CR, RCC_CR_HSI48ON);
        while((RCC->CR & RCC_CR_HSI48RDY) == 0);
        SET_BIT(RCC->APB1HENR, RCC_APB1HENR_CRSEN);
        SET_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_CRSRST);
        CLEAR_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_CRSRST);
        CLEAR_BIT(CRS->CFGR, CRS_CFGR_SYNCSRC);
        SET_BIT(CRS->CR, CRS_CR_CEN | CRS_CR_AUTOTRIMEN);
        CLEAR_BIT(RCC->D2CCIP2R, RCC_D2CCIP2R_USBSEL);
        SET_BIT(RCC->D2CCIP2R, RCC_D2CCIP2R_USBSEL);
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

    SCB->VTOR = (uint32_t)VectorTable;

    dfu_reboot_check();

    clock_setup();

    sched_main();
}
