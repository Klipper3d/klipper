// Main starting point for micro-controller code running on linux systems
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/misc.h" // console_sendf
#include "command.h" // DECL_CONSTANT
#include "internal.h" // console_setup
#include "sched.h" // sched_main

DECL_CONSTANT(MCU, "linux");


/****************************************************************
 * Startup
 ****************************************************************/

// Main entry point for simulator.
int
main(void)
{
    int ret = console_setup("/tmp/klipper_host_mcu");
    if (ret)
        return -1;

    sched_main();
    return 0;
}
