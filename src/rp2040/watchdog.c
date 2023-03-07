// Watchdog code on rp2040
//
// Copyright (C) 2021-2022  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include "hardware/structs/psm.h" // psm_hw
#include "hardware/structs/watchdog.h" // watchdog_hw
#include "sched.h" // DECL_TASK

void
watchdog_reset(void)
{
    watchdog_hw->load = 0x800000; // ~350ms
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    psm_hw->wdsel = PSM_WDSEL_BITS & ~(PSM_WDSEL_ROSC_BITS|PSM_WDSEL_XOSC_BITS);
    watchdog_reset();
    watchdog_hw->ctrl = (WATCHDOG_CTRL_PAUSE_DBG0_BITS
                         | WATCHDOG_CTRL_PAUSE_DBG1_BITS
                         | WATCHDOG_CTRL_PAUSE_JTAG_BITS
                         | WATCHDOG_CTRL_ENABLE_BITS);
}
DECL_INIT(watchdog_init);
