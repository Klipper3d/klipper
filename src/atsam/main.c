// Main starting point for SAM3/SAM4 boards
//
// Copyright (C) 2016-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/armcm_boot.h" // armcm_main
#include "board/irq.h" // irq_disable
#include "board/misc.h" // bootloader_request
#include "command.h" // DECL_COMMAND_FLAGS
#include "internal.h" // WDT
#include "sched.h" // sched_main

#define FREQ_PERIPH_DIV (CONFIG_MACH_SAME70 ? 2 : 1)
#define FREQ_PERIPH (CONFIG_CLOCK_FREQ / FREQ_PERIPH_DIV)

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

// Return the frequency of the given peripheral clock
uint32_t
get_pclock_frequency(uint32_t id)
{
    return FREQ_PERIPH;
}


/****************************************************************
 * Resets
 ****************************************************************/

#if CONFIG_MACH_SAME70
#define RST_PARAMS ((0xA5 << RSTC_CR_KEY_Pos) | RSTC_CR_PROCRST)
#else
#define RST_PARAMS ((0xA5 << RSTC_CR_KEY_Pos) | RSTC_CR_PROCRST \
                    | RSTC_CR_PERRST)
#endif

void
command_reset(uint32_t *args)
{
    irq_disable();
    RSTC->RSTC_CR = RST_PARAMS;
    for (;;)
        ;
}
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");

#if CONFIG_MACH_SAM3X || CONFIG_MACH_SAM4S
#define EFC_HW EFC0
#elif CONFIG_MACH_SAM4E || CONFIG_MACH_SAME70
#define EFC_HW EFC
#endif

void noinline __aligned(16) // align for predictable flash code access
bootloader_request(void)
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
    RSTC->RSTC_CR = RST_PARAMS;
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

// Main entry point - called from armcm_boot.c:ResetHandler()
void
armcm_main(void)
{
    SystemInit();
    matrix_init();
    sched_main();
}
