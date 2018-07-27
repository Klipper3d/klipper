// Main starting point for SAM4e8e boards.
//
// Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // DECL_CONSTANT
#include "sam4e8e.h"
#include "sysclk.h"
#include "board.h"
#include "wdt/wdt.h"
#include "sched.h" // sched_main

DECL_CONSTANT(MCU, "sam4e8e");

/****************************************************************
 * watchdog handler
 ****************************************************************/

void
watchdog_reset(void)
{
    wdt_restart(WDT);
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    Wdt *p_wdt = WDT;
    uint32_t wdt_mode = WDT_MR_WDRSTEN; // WDReset enable
    uint32_t timeout = wdt_get_timeout_value(500000, BOARD_FREQ_SLCK_XTAL);
    wdt_init(p_wdt, wdt_mode, timeout, timeout);
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
