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

enum {
    W1_IDLE = 0, // No requests
    W1_READ_REQUESTED = 1, // Reading or waiting to read
    W1_READY = 2, // Read complete, waiting to report
    W1_ERROR = 3, // Request shutdown
};

enum {
    TS_PENDING = 1,
};

struct ds18_sysfs_reader_s {
    int temperature;
    uint32_t request_time;
    uint8_t status;
    int fd;
    const char* error;
};

struct ds18_worker_s {
    pthread_t tid;
    // Lock all reads/writes to readers and readers_count.
    // The worker thread will read and seek file descriptors without obtaining a lock.
    pthread_mutex_t lock;
    pthread_cond_t cond;
    struct ds18_sysfs_reader_s readers[4];
    int readers_count;
};

struct ds18_s {
    struct timer timer;
    struct ds18_worker_s* worker; // Always a reference to the same single worker
    uint32_t rest_time;
    int32_t min_value, max_value;
    uint8_t flags;
    uint8_t reader_index;
};

// This struct instance holds state for every reader.
static struct ds18_worker_s worker = {
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER
};
// Thread for worker will be started once with the first configured device.
int worker_started;

// Set error status and message for the reader at the specified index
static void
set_reader_error(struct ds18_worker_s *w, uint8_t reader_index, const char *error)
{
    pthread_mutex_lock(&w->lock);
    w->readers[reader_index].error = error;
    w->readers[reader_index].status = W1_ERROR;
    pthread_mutex_unlock(&w->lock);
}

// Return the index of the earliest-requested reader with status W1_READ_REQUESTED, or -1 if none found.
// Caller is responsible for holding the w->lock mutex during this call and for reads/writes to returned reader.
static int
get_next_reader_index(struct ds18_worker_s *w)
{
    int reader_index = -1;
    uint32_t earliest_time; // Only valid if reader_index != -1
    if (w->readers_count > 0) {
        for (int i = 0; i < w->readers_count; i++) {
            if (w->readers[i].status == W1_READ_REQUESTED &&
                (reader_index == -1 ||  timer_is_before(earliest_time, w->readers[i].request_time))) {
                reader_index = i;
                earliest_time = w->readers[i].request_time;
            }
        }
    }
    return reader_index;
}

static void *
worker_start_routine(void *param) {
    struct ds18_worker_s *w = param;
    for (;;) {
        int fd;
        int reader_index;

        // Wait for requests to read temperature sensors
        pthread_mutex_lock(&w->lock);
        while ((reader_index = get_next_reader_index(w)) == -1) {
            pthread_cond_wait(&w->cond, &w->lock);
        }
        fd = w->readers[reader_index].fd;
        pthread_mutex_unlock(&w->lock);

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
            set_reader_error(w, reader_index, "Unable to read DS18B20");
            pthread_exit(NULL);
        }
        data[ret] = '\0';
        char *temp_string = strstr(data, "t=");
        if (temp_string == NULL || temp_string[2] == '\0') {
            set_reader_error(w, reader_index, "Unable to find temperature value in DS18B20 report");
            pthread_exit(NULL);
        }
        temp_string += 2;
        int val = atoi(temp_string);

        // Store temperature
        pthread_mutex_lock(&w->lock);
        w->readers[reader_index].status = W1_READY;
        w->readers[reader_index].temperature = val;
        pthread_mutex_unlock(&w->lock);

        // Seek file in preparation of next read
        ret = lseek(fd, 0, SEEK_SET);
        if (ret < 0) {
            report_errno("seek DS18B20", ret);
            set_reader_error(w, reader_index, "Unable to seek DS18B20");
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
    // start the worker.
    if (!worker_started) {
        if (pthread_create(&worker.tid, NULL, worker_start_routine, &worker) != 0) {
            goto fail4;
        }
        worker_started = 1;
    }

    // Add entry for worker thread.
    pthread_mutex_lock(&worker.lock);
    if (worker.readers_count >= ARRAY_SIZE(worker.readers)) {
        pthread_mutex_unlock(&worker.lock);
        goto fail5;
    }
    d->reader_index = worker.readers_count;
    d->worker = &worker;
    worker.readers[worker.readers_count].fd = fd;
    worker.readers_count++;
    pthread_mutex_unlock(&worker.lock);
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
    struct ds18_worker_s *w = d->worker;
    struct ds18_sysfs_reader_s *r = &d->worker->readers[d->reader_index];
    pthread_mutex_lock(&w->lock);
    if (r->status == W1_ERROR) {
        pthread_mutex_unlock(&w->lock);
        // Can't pass worker error to try_shutdown, which expects a static string.
        output("Error: %s", r->error);
        try_shutdown("Error reading DS18B20 sensor");
    } else if (r->status == W1_IDLE) {
        // This happens the first time requesting a temperature.
        // Nothing to report yet.
        r->request_time = request_time;
        r->status = W1_READ_REQUESTED;
    } else if (r->status == W1_READY) {
        // Report the previous temperature and request a new one.
        int val = r->temperature;
        sendf("ds18_result oid=%c next_clock=%u value=%i"
              , oid, next_begin_time, val);
        if (val < d->min_value || val > d->max_value) {
            pthread_mutex_unlock(&w->lock);
            try_shutdown("DS18B20 out of range");
        }
        r->request_time = request_time;
        r->status = W1_READ_REQUESTED;
    } else if (r->status == W1_READ_REQUESTED) {
        // This is a sign that we are not keeping up with the polling interval.
        // Should that trigger an error?
        pthread_mutex_unlock(&w->lock);
        try_shutdown("DS18B20 sensors didn't respond within polling interval");
    }
    pthread_cond_signal(&w->cond);
    pthread_mutex_unlock(&w->lock);
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
