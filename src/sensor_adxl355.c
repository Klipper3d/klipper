// Support for gathering acceleration data from ADXL355 chip
//
// Copyright (C) 2026  Sven
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

struct adxl355 {
    struct timer timer;
    uint32_t rest_ticks;
    struct spidev_s *spi;
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
#define AR_XDATA3 0x08

#define AM_READ(reg) ((((reg) & 0x7f) << 1) | 0x01)

#define BYTES_PER_SAMPLE 9

// Query accelerometer data
static void
adxl355_query(struct adxl355 *ax, uint8_t oid)
{
    // Read x, y, z measurements from XDATA3..ZDATA1
    uint8_t msg[10] = { AM_READ(AR_XDATA3), 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);

    uint8_t *d = &ax->sb.data[ax->sb.data_count];
    // Each axis is 20-bit left-justified in 3 bytes, so low nibble should be 0.
    if ((msg[3] & 0x0f) || (msg[6] & 0x0f) || (msg[9] & 0x0f)) {
        // Data error - may be a CS, MISO, MOSI, or SCLK glitch
        d[0] = d[1] = d[2] = d[3] = d[4] = d[5] = d[6] = d[7] = d[8] = 0xff;
    } else {
        memcpy(d, &msg[1], BYTES_PER_SAMPLE);
    }

    ax->sb.data_count += BYTES_PER_SAMPLE;
    if (ax->sb.data_count + BYTES_PER_SAMPLE > ARRAY_SIZE(ax->sb.data))
        sensor_bulk_report(&ax->sb, oid);

    // Sleep until next check time
    ax->flags &= ~AX_PENDING;
    adxl355_reschedule_timer(ax);
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
    sensor_bulk_reset(&ax->sb);
    adxl355_reschedule_timer(ax);
}
DECL_COMMAND(command_query_adxl355, "query_adxl355 oid=%c rest_ticks=%u");

void
command_query_adxl355_status(uint32_t *args)
{
    struct adxl355 *ax = oid_lookup(args[0], command_config_adxl355);
    uint8_t msg[2] = { AM_READ(AR_STATUS), 0x00 };

    uint32_t time1 = timer_read_time();
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    uint32_t time2 = timer_read_time();

    // STATUS[2] indicates FIFO overrun.
    if (msg[1] & (1<<2))
        ax->sb.possible_overflows++;
    sensor_bulk_status(&ax->sb, args[0], time1, time2-time1, 0);
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
