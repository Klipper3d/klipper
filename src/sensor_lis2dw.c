// Support for gathering acceleration data from LIS2DW chip
//
// Copyright (C) 2023  Zhou.XianMing <zhouxm@biqu3d.com>
// Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "spicmds.h" // spidev_transfer

#define LIS_AR_DATAX0 0x28
#define LIS_AM_READ   0x80
#define LIS_FIFO_CTRL    0x2E
#define LIS_FIFO_SAMPLES 0x2F

struct lis2dw {
    struct timer timer;
    uint32_t rest_ticks;
    struct spidev_s *spi;
    uint16_t sequence, limit_count;
    uint8_t flags, data_count, fifo_disable;
    uint8_t data[48];
};

enum {
    LIS_HAVE_START = 1<<0, LIS_RUNNING = 1<<1, LIS_PENDING = 1<<2,
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

// Report local measurement buffer
static void
lis2dw_report(struct lis2dw *ax, uint8_t oid)
{
    sendf("lis2dw_data oid=%c sequence=%hu data=%*s"
          , oid, ax->sequence, ax->data_count, ax->data);
    ax->data_count = 0;
    ax->sequence++;
}

// Report buffer and fifo status
static void
lis2dw_status(struct lis2dw *ax, uint_fast8_t oid
            , uint32_t time1, uint32_t time2, uint_fast8_t fifo)
{
    sendf("lis2dw_status oid=%c clock=%u query_ticks=%u next_sequence=%hu"
          " buffered=%c fifo=%c limit_count=%hu"
          , oid, time1, time2-time1, ax->sequence
          , ax->data_count, fifo, ax->limit_count);
}

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
    uint8_t *d = &ax->data[ax->data_count];

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

    ax->data_count += 6;
    if (ax->data_count + 6 > ARRAY_SIZE(ax->data))
        lis2dw_report(ax, oid);

    // Check fifo status
    if (fifo_ovrn)
        ax->limit_count++;

    // check if we need to run the task again (more packets in fifo?)
    if (!fifo_empty&&!(ax->fifo_disable)) {
        // More data in fifo - wake this task again
        sched_wake_task(&lis2dw_wake);
    } else if (ax->flags & LIS_RUNNING) {
        // Sleep until next check time
        sched_del_timer(&ax->timer);
        ax->flags &= ~LIS_PENDING;
        lis2dw_reschedule_timer(ax);
    }
}

// Startup measurements
static void
lis2dw_start(struct lis2dw *ax, uint8_t oid)
{
    sched_del_timer(&ax->timer);
    ax->flags = LIS_RUNNING;
    ax->fifo_disable = 0;
    uint8_t ctrl[2] = {LIS_FIFO_CTRL , 0xC0};
    spidev_transfer(ax->spi, 0, sizeof(ctrl), ctrl);
    lis2dw_reschedule_timer(ax);
}

// End measurements
static void
lis2dw_stop(struct lis2dw *ax, uint8_t oid)
{
    // Disable measurements
    sched_del_timer(&ax->timer);
    ax->flags = 0;
    // Drain any measurements still in fifo
    ax->fifo_disable = 1;
    lis2dw_query(ax, oid);

    uint8_t ctrl[2] = {LIS_FIFO_CTRL , 0};
    uint32_t end1_time = timer_read_time();
    spidev_transfer(ax->spi, 0, sizeof(ctrl), ctrl);
    uint32_t end2_time = timer_read_time();

    uint8_t msg[2] = { LIS_FIFO_SAMPLES | LIS_AM_READ , 0};
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    uint8_t fifo_status = msg[1]&0x1f;

    //Report final data
    if (ax->data_count)
        lis2dw_report(ax, oid);
    lis2dw_status(ax, oid, end1_time, end2_time, fifo_status);
}

void
command_query_lis2dw(uint32_t *args)
{
    struct lis2dw *ax = oid_lookup(args[0], command_config_lis2dw);

    if (!args[2]) {
        // End measurements
        lis2dw_stop(ax, args[0]);
        return;
    }
    // Start new measurements query
    sched_del_timer(&ax->timer);
    ax->timer.waketime = args[1];
    ax->rest_ticks = args[2];
    ax->flags = LIS_HAVE_START;
    ax->sequence = ax->limit_count = 0;
    ax->data_count = 0;
    ax->fifo_disable = 0;
    sched_add_timer(&ax->timer);
}
DECL_COMMAND(command_query_lis2dw,
             "query_lis2dw oid=%c clock=%u rest_ticks=%u");

void
command_query_lis2dw_status(uint32_t *args)
{
    struct lis2dw *ax = oid_lookup(args[0], command_config_lis2dw);
    uint8_t msg[2] = { LIS_FIFO_SAMPLES | LIS_AM_READ, 0x00 };
    uint32_t time1 = timer_read_time();
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    uint32_t time2 = timer_read_time();
    lis2dw_status(ax, args[0], time1, time2, msg[1]&0x1f);
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
        if (!(flags & LIS_PENDING))
            continue;
        if (flags & LIS_HAVE_START)
            lis2dw_start(ax, oid);
        else
            lis2dw_query(ax, oid);
    }
}
DECL_TASK(lis2dw_task);
