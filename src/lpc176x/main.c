// Main starting point for LPC176x boards.
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "LPC17xx.h" // NVIC_SystemReset
#include "command.h" // DECL_CONSTANT
#include "sched.h" // sched_main

DECL_CONSTANT(MCU, "lpc176x");


/****************************************************************
 * watchdog handler
 ****************************************************************/

void
watchdog_reset(void)
{
    LPC_WDT->WDFEED = 0xaa;
    LPC_WDT->WDFEED = 0x55;
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    LPC_WDT->WDTC = 4000000 / 2; // 500ms timeout
    LPC_WDT->WDCLKSEL = 1<<31; // Lock to internal RC
    LPC_WDT->WDMOD = 0x03; // select reset and enable
    watchdog_reset();
}
DECL_INIT(watchdog_init);


/****************************************************************
 * misc functions
 ****************************************************************/

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
    sched_main();
    return 0;
}
