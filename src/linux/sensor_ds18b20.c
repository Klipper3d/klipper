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

struct ds18_s {
    struct timer timer;
    uint32_t rest_time;
    int32_t min_value, max_value;
    uint8_t flags;
    uint8_t worker_entry_index;
};

enum {
    W1_IDLE = 0, // No requests
    W1_PENDING = 1, // Reading or waiting to read
    W1_READY = 2, // Read complete, waiting to report
    W1_ERROR = 3, // Request shutdown
};

struct ds18_worker_entry_s {
    int temperature;
    uint32_t request_time;
    uint8_t status;
    int fd;
    const char* error;
};
static struct ds18_worker_entry_s worker_entries[16];
static int worker_entries_count;

enum {
    TS_PENDING = 1,
};

// Lock all reads/writes to worker_entries and worker_entries_count.
// The worker thread will read and seek file descriptors without obtaining a lock.
static pthread_mutex_t worker_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t worker_cond = PTHREAD_COND_INITIALIZER;
static pthread_t worker_tid;
static int worker_started = 0;

// Set error status and message for the specified worker entry
static void
set_worker_error(uint8_t worker_entry_index, const char *error)
{
    pthread_mutex_lock(&worker_lock);
    worker_entries[worker_entry_index].error = error;
    worker_entries[worker_entry_index].status = W1_ERROR;
    pthread_mutex_unlock(&worker_lock);
}

static void *
worker(void *param) {
    for (;;) {
        int fd;
        int worker_entry_index;

        // Wait for requests to read temperature sensors
        pthread_mutex_lock(&worker_lock);
        for (;;) {
            // Find the pending entry (if any) with the earliest request time.
            worker_entry_index = -1;
            uint32_t earliest_time; // Only valid if worker_entry_index != -1
            if (worker_entries_count > 0) {
                for (int i = 0; i < worker_entries_count; i++) {
                    if (worker_entries[i].status == W1_PENDING &&
                        (worker_entry_index == -1 ||  timer_is_before(earliest_time, worker_entries[i].request_time))) {
                        worker_entry_index = i;
                        earliest_time = worker_entries[i].request_time;
                    }
                }
            }
            if (worker_entry_index != -1)
                break;
            pthread_cond_wait(&worker_cond, &worker_lock);
        }
        fd = worker_entries[worker_entry_index].fd;
        pthread_mutex_unlock(&worker_lock);

        // Read temp.
        // The temperature data is at the end of the report, after a "t=".
        // Example (3.062 degrees C):
        //
        // 31 00 4b 46 7f ff 0c 10 77 : crc=77 YES
        // 31 00 4b 46 7f ff 0c 10 77 t=3062
        char data[128];
        int ret = read(fd, data, sizeof(data)-1);
        if (ret < 0) {
            report_errno("read DS18B20", ret);
            set_worker_error(worker_entry_index, "Unable to read DS18B20");
            pthread_exit(NULL);
        }
        data[ret] = '\0';
        char *temp_string = strstr(data, "t=");
        if (temp_string == NULL || temp_string[2] == '\0') {
            set_worker_error(worker_entry_index, "Unable to find temperature value in DS18B20 report");
            pthread_exit(NULL);
        }
        temp_string += 2;
        int val = atoi(temp_string);

        // Store temperature
        pthread_mutex_lock(&worker_lock);
        worker_entries[worker_entry_index].status = W1_READY;
        worker_entries[worker_entry_index].temperature = val;
        pthread_mutex_unlock(&worker_lock);

        // Seek file in preparation of next read
        ret = lseek(fd, 0, SEEK_SET);
        if (ret < 0) {
            report_errno("seek DS18B20", ret);
            set_worker_error(worker_entry_index, "Unable to seek DS18B20");
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

    // Sensors share one worker thread. If this is the first sensor configured,
    // start the worker thread.
    if (!worker_started) {
        if (pthread_create(&worker_tid, NULL, worker, NULL) != 0) {
            goto fail4;
        }
        worker_started = 1;
    }

    // Add entry for worker thread.
    pthread_mutex_lock(&worker_lock);
    if (worker_entries_count >= ARRAY_SIZE(worker_entries)) {
        pthread_mutex_unlock(&worker_lock);
        goto fail5;
    }
    d->worker_entry_index = worker_entries_count;
    worker_entries[worker_entries_count].fd = fd;
    worker_entries_count++;
    pthread_mutex_unlock(&worker_lock);
    return;
fail1:
    shutdown("Invalid DS18B20 serial id 1");
fail2:
    shutdown("Invalid DS18B20 serial id 2");
fail3:
    shutdown("Invalid DS18B20 serial id 3");
fail4:
    shutdown("Could not start worker thread");
fail5:
    shutdown("Too many DS18B20 sensors configured");
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
    pthread_mutex_lock(&worker_lock);
    if (worker_entries[d->worker_entry_index].status == W1_ERROR) {
        pthread_mutex_unlock(&worker_lock);
        // Can't pass worker error to try_shutdown, which expects a static string.
        output("Error: %s", worker_entries[d->worker_entry_index].error);
        try_shutdown("Error reading DS18B20 sensor");
    } else if (worker_entries[d->worker_entry_index].status == W1_IDLE) {
        // This happens the first time requesting a temperature.
        // Nothing to report yet.
        worker_entries[d->worker_entry_index].request_time = request_time;
        worker_entries[d->worker_entry_index].status = W1_PENDING;
    } else if (worker_entries[d->worker_entry_index].status == W1_READY) {
        // Report the previous temperature and request a new one.
        int val = worker_entries[d->worker_entry_index].temperature;
        sendf("ds18_result oid=%c next_clock=%u value=%i"
              , oid, next_begin_time, val);
        if (val < d->min_value || val > d->max_value) {
            pthread_mutex_unlock(&worker_lock);
            try_shutdown("DS18B20 out of range");
        }
        worker_entries[d->worker_entry_index].request_time = request_time;
        worker_entries[d->worker_entry_index].status = W1_PENDING;
    } else if (worker_entries[d->worker_entry_index].status == W1_PENDING) {
        // This is a sign that we are not keeping up with the polling interval.
        // Should that trigger an error?
        pthread_mutex_unlock(&worker_lock);
        try_shutdown("DS18B20 sensors didn't respond within polling interval");
    }
    pthread_cond_signal(&worker_cond);
    pthread_mutex_unlock(&worker_lock);
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
