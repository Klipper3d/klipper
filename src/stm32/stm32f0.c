// Code to setup clocks on stm32f0
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

#define FREQ_PERIPH 48000000

// Map a peripheral address to its enable bits
struct cline
lookup_clock_line(uint32_t periph_base)
{
    if (periph_base >= AHB2PERIPH_BASE) {
        uint32_t bit = 1 << ((periph_base - AHB2PERIPH_BASE) / 0x400 + 17);
        return (struct cline){.en=&RCC->AHBENR, .rst=&RCC->AHBRSTR, .bit=bit};
    } else if (periph_base >= SYSCFG_BASE) {
        uint32_t bit = 1 << ((periph_base - SYSCFG_BASE) / 0x400);
        return (struct cline){.en=&RCC->APB2ENR, .rst=&RCC->APB2RSTR, .bit=bit};
    } else {
        uint32_t bit = 1 << ((periph_base - APBPERIPH_BASE) / 0x400);
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
    uint32_t rcc_pos = ((uint32_t)regs - AHB2PERIPH_BASE) / 0x400;
    RCC->AHBENR |= 1 << (rcc_pos + 17);
    RCC->AHBENR;
}

// PLL (f0) input: 1 to 24Mhz, output: 16 to 48Mhz

#if !CONFIG_STM32_CLOCK_REF_INTERNAL
DECL_CONSTANT_STR("RESERVE_PINS_crystal", "PF0,PF1");
#endif

// Configure and enable the PLL as clock source
static void
pll_setup(void)
{
    uint32_t cfgr;
    if (!CONFIG_STM32_CLOCK_REF_INTERNAL) {
        // Configure 48Mhz PLL from external crystal (HSE)
        uint32_t div = CONFIG_CLOCK_FREQ / CONFIG_CLOCK_REF_FREQ;
        RCC->CR = ((RCC->CR & ~RCC_CR_HSITRIM) | RCC_CR_HSEON
                   | (CONFIG_STM32F0_TRIM << RCC_CR_HSITRIM_Pos));
        cfgr = RCC_CFGR_PLLSRC_HSE_PREDIV | ((div - 2) << RCC_CFGR_PLLMUL_Pos);
    } else {
        // Configure 48Mhz PLL from internal 8Mhz oscillator (HSI)
        uint32_t div2 = (CONFIG_CLOCK_FREQ / 8000000) * 2;
        cfgr = RCC_CFGR_PLLSRC_HSI_DIV2 | ((div2 - 2) << RCC_CFGR_PLLMUL_Pos);
    }
    RCC->CFGR = cfgr;
    RCC->CR |= RCC_CR_PLLON;

    // Wait for PLL lock
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;

    // Switch system clock to PLL
    RCC->CFGR = cfgr | RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL)
        ;

    // Setup CFGR3 register
    uint32_t cfgr3 = RCC_CFGR3_I2C1SW;
#if CONFIG_USB
        // Select PLL as source for USB clock
        cfgr3 |= RCC_CFGR3_USBSW;
#endif
    RCC->CFGR3 = cfgr3;
}

// Configure and enable internal 48Mhz clock on the stm32f042
static void
hsi48_setup(void)
{
#if CONFIG_MACH_STM32F0x2
    // Enable HSI48
    RCC->CR2 |= RCC_CR2_HSI48ON;
    while (!(RCC->CR2 & RCC_CR2_HSI48RDY))
        ;

    // Switch system clock to HSI48
    RCC->CFGR = RCC_CFGR_SW_HSI48;
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSI48)
        ;

    // Enable USB clock recovery
    if (CONFIG_USB) {
        enable_pclock(CRS_BASE);
        CRS->CR |= CRS_CR_AUTOTRIMEN | CRS_CR_CEN;
    }

    // Setup I2C1 clock
    RCC->CFGR3 = RCC_CFGR3_I2C1SW;
#endif
}

// Enable high speed internal 14Mhz clock for ADC
static void
hsi14_setup(void)
{
    // Enable HSI14 for ADC
    RCC->CR2 = RCC_CR2_HSI14ON;
    while (!(RCC->CR2 & RCC_CR2_HSI14RDY))
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

// Copy vector table and remap ram so new vector table is used
static void
enable_ram_vectortable(void)
{
    // Symbols created by armcm_link.lds.S linker script
    extern uint32_t _ram_vectortable_start, _ram_vectortable_end;
    extern uint32_t _text_vectortable_start;

    uint32_t count = (&_ram_vectortable_end - &_ram_vectortable_start) * 4;
    __builtin_memcpy(&_ram_vectortable_start, &_text_vectortable_start, count);
    barrier();

    SYSCFG->CFGR1 |= 3 << SYSCFG_CFGR1_MEM_MODE_Pos;
}

// Main entry point - called from armcm_boot.c:ResetHandler()
void
armcm_main(void)
{
    dfu_reboot_check();
    SystemInit();

    enable_pclock(SYSCFG_BASE);
    if (CONFIG_ARMCM_RAM_VECTORTABLE)
        enable_ram_vectortable();

    // Set flash latency
    FLASH->ACR = (1 << FLASH_ACR_LATENCY_Pos) | FLASH_ACR_PRFTBE;

    // Configure main clock
    if (CONFIG_MACH_STM32F0x2 && CONFIG_STM32_CLOCK_REF_INTERNAL && CONFIG_USB)
        hsi48_setup();
    else
        pll_setup();

    // Turn on hsi14 oscillator for ADC
    hsi14_setup();

    // Support pin remapping USB/CAN pins on low pinout stm32f042
    if (CONFIG_STM32_USB_PA11_PA12_REMAP || CONFIG_STM32_CANBUS_PA11_PA12_REMAP)
        SYSCFG->CFGR1 |= 1<<4; // SYSCFG_CFGR1_PA11_PA12_RMP

    sched_main();
}
