// Main starting point for SAM3/SAM4 boards
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_disable
#include "command.h" // DECL_CONSTANT
#include "internal.h" // WDT
#include "sched.h" // sched_main

DECL_CONSTANT(MCU, CONFIG_MCU);


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
 * misc functions
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

// Main entry point
int
main(void)
{
    SystemInit();
    sched_main();
    return 0;
}
