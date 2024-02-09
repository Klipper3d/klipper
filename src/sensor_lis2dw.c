// Support for gathering acceleration data from LIS2DW chip
//
// Copyright (C) 2023  Zhou.XianMing <zhouxm@biqu3d.com>
// Copyright (C) 2020-2023  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "sensor_bulk.h" // sensor_bulk_report
#include "spicmds.h" // spidev_transfer

#define LIS_AR_DATAX0 0x28
#define LIS_AM_READ   0x80
#define LIS_FIFO_SAMPLES 0x2F

#define BYTES_PER_SAMPLE 6

struct lis2dw {
    struct timer timer;
    uint32_t rest_ticks;
    struct spidev_s *spi;
    uint8_t flags;
    struct sensor_bulk sb;
};

enum {
    LIS_PENDING = 1<<0,
};

static struct task_wake lis2dw_wake;

// Event handler that wakes lis2dw_task() periodically
static uint_fast8_t
lis2dw_event(struct timer *timer)
{
    struct lis2dw *ax = container_of(timer, struct lis2dw, timer);
    ax->flags |= LIS_PENDING;
    sched_wake_task(&lis2dw_wake);
    return SF_DONE;
}

void
command_config_lis2dw(uint32_t *args)
{
    struct lis2dw *ax = oid_alloc(args[0], command_config_lis2dw
                                   , sizeof(*ax));
    ax->timer.func = lis2dw_event;
    ax->spi = spidev_oid_lookup(args[1]);
}
DECL_COMMAND(command_config_lis2dw, "config_lis2dw oid=%c spi_oid=%c");

// Helper code to reschedule the lis2dw_event() timer
static void
lis2dw_reschedule_timer(struct lis2dw *ax)
{
    irq_disable();
    ax->timer.waketime = timer_read_time() + ax->rest_ticks;
    sched_add_timer(&ax->timer);
    irq_enable();
}

// Query accelerometer data
static void
lis2dw_query(struct lis2dw *ax, uint8_t oid)
{
    uint8_t msg[7] = {0};
    uint8_t fifo[2] = {LIS_FIFO_SAMPLES| LIS_AM_READ , 0};
    uint8_t fifo_empty,fifo_ovrn = 0;

    msg[0] = LIS_AR_DATAX0 | LIS_AM_READ ;
    uint8_t *d = &ax->sb.data[ax->sb.data_count];

    spidev_transfer(ax->spi, 1, sizeof(msg), msg);

    spidev_transfer(ax->spi, 1, sizeof(fifo), fifo);
    fifo_empty = fifo[1]&0x3F;
    fifo_ovrn = fifo[1]&0x40;

    d[0] = msg[1]; // x low bits
    d[1] = msg[2]; // x high bits
    d[2] = msg[3]; // y low bits
    d[3] = msg[4]; // y high bits
    d[4] = msg[5]; // z low bits
    d[5] = msg[6]; // z high bits

    ax->sb.data_count += BYTES_PER_SAMPLE;
    if (ax->sb.data_count + BYTES_PER_SAMPLE > ARRAY_SIZE(ax->sb.data))
        sensor_bulk_report(&ax->sb, oid);

    // Check fifo status
    if (fifo_ovrn)
        ax->sb.possible_overflows++;

    // check if we need to run the task again (more packets in fifo?)
    if (!fifo_empty) {
        // More data in fifo - wake this task again
        sched_wake_task(&lis2dw_wake);
    } else {
        // Sleep until next check time
        ax->flags &= ~LIS_PENDING;
        lis2dw_reschedule_timer(ax);
    }
}

void
command_query_lis2dw(uint32_t *args)
{
    struct lis2dw *ax = oid_lookup(args[0], command_config_lis2dw);

    sched_del_timer(&ax->timer);
    ax->flags = 0;
    if (!args[1])
        // End measurements
        return;

    // Start new measurements query
    ax->rest_ticks = args[1];
    sensor_bulk_reset(&ax->sb);
    lis2dw_reschedule_timer(ax);
}
DECL_COMMAND(command_query_lis2dw, "query_lis2dw oid=%c rest_ticks=%u");

void
command_query_lis2dw_status(uint32_t *args)
{
    struct lis2dw *ax = oid_lookup(args[0], command_config_lis2dw);
    uint8_t msg[2] = { LIS_FIFO_SAMPLES | LIS_AM_READ, 0x00 };
    uint32_t time1 = timer_read_time();
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    uint32_t time2 = timer_read_time();
    sensor_bulk_status(&ax->sb, args[0], time1, time2-time1
                       , (msg[1] & 0x1f) * BYTES_PER_SAMPLE);
}
DECL_COMMAND(command_query_lis2dw_status, "query_lis2dw_status oid=%c");

void
lis2dw_task(void)
{
    if (!sched_check_wake(&lis2dw_wake))
        return;
    uint8_t oid;
    struct lis2dw *ax;
    foreach_oid(oid, ax, command_config_lis2dw) {
        uint_fast8_t flags = ax->flags;
        if (flags & LIS_PENDING)
            lis2dw_query(ax, oid);
    }
}
DECL_TASK(lis2dw_task);
