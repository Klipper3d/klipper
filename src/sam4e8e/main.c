// SAM4e8e port main entry
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// CMSIS
#include "sam.h"

// Klipper
#include "command.h" // DECL_CONSTANT
#include "sched.h" // sched_main

DECL_CONSTANT(MCU, "sam4e8e");

#define WDT_PASSWORD 0xA5000000
#define WDT_SLOW_CLOCK_DIV 128

void
watchdog_reset(void)
{
    WDT->WDT_CR = WDT_PASSWORD | WDT_CR_WDRSTT;
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    uint32_t timeout = 500000 / (WDT_SLOW_CLOCK_DIV * 1000000 / 32768UL);
    WDT->WDT_MR = WDT_MR_WDRSTEN | WDT_MR_WDV(timeout) | WDT_MR_WDD(timeout);
}
DECL_INIT(watchdog_init);

void
command_reset(uint32_t *args)
{
    NVIC_SystemReset();
}
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");

// Main entry point
int
main(void)
{
    SystemInit();

    // Enable Cache
    if (!(CMCC->CMCC_SR & CMCC_SR_CSTS))
        CMCC->CMCC_CTRL = CMCC_CTRL_CEN;

    // Start main loop
    sched_main();
    return 0;
}
