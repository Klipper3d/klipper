// Communicate with a DS18B20 temperature sensor on linux
//
// Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <fcntl.h> // open
#include <stdio.h> // snprintf
#include <stdlib.h> // atof
#include <string.h> // memchr
#include <unistd.h> // read
#include "basecmd.h" // oid_alloc
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_COMMAND
#include "internal.h" // report_errno
#include "sched.h" // DECL_SHUTDOWN
#include "board/misc.h"

struct ds18_s {
    struct timer timer;
    int fd;
    uint32_t rest_time, min_value, max_value;
    uint8_t flags;
};

enum {
    TS_PENDING = 1,
};

static struct task_wake ds18_wake;

static uint_fast8_t
ds18_event(struct timer *timer)
{
    struct ds18_s *d = container_of(timer, struct ds18_s, timer);
    // Trigger task to read and send results
    sched_wake_task(&ds18_wake);
    d->flags |= TS_PENDING;
    d->timer.waketime += d->rest_time;
    return SF_RESCHEDULE;
}

void
command_config_ds18b20(uint32_t *args)
{
    // Open kernel port
    uint8_t serial_len = args[1];
    uint8_t *serial = (void*)(size_t)args[2];
    if (memchr(serial, '/', serial_len))
        goto fail1;
    char fname[56];
    snprintf(fname, sizeof(fname), "/sys/bus/w1/devices/%.*s/temperature"
             , serial_len, serial);
    output("fname: %s", fname);
    int fd = open(fname, O_RDONLY|O_CLOEXEC);
    if (fd < 0) {
        report_errno("open ds18", fd);
        goto fail2;
    }
    int ret = set_non_blocking(fd);
    if (ret < 0)
        goto fail3;

    struct ds18_s *d = oid_alloc(args[0], command_config_ds18b20, sizeof(*d));
    d->fd = fd;
    d->timer.func = ds18_event;
    return;
fail1:
    shutdown("Invalid ds18 serial id 1");
fail2:
    shutdown("Invalid ds18 serial id 2");
fail3:
    shutdown("Invalid ds18 serial id 3");
}
DECL_COMMAND(command_config_ds18b20, "config_ds18b20 oid=%c serial=%*s");

void
command_query_ds18b20(uint32_t *args)
{
    struct ds18_s *d = oid_lookup(args[0], command_config_ds18b20);

    sched_del_timer(&d->timer);
    d->timer.waketime = args[1];
    d->rest_time = args[2];
    if (! d->rest_time)
        return;
    d->min_value = args[3];
    d->max_value = args[4];
    sched_add_timer(&d->timer);
}
DECL_COMMAND(command_query_ds18b20,
             "query_ds18b20 oid=%c clock=%u rest_ticks=%u"
             " min_value=%u max_value=%u");

// Read temperature and report
static void
ds18_read(struct ds18_s *d, uint32_t next_begin_time, uint8_t oid)
{
    // Read data and report
    char data[16];
    uint32_t t1 = timer_read_time();
    int ret = read(d->fd, data, sizeof(data)-1);
    uint32_t t2 = timer_read_time();
    if (ret < 0) {
        report_errno("read ds18b20", ret);
        try_shutdown("Unable to read DS18B20");
    }
    data[ret] = '\0';
    int val = atoi(data);
    sendf("ds18_result oid=%c next_clock=%u value=%u"
          , oid, next_begin_time, val);
    if (val < d->min_value || val > d->max_value)
        try_shutdown("DS18 out of range");

    // Seek file in preparation of next read
    uint32_t t3 = timer_read_time();
    ret = lseek(d->fd, 0, SEEK_SET);
    uint32_t t4 = timer_read_time();
    if (ret < 0) {
        report_errno("seek ds18b20", ret);
        try_shutdown("Unable to seek DS18B20");
    }

    output("read timing t1=%u t2=%u t3=%u t4=%u", t1, t2, t3, t4);
}

// task to read temperature and send response
void
ds18_task(void)
{
    if (!sched_check_wake(&ds18_wake))
        return;
    uint8_t oid;
    struct ds18_s *d;
    foreach_oid(oid, d, command_config_ds18b20) {
        if (!(d->flags & TS_PENDING))
            continue;
        irq_disable();
        uint32_t next_begin_time = d->timer.waketime;
        d->flags &= ~TS_PENDING;
        irq_enable();
        ds18_read(d, next_begin_time, oid);
    }
}
DECL_TASK(ds18_task);
