// Code to setup clocks on Huada HC32F460
//
// Copyright (C) 2022  Steven Gotthardt <gotthardt@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_MACH_AVR
#include "sched.h"
#include "system_hc32f460.h"
#include "hc32f460_gpio.h"


/****************************************************************
 * Startup
 ****************************************************************/

// Main entry point - called from armcm_boot.c:ResetHandler()
void  __attribute__((noreturn))
armcm_main(void)
{
    // sets the system clock speed variable for library use
    SystemInit();

    // disable JTAG/SWD on pins PA13, PA14, PA15, PB3, PB4
    // SWD still works until the relevant pins are reconfigured. Proprietary
    // flash program (XHSC ISP) must be used to reflash afterwards.
    PORT_DebugPortSetting(ALL_DBG_PIN, Disable);

    // manage the system
    sched_main();

    // never get here
    for (;;)  ;
}
