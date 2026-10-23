// Main starting point for micro-controller code running on linux systems
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <errno.h> // errno
#include </usr/include/sched.h> // sched_setscheduler sched_get_priority_max
#include <stdio.h> // fprintf
#include <string.h> // memset
#include <stdlib.h> // strtoul
#include <sys/types.h> // getgrent struct
#include <grp.h> // getgrnam
#include <unistd.h> // getopt
#include <sys/mman.h> // mlockall MCL_CURRENT MCL_FUTURE
#include "board/misc.h" // console_sendf
#include "command.h" // DECL_CONSTANT
#include "internal.h" // console_setup
#include "sched.h" // sched_main

DECL_CONSTANT_STR("MCU", "linux");


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
    // Parse program args
    orig_argv = argv;
    int opt, watchdog = 0, realtime = 0;
    char *serial = "/tmp/klipper_host_mcu";
    char *pty_group_name = NULL;
    mode_t pty_mode = 0660;
    while ((opt = getopt(argc, argv, "wrI:g:m:")) != -1) {
        switch (opt) {
        case 'w':
            watchdog = 1;
            break;
        case 'r':
            realtime = 1;
            break;
        case 'I':
            serial = optarg;
            break;
        case 'g':
            pty_group_name = optarg;
            break;
        case 'm':
            errno = 0; // see man 2 strtoul
            unsigned long int input_pty_mode = strtoul(optarg, NULL, 8);
            if (errno) {
                fprintf(stderr, "unable to parse pty mode: %s", optarg);
                return -1;
            }
            if (input_pty_mode >  0777ul) {
                fprintf(stderr,
                        "mode %s invalid (setuid/gid/sticky not supported)",
                        optarg);
                return -1;
            }
            pty_mode = input_pty_mode;
            break;
        default:
            fprintf(stderr,
                    "Usage: %s [-w] [-r] [-I path] [-g group] [-m filemode]\n",
                    argv[0]);
            return -1;
        }
    }

    // Initial setup
    if (realtime) {
        int ret = realtime_setup();
        if (ret)
            return ret;
    }
    gid_t pty_group_id = -1;
    if (pty_group_name) {
        struct group * pty_group = getgrnam(pty_group_name);
        if (!pty_group) {
            fprintf(stderr, "Unable to find group \"%s\"\n", pty_group_name);
            return -1;
        }
        pty_group_id = pty_group->gr_gid;
    }

    int ret = console_setup(serial, pty_mode, pty_group_id);
    if (ret)
        return -1;
    if (watchdog) {
        int ret = watchdog_setup();
        if (ret)
            return ret;
    }

    // Main loop
    sched_main();
    return 0;
}
