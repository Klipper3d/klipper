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
#include <pthread.h> // pthread_create
#include "basecmd.h" // oid_alloc
#include "board/irq.h" // irq_disable
#include "board/misc.h" // output
#include "command.h" // DECL_COMMAND
#include "internal.h" // report_errno
#include "sched.h" // DECL_SHUTDOWN

// Status of a sensor
enum {
    W1_IDLE = 0, // No read requested yet
    W1_READ_REQUESTED = 1, // Reading or waiting to read
    W1_READY = 2, // Read complete, waiting to report
    W1_ERROR = 3, // Request shutdown
};

enum {
    TS_PENDING = 1,
};

struct ds18_s {
    struct timer timer;
    uint32_t rest_time;
    int32_t min_value, max_value;
    uint8_t flags;

    // Set by main thread in configuration phase.
    // Should only be accessed by reader thread after configuration.
    int fd;

    // Used for guarding shared members.
    pthread_mutex_t lock;
    pthread_cond_t cond;

    // Protect all reads/writes to the following members using the mutex
    // once reader thread is initialized.
    int temperature;
    uint32_t request_time;
    uint8_t status;
    const char* error;
};

// Lock ds18_s mutex, set error status and message, unlock mutex.
static void
locking_set_read_error(struct ds18_s *d, const char *error)
{
    pthread_mutex_lock(&d->lock);
    d->error = error;
    d->status = W1_ERROR;
    pthread_mutex_unlock(&d->lock);
}

// The kernel interface to DS18B20 sensors is a sysfs entry that blocks for
// around 750ms when read. Most of this is idle time waiting for the result
// to be ready. Read in a separate thread in order to avoid blocking time-
// sensitive work.
static void *
reader_start_routine(void *param) {
    struct ds18_s *d = param;
    for (;;) {
        // Wait for requests to read temperature sensors
        pthread_mutex_lock(&d->lock);
        while (d->status != W1_READ_REQUESTED) {
            pthread_cond_wait(&d->cond, &d->lock);
        }
        pthread_mutex_unlock(&d->lock);

        // Read temp.
        // The temperature data is at the end of the report, after a "t=".
        // Example (3.062 degrees C):
        //
        // 31 00 4b 46 7f ff 0c 10 77 : crc=77 YES
        // 31 00 4b 46 7f ff 0c 10 77 t=3062
        char data[128];
        int ret = read(d->fd, data, sizeof(data)-1);
        if (ret < 0) {
            report_errno("read DS18B20", ret);
            locking_set_read_error(d, "Unable to read DS18B20");
            pthread_exit(NULL);
        }
        data[ret] = '\0';
        char *temp_string = strstr(data, "t=");
        if (temp_string == NULL || temp_string[2] == '\0') {
            locking_set_read_error(d, "Unable to find temperature value in DS18B20 report");
            pthread_exit(NULL);
        }
        // Don't pass 't' and '=' to atoi
        temp_string += 2;
        int val = atoi(temp_string);

        // Store temperature
        pthread_mutex_lock(&d->lock);
        d->status = W1_READY;
        d->temperature = val;
        pthread_mutex_unlock(&d->lock);

        // Seek file in preparation of next read
        ret = lseek(d->fd, 0, SEEK_SET);
        if (ret < 0) {
            report_errno("seek DS18B20", ret);
            locking_set_read_error(d, "Unable to seek DS18B20");
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

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
    snprintf(fname, sizeof(fname), "/sys/bus/w1/devices/%.*s/w1_slave"
             , serial_len, serial);
    output("fname: %s", fname);
    uint32_t t1 = timer_read_time();
    int fd = open(fname, O_RDONLY|O_CLOEXEC);
    uint32_t t2 = timer_read_time();
    output("read timing opening file t1=%u t2=%u", t1, t2);
    if (fd < 0) {
        report_errno("open ds18", fd);
        goto fail2;
    }
    int ret = set_non_blocking(fd);
    if (ret < 0)
        goto fail3;

    struct ds18_s *d = oid_alloc(args[0], command_config_ds18b20, sizeof(*d));
    d->timer.func = ds18_event;
    d->fd = fd;
    d->status = W1_IDLE;
    pthread_mutex_init(&d->lock, NULL);
    pthread_cond_init(&d->cond, NULL);

    pthread_t reader_tid; // Do we need
    if (pthread_create(&reader_tid, NULL, reader_start_routine, d) != 0) {
        goto fail4;
    }

    return;
fail1:
    shutdown("Invalid DS18B20 serial id 1");
fail2:
    shutdown("Invalid DS18B20 serial id 2");
fail3:
    shutdown("Invalid DS18B20 serial id 3");
fail4:
    shutdown("Could not start reader thread");
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
             " min_value=%i max_value=%i");

// Report temperature if ready, and set back to pending.
static void
ds18_send_and_request(struct ds18_s *d, uint32_t next_begin_time, uint8_t oid)
{
    uint32_t request_time = timer_read_time();
    pthread_mutex_lock(&d->lock);
    if (d->status == W1_ERROR) {
        // Can't pass read error to try_shutdown, which expects a static string.
        // Output the specific error, then pass a generic error to try_shutdown.
        output("Error: %s", d->error);
        pthread_mutex_unlock(&d->lock);
        try_shutdown("Error reading DS18B20 sensor");
        return;
    } else if (d->status == W1_IDLE) {
        // This happens the first time requesting a temperature.
        // Nothing to report yet.
        d->request_time = request_time;
        d->status = W1_READ_REQUESTED;
    } else if (d->status == W1_READY) {
        // Report the previous temperature and request a new one.
        sendf("ds18_result oid=%c next_clock=%u value=%i"
              , oid, next_begin_time, d->temperature);
        if (d->temperature < d->min_value || d->temperature > d->max_value) {
            pthread_mutex_unlock(&d->lock);
            try_shutdown("DS18B20 out of range");
            return;
        }
        d->request_time = request_time;
        d->status = W1_READ_REQUESTED;
    } else if (d->status == W1_READ_REQUESTED) {
        // This is a sign that we are not keeping up with the polling interval.
        // Should that trigger an error?
        pthread_mutex_unlock(&d->lock);
        try_shutdown("DS18B20 sensors didn't respond within polling interval");
        return;
    }
    pthread_cond_signal(&d->cond);
    pthread_mutex_unlock(&d->lock);
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
        ds18_send_and_request(d, next_begin_time, oid);
    }
}
DECL_TASK(ds18_task);
