// Code to setup clocks and gpio on stm32f0
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_REF_FREQ
#include "board/armcm_boot.h" // armcm_main
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // enable_pclock
#include "sched.h" // sched_main

#define FREQ_PERIPH 64000000

typedef struct
{
    uint32_t periph_addr;
    volatile uint32_t* rcc_src;
    uint8_t rcc_bit;
}periph_map_t;

const periph_map_t apb_mess_map[] = {
    {LPUART2_BASE, &RCC->APBENR1, 7},
    {USART5_BASE, &RCC->APBENR1, 8},
    {USART6_BASE, &RCC->APBENR1, 9}, // > SYSCFG_BASE but in APB1
    {FDCAN1_BASE, &RCC->APBENR1, 12},
    {FDCAN2_BASE, &RCC->APBENR1, 12},
    {USB_BASE, &RCC->APBENR1, 13},
    {CRS_BASE, &RCC->APBENR1, 16},
    {LPUART1_BASE, &RCC->APBENR1, 20},
    {I2C3_BASE, &RCC->APBENR1, 23},
    {CEC_BASE, &RCC->APBENR1, 24},
    {UCPD1_BASE, &RCC->APBENR1, 25},
    {UCPD2_BASE, &RCC->APBENR1, 26},
    {DBG_BASE, &RCC->APBENR1, 27}, // > SYSCFG_BASE but in APB1
    {LPTIM2_BASE, &RCC->APBENR1, 30},

    {TIM14_BASE, &RCC->APBENR2, 15}, // < SYSCFG_BASE but in APB2
    {ADC1_BASE, &RCC->APBENR2, 20},
};

// Enable a peripheral clock
void
enable_pclock(uint32_t periph_base)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(apb_mess_map); i++) {
        if (periph_base == apb_mess_map[i].periph_addr) {
            *apb_mess_map[i].rcc_src |= 1 << apb_mess_map[i].rcc_bit;
            return;
        }
    }
    if (periph_base < SYSCFG_BASE) {
        uint32_t pos = (periph_base - APBPERIPH_BASE) / 0x400;
        RCC->APBENR1 |= 1 << pos;
        RCC->APBENR1;
    } else if (periph_base < AHBPERIPH_BASE) {
        uint32_t pos = (periph_base - SYSCFG_BASE) / 0x400;
        RCC->APBENR2 |= 1 << pos;
        RCC->APBENR2;
    } else if (periph_base < IOPORT_BASE){
        uint32_t pos = (periph_base - AHBPERIPH_BASE) / 0x400;
        RCC->AHBENR |= 1 << pos;
        RCC->AHBENR;
    } else{
        uint32_t pos = (periph_base - IOPORT_BASE) / 0x400;
        RCC->IOPENR |= 1 << pos;
        RCC->IOPENR;
    }
}

