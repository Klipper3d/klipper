// ARM Cortex-M vector table and initial bootup handling
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "armcm_boot.h" // DECL_ARMCM_IRQ
#include "autoconf.h" // CONFIG_MCU
#include "board/internal.h" // SysTick
#include "command.h" // DECL_CONSTANT_STR
#include "misc.h" // dynmem_start

// Export MCU type
DECL_CONSTANT_STR("MCU", CONFIG_MCU);

// Symbols created by armcm_link.lds.S linker script
extern uint32_t _data_start, _data_end, _data_flash;
extern uint32_t _bss_start, _bss_end, _stack_start;
extern uint32_t _stack_end;

void ResetHandlerC(void);

/****************************************************************
 * Basic interrupt handlers
 ****************************************************************/

// Initial code entry point - invoked by the processor after a reset
void
ResetHandler(void)
{
    __disable_irq();

    // Explicitly load the stack pointer (for some bootloaders that don't)
    asm volatile("mov sp, %0\n bx %1"
                 : : "r"(&_stack_end), "r"(ResetHandlerC));
}

void
__noreturn
ResetHandlerC(void)
{
    // Clear all enabled user interrupts and user pending interrupts
    for (uint8_t i=0; i < sizeof(NVIC->ICER)/sizeof(NVIC->ICER[0]); i++) {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    // Reset all user interrupt priorities
    for (uint8_t i=0; i < sizeof(NVIC->IP)/sizeof(NVIC->IP[0]); i++)
        NVIC->IP[i] = 0;

    // Reset all system interrupt priorities
    for (uint8_t i=0; i < sizeof(SCB->SHP)/sizeof(SCB->SHP[0]); i++)
        SCB->SHP[i] = 0;

    // Clear pending pendsv and systick interrupts
    SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk | SCB_ICSR_PENDSTCLR_Msk;

    // Disable SysTick irq (for some bootloaders that don't)
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;

    __DSB();
    __ISB();

    // Copy global variables from flash to ram
    uint32_t count = (&_data_end - &_data_start) * 4;
    __builtin_memcpy(&_data_start, &_data_flash, count);

    // Clear the bss segment
    __builtin_memset(&_bss_start, 0, (&_bss_end - &_bss_start) * 4);

    barrier();

    // Initializing the C library isn't needed...
    //__libc_init_array();

    // Run the main board specific code
    __enable_irq();
    armcm_main();

    // The armcm_main() call should not return
    for (;;)
        ;
}
DECL_ARMCM_IRQ(ResetHandler, -15);

// Code called for any undefined interrupts
void
DefaultHandler(void)
{
    for (;;)
        ;
}


/****************************************************************
 * Dynamic memory range
 ****************************************************************/

// Return the start of memory available for dynamic allocations
void *
dynmem_start(void)
{
    return &_bss_end;
}

// Return the end of memory available for dynamic allocations
void *
dynmem_end(void)
{
    return &_stack_start;
}
