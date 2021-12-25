// Code to setup clocks on stm32g0
//
// Copyright (C) 2019-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_REF_FREQ
#include "board/armcm_boot.h" // armcm_main
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // sched_main


/****************************************************************
 * Clock setup
 ****************************************************************/

#define FREQ_PERIPH 64000000
#define FREQ_USB 48000000

// Map an APB peripheral address to an enable bit
static int
lookup_apb_bit(uint32_t periph_base)
{
    if (periph_base == USB_BASE)
        return 13;
    if (periph_base == CRS_BASE)
        return 16;
    if (periph_base == SPI1_BASE)
        return 32 + 12;
    if (periph_base == USART1_BASE)
        return 32 + 14;
    if (periph_base == ADC1_BASE)
        return 32 + 20;
    return (periph_base - APBPERIPH_BASE) / 0x400;
}

// Enable a peripheral clock
void
enable_pclock(uint32_t periph_base)
{
    if (periph_base >= IOPORT_BASE) {
        uint32_t pos = (periph_base - IOPORT_BASE) / 0x400;
        RCC->IOPENR |= 1 << pos;
        RCC->IOPENR;
        RCC->IOPRSTR |= (1<<pos);
        RCC->IOPRSTR &= ~(1<<pos);
    } else if (periph_base >= AHBPERIPH_BASE) {
        uint32_t pos = (periph_base - AHBPERIPH_BASE) / 0x400;
        RCC->AHBENR |= 1 << pos;
        RCC->AHBENR;
        RCC->AHBRSTR |= (1<<pos);
        RCC->AHBRSTR &= ~(1<<pos);
    } else {
        uint32_t pos = lookup_apb_bit(periph_base);
        if (pos < 32) {
            RCC->APBENR1 |= 1 << pos;
            RCC->APBENR1;
            RCC->APBRSTR1 |= (1 << pos);
            RCC->APBRSTR1 &= ~(1 << pos);
        } else {
            RCC->APBENR2 |= 1 << (pos - 32);
            RCC->APBENR2;
            RCC->APBRSTR2 |= (1 << (pos - 32));
            RCC->APBRSTR2 &= ~(1 << (pos - 32));
        }
    }
}

// Check if a peripheral clock has been enabled
int
is_enabled_pclock(uint32_t periph_base)
{
    if (periph_base >= IOPORT_BASE) {
        uint32_t pos = (periph_base - IOPORT_BASE) / 0x400;
        return RCC->IOPENR & (1 << pos);
    } else if (periph_base >= AHBPERIPH_BASE) {
        uint32_t pos = (periph_base - AHBPERIPH_BASE) / 0x400;
        return RCC->AHBENR & (1 << pos);
    } else {
        uint32_t pos = lookup_apb_bit(periph_base);
        if (pos < 32)
            return RCC->APBENR1 & (1 << pos);
        return RCC->APBENR2 & (1 << (pos - 32));
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
    RCC->PLLCFGR = pllcfgr | RCC_PLLCFGR_PLLREN;
    RCC->CR |= RCC_CR_PLLON;

    // Wait for PLL lock
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;

    // Switch system clock to PLL
    RCC->CFGR = (2 << RCC_CFGR_SW_Pos);
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != (2 << RCC_CFGR_SWS_Pos))
        ;

    // Enable USB clock
    if (CONFIG_USBSERIAL) {
        RCC->CR |= RCC_CR_HSI48ON;
        while (!(RCC->CR & RCC_CR_HSI48RDY))
            ;
        enable_pclock(CRS_BASE);
        CRS->CR |= CRS_CR_AUTOTRIMEN | CRS_CR_CEN;
    }
}


/****************************************************************
 * USB bootloader
 ****************************************************************/

#define USB_BOOT_FLAG_ADDR (CONFIG_RAM_START + CONFIG_RAM_SIZE - 1024)
#define USB_BOOT_FLAG 0x55534220424f4f54 // "USB BOOT"

// Flag that bootloader is desired and reboot
static void
usb_reboot_for_dfu_bootloader(void)
{
    irq_disable();
    *(uint64_t*)USB_BOOT_FLAG_ADDR = USB_BOOT_FLAG;
    NVIC_SystemReset();
}

// Check if rebooting into system DFU Bootloader
static void
check_usb_dfu_bootloader(void)
{
    if (!CONFIG_USBSERIAL || *(uint64_t*)USB_BOOT_FLAG_ADDR != USB_BOOT_FLAG)
        return;
    *(uint64_t*)USB_BOOT_FLAG_ADDR = 0;
    uint32_t *sysbase = (uint32_t*)0x1fff0000;
    asm volatile("mov sp, %0\n bx %1"
                 : : "r"(sysbase[0]), "r"(sysbase[1]));
}

// Handle USB reboot requests
void
usb_request_bootloader(void)
{
    usb_reboot_for_dfu_bootloader();
}


/****************************************************************
 * Startup
 ****************************************************************/

// Main entry point - called from armcm_boot.c:ResetHandler()
void
armcm_main(void)
{
    check_usb_dfu_bootloader();
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

    // Set flash latency
    FLASH->ACR = (2<<FLASH_ACR_LATENCY_Pos) | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN;

    // Configure main clock
    clock_setup();

    sched_main();
}
