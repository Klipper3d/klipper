// Main starting point for micro-controller code running on Raspberry Pi's on Linux.
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2024 Liam Powell <klipper@liampwll.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#define _GNU_SOURCE
#include </usr/include/sched.h> // sched_setscheduler sched_get_priority_max
#include <stdio.h> // fprintf
#include <stdlib.h>
#include <string.h> // memset
#include <unistd.h> // getopt
#include <sys/mman.h> // mlockall MCL_CURRENT MCL_FUTURE
#include "board/misc.h" // console_sendf
#include "command.h" // DECL_CONSTANT
#include "internal.h" // console_setup
#include "sched.h" // sched_main
#include "gpio.h"

DECL_CONSTANT_STR("MCU", "raspberrypi");

static void int_handler(int _unused) {
    exit(0);
}

/****************************************************************
 * Real-time setup
 ****************************************************************/

static int
realtime_setup(void)
{
    struct sched_param sp;
    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = sched_get_priority_max(SCHED_FIFO) / 2;
    int ret = sched_setscheduler(0, SCHED_FIFO, &sp);
    if (ret < 0) {
        report_errno("sched_setscheduler", ret);
        return -1;
    }
    // Lock ourselves into memory
    ret = mlockall(MCL_CURRENT | MCL_FUTURE);
    if (ret) {
        report_errno("mlockall", ret);
        return -1;
    }
    return 0;
}


/****************************************************************
 * Restart
 ****************************************************************/

static char **orig_argv;

void
command_config_reset(uint32_t *args)
{
    if (! sched_is_shutdown())
        shutdown("config_reset only available when shutdown");
    int ret = execv(orig_argv[0], orig_argv);
    report_errno("execv", ret);
}
DECL_COMMAND_FLAGS(command_config_reset, HF_IN_SHUTDOWN, "config_reset");


/****************************************************************
 * Startup
 ****************************************************************/

int
main(int argc, char **argv)
{
    signal(SIGINT, int_handler);

    // Parse program args
    orig_argv = argv;
    int opt, realtime_cpu = 3;
    char *serial = "/tmp/klipper_host_mcu";
    while ((opt = getopt(argc, argv, "I:c:")) != -1) {
        switch (opt) {
        case 'I':
            serial = optarg;
            break;
        case 'c':
            realtime_cpu = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-I path] [-c core]\n", argv[0]);
            return -1;
        }
    }

    // Initial setup
    if (console_setup(serial)) {
        fprintf(stderr, "Console setup failed.");
        return -1;
    }

    if (gpio_initialise()) {
        fprintf(stderr, "GPIO setup failed.");
        return -1;
    }

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(realtime_cpu, &cpuset);
    if (sched_setaffinity(getpid(), sizeof(cpuset), &cpuset)) {
        fprintf(stderr, "Could not set CPU affinity.");
        return -1;
    }

    if (realtime_setup()) {
        fprintf(stderr, "Realtime setup failed.");
        return -1;
    }

    // Main loop
    sched_main();
    return 0;
}