// Check if a peripheral clock has been enabled
int
is_enabled_pclock(uint32_t periph_base)
{
    if (periph_base < SYSCFG_BASE) {
        uint32_t pos = (periph_base - APBPERIPH_BASE) / 0x400;
        return RCC->APBENR1 & (1 << pos);
    } else if (periph_base < AHBPERIPH_BASE) {
        uint32_t pos = (periph_base - SYSCFG_BASE) / 0x400;
        return RCC->APBENR2 & (1 << pos);
    } else if (periph_base < IOPORT_BASE){
        uint32_t pos = (periph_base - AHBPERIPH_BASE) / 0x400;
        return RCC->AHBENR & 1 << pos;
    } else{
        uint32_t pos = (periph_base - IOPORT_BASE) / 0x400;
        return RCC->IOPENR & 1 << pos;
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

#define USB_BOOT_FLAG_ADDR (CONFIG_RAM_START + CONFIG_RAM_SIZE - 1024)
#define USB_BOOT_FLAG 0x55534220424f4f54 // "USB BOOT"

// Handle USB reboot requests
void
usb_request_bootloader(void)
{
    irq_disable();
    *(uint64_t*)USB_BOOT_FLAG_ADDR = USB_BOOT_FLAG;
    NVIC_SystemReset();
}

#if !CONFIG_STM32_CLOCK_REF_INTERNAL
DECL_CONSTANT_STR("RESERVE_PINS_crystal", "PF0,PF1");
#endif

// Configure and enable the PLL as clock source
static void
clock_setup(void)
{
    // Modify voltage scaling range
    PWR->CR1 &= (~PWR_CR1_VOS | PWR_CR1_VOS_0);
    // Wait until VOSF is reset
    while (PWR->SR2 & PWR_SR2_VOSF)
        ;

    // Disable PLL
    RCC->CR &= ~RCC_CR_PLLON;
    while (RCC->CR & RCC_CR_PLLRDY)
        ;
    uint32_t pll_base = 4000000, pll_freq = CONFIG_CLOCK_FREQ * 2, pllcfgr;
    if (!CONFIG_STM32_CLOCK_REF_INTERNAL) {
        // Configure 64Mhz PLL from external crystal (HSE)
        uint32_t div = CONFIG_CLOCK_REF_FREQ / pll_base;
        // Enable HSE
        RCC->CR |= RCC_CR_HSEON;
        while (!(RCC->CR & RCC_CR_HSERDY))
            ;
        pllcfgr = RCC_PLLCFGR_PLLSRC_HSE | (div << RCC_PLLCFGR_PLLM_Pos);
    } else {
        // Configure 64Mhz PLL from internal 16Mhz oscillator (HSI)
        uint32_t div = 16000000 / pll_base;
        pllcfgr = RCC_PLLCFGR_PLLSRC_HSI | (div << RCC_PLLCFGR_PLLM_Pos);
    }
    RCC->PLLCFGR |= (pllcfgr | ((pll_freq/pll_base) << RCC_PLLCFGR_PLLN_Pos))
                    | (uint32_t) (1 << RCC_PLLCFGR_PLLR_Pos) // R div = 2
                    | (uint32_t) (1 << RCC_PLLCFGR_PLLP_Pos) // P div = 2
                    | (uint32_t) (1 << RCC_PLLCFGR_PLLQ_Pos); // Q div = 2
    // Enable PLLR Clock output
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
    // Enable PLL
    RCC->CR |=RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;
    // Enable HSI48 for USB
    RCC->CR |= RCC_CR_HSI48ON;
    while (!(RCC->CR &RCC_CR_HSI48RDY))
        ;

    // Flash memory access latency two wait states
    FLASH->ACR &= ~(FLASH_ACR_LATENCY);
    FLASH->ACR |= FLASH_ACR_LATENCY_1;
    while ((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_1)
        ;

    // APB div = 1
    RCC->CFGR |= (0 << RCC_CFGR_PPRE_Pos);
    // AHB div = 1
    RCC->CFGR |= (0 << RCC_CFGR_HPRE_Pos);
    // Set PLLRCLK for SYSCLK
    RCC->CFGR |= (2 << RCC_CFGR_SW_Pos);
    while ((RCC->CFGR & RCC_CFGR_SWS) != (2 << RCC_CFGR_SWS_Pos))
        ;
}

void
armcm_main(void)
{
    // Reset clock cfg from bootloader
    RCC->PLLCFGR = 0x00001000;
    RCC->CFGR = 0x00000000;
    RCC->CR = 0x00000500;

    if (CONFIG_USBSERIAL && *(uint64_t*)USB_BOOT_FLAG_ADDR == USB_BOOT_FLAG) {
        *(uint64_t*)USB_BOOT_FLAG_ADDR = 0;
        uint32_t *sysbase = (uint32_t*)0x1fff0000;
        asm volatile("mov sp, %0\n bx %1"
                     : : "r"(sysbase[0]), "r"(sysbase[1]));
    }

    // Run SystemInit() and then restore VTOR
    SystemInit();
    SCB->VTOR = (uint32_t)VectorTable;

    clock_setup();

    sched_main();
}
