// Support for Linux watchdog
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <fcntl.h> // open
#include <unistd.h> // write
#include "internal.h" // report_errno
#include "sched.h" // DECL_TASK

static int watchdog_fd = -1;

int
watchdog_setup(void)
{
    int ret = open("/dev/watchdog", O_RDWR|O_CLOEXEC);
    if (ret < 0) {
        report_errno("watchdog open", ret);
        return -1;
    }
    watchdog_fd = ret;
    return set_non_blocking(watchdog_fd);
}

void
watchdog_task(void)
{
    static uint32_t next_watchdog_time;
    if (watchdog_fd < 0 || !timer_check_periodic(&next_watchdog_time))
        return;
    int ret = write(watchdog_fd, ".", 1);
    if (ret <= 0)
        report_errno("watchdog write", ret);
}
DECL_TASK(watchdog_task);
