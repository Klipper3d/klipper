// Initialization of AVR watchdog timer.
//
// Copyright (C) 2014  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include <avr/interrupt.h> // WDT_vect
#include <avr/wdt.h> // wdt_enable
#include "sched.h" // DECL_TASK

static uint8_t watchdog_shutdown;

ISR(WDT_vect)
{
    shutdown("Watchdog timer!");
    watchdog_shutdown = 1;
}

static void
watchdog_reset(void)
{
    wdt_reset();
    if (watchdog_shutdown) {
        WDTCSR |= 1<<WDIE;
        watchdog_shutdown = 0;
    }
}
DECL_TASK(watchdog_reset);

static void
watchdog_init(void)
{
    // 0.5s timeout, interrupt and system reset
    wdt_enable(WDTO_500MS);
    WDTCSR |= 1<<WDIE;
}
DECL_INIT(watchdog_init);
