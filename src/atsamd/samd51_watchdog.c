// Watchdog handler on SAMD21 boards
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "internal.h" // WDT
#include "sched.h" // DECL_TASK

void
watchdog_reset(void)
{
    if (!(WDT->SYNCBUSY.reg & WDT_SYNCBUSY_CLEAR))
        WDT->CLEAR.reg = 0xa5;
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    WDT->CONFIG.reg = WDT_CONFIG_PER(6); // 500ms timeout
    WDT->CTRLA.reg = WDT_CTRLA_ENABLE;
}
DECL_INIT(watchdog_init);
