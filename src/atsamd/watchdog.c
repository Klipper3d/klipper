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
    if (!(WDT->STATUS.reg & WDT_STATUS_SYNCBUSY))
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
