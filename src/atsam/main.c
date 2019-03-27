// Main starting point for SAM3/SAM4 boards
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_disable
#include "board/usb_cdc.h" // usb_request_bootloader
#include "command.h" // DECL_CONSTANT
#include "internal.h" // WDT
#include "sched.h" // sched_main

DECL_CONSTANT_STR("MCU", CONFIG_MCU);


/****************************************************************
 * watchdog handler
 ****************************************************************/

void
watchdog_reset(void)
{
    WDT->WDT_CR = 0xA5000001;
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    uint32_t timeout = 500 * 32768 / 128 / 1000;  // 500ms timeout
    WDT->WDT_MR = WDT_MR_WDRSTEN | WDT_MR_WDV(timeout) | WDT_MR_WDD(timeout);
}
DECL_INIT(watchdog_init);


/****************************************************************
 * Peripheral clocks
 ****************************************************************/

// Check if a peripheral clock has been enabled
int
is_enabled_pclock(uint32_t id)
{
    if (id < 32)
        return !!(PMC->PMC_PCSR0 & (1 << id));
    else
        return !!(PMC->PMC_PCSR1 & (1 << (id - 32)));
}

// Enable a peripheral clock
void
enable_pclock(uint32_t id)
{
    if (id < 32)
        PMC->PMC_PCER0 = 1 << id;
    else
        PMC->PMC_PCER1 = 1 << (id - 32);
}


/****************************************************************
 * Resets
 ****************************************************************/

void
command_reset(uint32_t *args)
{
    irq_disable();
    RSTC->RSTC_CR = ((0xA5 << RSTC_CR_KEY_Pos) | RSTC_CR_PROCRST
                     | RSTC_CR_PERRST);
    for (;;)
        ;
}
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");

#if CONFIG_MACH_SAM3X || CONFIG_MACH_SAM4S
#define EFC_HW EFC0
#elif CONFIG_MACH_SAM4E
#define EFC_HW EFC
#endif

void noinline __aligned(16) // align for predictable flash code access
usb_request_bootloader(void)
{
    irq_disable();
    // Request boot from ROM (instead of boot from flash)
    while ((EFC_HW->EEFC_FSR & EEFC_FSR_FRDY) == 0)
        ;
    EFC_HW->EEFC_FCR = (EEFC_FCR_FCMD_CGPB | EEFC_FCR_FARG(1)
                        | EEFC_FCR_FKEY_PASSWD);
    while ((EFC_HW->EEFC_FSR & EEFC_FSR_FRDY) == 0)
        ;
    // Reboot
    RSTC->RSTC_CR = ((0xA5 << RSTC_CR_KEY_Pos) | RSTC_CR_PROCRST
                     | RSTC_CR_PERRST);
    for (;;)
        ;
}


/****************************************************************
 * Startup
 ****************************************************************/

static void
matrix_init(void)
{
    // The ATSAM sram is in a "no default master" state at reset
    // (despite the specs).  That typically adds 1 wait cycle to every
    // memory access.  Set it to "last access master" to avoid that.
    MATRIX->MATRIX_SCFG[0] = (MATRIX_SCFG_SLOT_CYCLE(64)
                              | MATRIX_SCFG_DEFMSTR_TYPE(1));
}

// Main entry point
int
main(void)
{
    SystemInit();
    matrix_init();
    sched_main();
    return 0;
}
