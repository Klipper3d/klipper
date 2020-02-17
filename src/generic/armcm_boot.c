// ARM Cortex-M vector table and initial bootup handling
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "armcm_boot.h" // DECL_ARMCM_IRQ
#include "autoconf.h" // CONFIG_MCU
#include "command.h" // DECL_CONSTANT_STR
#include "misc.h" // dynmem_start
#if CONFIG_ARMCM_RAM_VECTORTABLE
#include "stm32/internal.h"
#endif

// Export MCU type
DECL_CONSTANT_STR("MCU", CONFIG_MCU);

// Symbols created by armcm_link.lds.S linker script
extern uint32_t _data_start, _data_end, _data_flash;
extern uint32_t _bss_start, _bss_end, _stack_start;
#if CONFIG_ARMCM_RAM_VECTORTABLE
extern uint32_t _text_vectortable_start, _text_vectortable_end;
extern uint32_t _ram_vectortable_start;
extern uint32_t _stack_end;
#endif

/****************************************************************
 * Basic interrupt handlers
 ****************************************************************/
// Initial code entry point - invoked by the processor after a reset
void
ResetHandler(void)
{
#if CONFIG_ARMCM_RAM_VECTORTABLE
    // disable systick for malyan bootloaders
    // other interrupts may need to be disabled because of different bootloaders
    SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);

    // feed watchdog if enabled
    if(IWDG->KR) IWDG->KR = 0x0000AAAAU;

    // disable interrupts
    asm volatile("cpsid i" ::: "memory");
    asm volatile("cpsid f" ::: "memory");

    // force clear backup registers
    RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;

    // set stack pointer
    asm volatile(
        "  ldr r0, =_stack_end\n"
        "  mov sp, r0\n"
        ::: "memory");

    // copy vectortable
    uint32_t count_vt = (&_text_vectortable_end - &_text_vectortable_start) * 4;
    __builtin_memcpy(&_ram_vectortable_start, &_text_vectortable_start,
        count_vt);

    // remap 0x0 to sram vector table
    SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE_0 | SYSCFG_CFGR1_MEM_MODE_1);
    SYSCFG->CFGR1 |= (SYSCFG_CFGR1_MEM_MODE_0 | SYSCFG_CFGR1_MEM_MODE_1);

    // enable interrupts
    asm volatile("cpsie i" ::: "memory");
    asm volatile("cpsie f" ::: "memory");
#endif

    // Copy global variables from flash to ram
    uint32_t count = (&_data_end - &_data_start) * 4;
    __builtin_memcpy(&_data_start, &_data_flash, count);

    // Clear the bss segment
    __builtin_memset(&_bss_start, 0, (&_bss_end - &_bss_start) * 4);

    barrier();

    // Initializing the C library isn't needed...
    //__libc_init_array();

    // Run the main board specific code
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
