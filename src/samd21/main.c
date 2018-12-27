// Main starting point for SAMD21 boards
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // DECL_CONSTANT
#include "samd21.h" // SystemInit
#include "sched.h" // sched_main

DECL_CONSTANT(MCU, "samd21g");


/****************************************************************
 * watchdog handler
 ****************************************************************/

void
watchdog_reset(void)
{
    WDT->CLEAR.reg = 0xa5;
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    WDT->CONFIG.reg = WDT_CONFIG_PER_16K; // 500ms timeout
    WDT->CTRL.reg = WDT_CTRL_ENABLE;
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
    SystemInit();
    sched_main();
    return 0;
}
