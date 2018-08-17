// Initialization of AVR watchdog timer.
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/wdt.h> // wdt_enable
#include "command.h" // DECL_COMMAND_FLAGS
#include "irq.h" // irq_disable
#include "sched.h" // DECL_TASK

void
watchdog_reset(void)
{
    wdt_reset();
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    // 0.5s timeout, interrupt and system reset
    wdt_enable(WDTO_500MS);
}
DECL_INIT(watchdog_init);

// Very early reset of the watchdog
void __attribute__((naked)) __visible __section(".init3")
watchdog_early_init(void)
{
    MCUSR = 0;
    wdt_disable();
}

// Support reset on AVR via the watchdog timer
void
command_reset(uint32_t *args)
{
    irq_disable();
    wdt_enable(WDTO_15MS);
    for (;;)
        ;
}
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");
