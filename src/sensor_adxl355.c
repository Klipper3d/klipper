// Support for gathering acceleration data from ADXL355 chip
//
// Copyright (C) 2026  Sven Thiele <thiele61@gmx.de>
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

struct adxl355 {
    struct timer timer;
    uint32_t rest_ticks;
    struct spidev_s *spi;
    uint16_t fifo_bytes_pending;
    uint8_t flags;
    struct sensor_bulk sb;
};

enum {
    AX_PENDING = 1<<0,
};

static struct task_wake adxl355_wake;

// Event handler that wakes adxl355_task() periodically
static uint_fast8_t
adxl355_event(struct timer *timer)
{
    struct adxl355 *ax = container_of(timer, struct adxl355, timer);
    ax->flags |= AX_PENDING;
    sched_wake_task(&adxl355_wake);
    return SF_DONE;
}

void
command_config_adxl355(uint32_t *args)
{
    struct adxl355 *ax = oid_alloc(args[0], command_config_adxl355
                                   , sizeof(*ax));
    ax->timer.func = adxl355_event;
    ax->spi = spidev_oid_lookup(args[1]);
}
DECL_COMMAND(command_config_adxl355, "config_adxl355 oid=%c spi_oid=%c");

// Helper code to reschedule the adxl355_event() timer
static void
adxl355_reschedule_timer(struct adxl355 *ax)
{
    irq_disable();
    ax->timer.waketime = timer_read_time() + ax->rest_ticks;
    sched_add_timer(&ax->timer);
    irq_enable();
}

// Chip registers
#define AR_STATUS 0x04
#define AR_FIFO_ENTRIES 0x05
#define AR_FIFO_DATA 0x11

#define AM_READ(reg) ((((reg) & 0x7f) << 1) | 0x01)

#define BYTES_PER_FIFO_ENTRY 3
#define BYTES_PER_BLOCK 45
#define MAX_FIFO_ENTRIES 96

// Update local status tracking from newly read fifo registers
static int
update_fifo_status(struct adxl355 *ax, uint8_t status, uint8_t fifo_entries)
{
    if (status & (1<<2))
        ax->sb.possible_overflows++;
    if (fifo_entries > MAX_FIFO_ENTRIES)
        return -1;
    ax->fifo_bytes_pending = fifo_entries * BYTES_PER_FIFO_ENTRY;
    return 0;
}

// Query fifo status registers
static int
query_fifo_status(struct adxl355 *ax)
{
    uint8_t msg[3] = { AM_READ(AR_STATUS), 0x00, 0x00 };
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    return update_fifo_status(ax, msg[1], msg[2]);
}

// Read 15 fifo entries (5 complete xyz samples)
static void
read_fifo_block(struct adxl355 *ax, uint8_t oid)
{
    uint8_t msg[BYTES_PER_BLOCK + 1] = {0};
    msg[0] = AM_READ(AR_FIFO_DATA);
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    memcpy(ax->sb.data, &msg[1], BYTES_PER_BLOCK);
    ax->sb.data_count = BYTES_PER_BLOCK;
    sensor_bulk_report(&ax->sb, oid);
    ax->fifo_bytes_pending -= BYTES_PER_BLOCK;
}

// Query accelerometer data
static void
adxl355_query(struct adxl355 *ax, uint8_t oid)
{
    if (ax->fifo_bytes_pending < BYTES_PER_BLOCK && query_fifo_status(ax))
        ax->fifo_bytes_pending = 0;

    if (ax->fifo_bytes_pending >= BYTES_PER_BLOCK)
        read_fifo_block(ax, oid);

    // check if we need to run the task again (more packets in fifo?)
    if (ax->fifo_bytes_pending >= BYTES_PER_BLOCK) {
        // More data in fifo - wake this task again
        sched_wake_task(&adxl355_wake);
    } else {
        // Sleep until next check time
        ax->flags &= ~AX_PENDING;
        adxl355_reschedule_timer(ax);
    }
}

void
command_query_adxl355(uint32_t *args)
{
    struct adxl355 *ax = oid_lookup(args[0], command_config_adxl355);

    sched_del_timer(&ax->timer);
    ax->flags = 0;
    if (!args[1])
        // End measurements
        return;

    // Start new measurements query
    ax->rest_ticks = args[1];
    ax->fifo_bytes_pending = 0;
    sensor_bulk_reset(&ax->sb);
    adxl355_reschedule_timer(ax);
}
DECL_COMMAND(command_query_adxl355, "query_adxl355 oid=%c rest_ticks=%u");

void
command_query_adxl355_status(uint32_t *args)
{
    struct adxl355 *ax = oid_lookup(args[0], command_config_adxl355);
    uint8_t msg[3] = { AM_READ(AR_STATUS), 0x00, 0x00 };

    uint32_t time1 = timer_read_time();
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    uint32_t time2 = timer_read_time();

    if (update_fifo_status(ax, msg[1], msg[2]))
        // Query error - don't send response - host will retry
        return;
    sensor_bulk_status(&ax->sb, args[0], time1, time2-time1
                       , ax->fifo_bytes_pending);
}
DECL_COMMAND(command_query_adxl355_status, "query_adxl355_status oid=%c");

void
adxl355_task(void)
{
    if (!sched_check_wake(&adxl355_wake))
        return;
    uint8_t oid;
    struct adxl355 *ax;
    foreach_oid(oid, ax, command_config_adxl355) {
        uint_fast8_t flags = ax->flags;
        if (flags & AX_PENDING)
            adxl355_query(ax, oid);
    }
}
DECL_TASK(adxl355_task);
