// Startup code on rp2040
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_*
#include <stdint.h> // uint32_t
#include "pico/types.h" // true/false
#include "hardware/resets.h" // resets_hw
#include "hardware/clocks.h" // clock_hw_t
#include "hardware/watchdog.h" // watchdog_hw
#include "sched.h" // sched_main
#include <stdarg.h>

#include "pico/platform.h" // for init of sdk
#include "hardware/resets.h" 
#include "hardware/structs/padsbank0.h"
#include "hardware/clocks.h" 

#if CONFIG_MACH_RP2040 && CONFIG_HAVE_PIO
#include "hardware/pio.h"
#endif

/****************************************************************
 * watchdog handler
 ****************************************************************/

DECL_TASK(watchdog_update);

void
watchdog_init(void)
{
    watchdog_enable( 330 /*ms*/, true) ;
}
//DECL_INIT(watchdog_init);

/****************************************************************
 * SDK Runtime
 ****************************************************************/

void tight_loop_contents(void) { if(1==1) {} };

void panic(const char *fmt, ...) {
    __breakpoint();
    while(1);
}
void picosdk_runtime_setup(void) {
    // pre-init runs really early since we need it even for memcpy and divide!
    // (basically anything in aeabi that uses bootrom)

    // Start and end points of the constructor list,
    // defined by the linker script.
    extern void (*__preinit_array_start)(void);
    extern void (*__preinit_array_end)(void);

    // Reset all peripherals to put system into a known state,
    // - except for QSPI pads and the XIP IO bank, as this is fatal if running from flash
    // - and the PLLs, as this is fatal if clock muxing has not been reset on this boot
    // - and USB, syscfg, as this disturbs USB-to-SWD on core 1
    reset_block(~(
            RESETS_RESET_IO_QSPI_BITS |
            RESETS_RESET_PADS_QSPI_BITS |
            RESETS_RESET_PLL_USB_BITS |
            RESETS_RESET_USBCTRL_BITS |
            RESETS_RESET_SYSCFG_BITS |
            RESETS_RESET_PLL_SYS_BITS
    ));

    // Remove reset from peripherals which are clocked only by clk_sys and
    // clk_ref. Other peripherals stay in reset until we've configured clocks.
    unreset_block_wait(RESETS_RESET_BITS & ~(
            RESETS_RESET_ADC_BITS |
            RESETS_RESET_RTC_BITS |
            RESETS_RESET_SPI0_BITS |
            RESETS_RESET_SPI1_BITS |
            RESETS_RESET_UART0_BITS |
            RESETS_RESET_UART1_BITS |
            RESETS_RESET_USBCTRL_BITS
    ));

    // Call each function in the list.
    // We have to take the address of the symbols, as __preinit_array_start *is*
    // the first function pointer, not the address of it.
    for (void (**p)(void) = &__preinit_array_start; p < &__preinit_array_end; ++p) {
        (*p)();
    }

#if CONFIG_MACH_RP2040 && CONFIG_HAVE_PIO
    // Reset pio programs
    pio_clear_instruction_memory(pio0);
    pio_clear_instruction_memory(pio1);
#endif

    // After calling preinit we have enough runtime to do the exciting maths
    // in clocks_init
    clocks_init();

    // Peripheral clocks should now all be running
    unreset_block_wait(RESETS_RESET_BITS);

#if !PICO_IE_26_29_UNCHANGED_ON_RESET
    // after resetting BANK0 we should disable IE on 26-29
    hw_clear_alias(padsbank0_hw)->io[26] = hw_clear_alias(padsbank0_hw)->io[27] =
            hw_clear_alias(padsbank0_hw)->io[28] = hw_clear_alias(padsbank0_hw)->io[29] = PADS_BANK0_GPIO0_IE_BITS;
#endif

    // this is an array of either mutex_t or recursive_mutex_t (i.e. not necessarily the same size)
    // however each starts with a lock_core_t, and the spin_lock is initialized to address 1 for a recursive
    // spinlock and 0 for a regular one.

    static_assert(!(sizeof(mutex_t)&3), "");
    static_assert(!(sizeof(recursive_mutex_t)&3), "");
    static_assert(!offsetof(mutex_t, core), "");
    static_assert(!offsetof(recursive_mutex_t, core), "");

// SDK Multi-core support, memcpy, interrupt handler, and alarm
#if false    
    extern lock_core_t __mutex_array_start;
    extern lock_core_t __mutex_array_end;

    for (lock_core_t *l = &__mutex_array_start; l < &__mutex_array_end; ) {
        if (l->spin_lock) {
            assert(1 == (uintptr_t)l->spin_lock); // indicator for a recursive mutex
            recursive_mutex_t *rm = (recursive_mutex_t *)l;
            recursive_mutex_init(rm);
            l = &rm[1].core; // next
        } else {
            mutex_t *m = (mutex_t *)l;
            mutex_init(m);
            l = &m[1].core; // next
        }
    }

#if !(PICO_NO_RAM_VECTOR_TABLE || PICO_NO_FLASH)
    __builtin_memcpy(ram_vector_table, (uint32_t *) scb_hw->vtor, sizeof(ram_vector_table));
    scb_hw->vtor = (uintptr_t) ram_vector_table;
#endif

#ifndef NDEBUG
    if (__get_current_exception()) {
        // crap; started in exception handler
        __asm ("bkpt #0");
    }
#endif

#if PICO_USE_STACK_GUARDS
    // install core0 stack guard
    extern char __StackBottom;
    runtime_install_stack_guard(&__StackBottom);
#endif

    spin_locks_reset();
    irq_init_priorities();
    alarm_pool_init_default();
#endif

    // Start and end points of the constructor list,
    // defined by the linker script.
    extern void (*__init_array_start)(void);
    extern void (*__init_array_end)(void);

    // Call each function in the list.
    // We have to take the address of the symbols, as __init_array_start *is*
    // the first function pointer, not the address of it.
    for (void (**p)(void) = &__init_array_start; p < &__init_array_end; ++p) {
        (*p)();
    }
}

// Main entry point - called from armcm_boot.c:ResetHandler()
void
armcm_main(void)
{
    picosdk_runtime_setup();
    sched_main();
}
