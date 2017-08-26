// Main starting point for micro-controller code running on linux systems
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdio.h> // fprintf
#include <unistd.h> // getopt
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
main(int argc, char **argv)
{
    int opt, watchdog = 0;
    while ((opt = getopt(argc, argv, "w")) != -1) {
        switch (opt) {
        case 'w':
            watchdog = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-w]\n", argv[0]);
            return -1;
        }
    }

    if (watchdog) {
        int ret = watchdog_setup();
        if (ret)
            return ret;
    }

    int ret = console_setup("/tmp/klipper_host_mcu");
    if (ret)
        return -1;

    sched_main();
    return 0;
}
