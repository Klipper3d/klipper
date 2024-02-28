// Support for gathering acceleration data from ADXL345 chip
//
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

struct adxl345 {
    struct timer timer;
    uint32_t rest_ticks;
    struct spidev_s *spi;
    uint8_t flags;
    struct sensor_bulk sb;
};

enum {
    AX_PENDING = 1<<0,
};

static struct task_wake adxl345_wake;

// Event handler that wakes adxl345_task() periodically
static uint_fast8_t
adxl345_event(struct timer *timer)
{
    struct adxl345 *ax = container_of(timer, struct adxl345, timer);
    ax->flags |= AX_PENDING;
    sched_wake_task(&adxl345_wake);
    return SF_DONE;
}

void
command_config_adxl345(uint32_t *args)
{
    struct adxl345 *ax = oid_alloc(args[0], command_config_adxl345
                                   , sizeof(*ax));
    ax->timer.func = adxl345_event;
    ax->spi = spidev_oid_lookup(args[1]);
}
DECL_COMMAND(command_config_adxl345, "config_adxl345 oid=%c spi_oid=%c");

// Helper code to reschedule the adxl345_event() timer
static void
adxl_reschedule_timer(struct adxl345 *ax)
{
    irq_disable();
    ax->timer.waketime = timer_read_time() + ax->rest_ticks;
    sched_add_timer(&ax->timer);
    irq_enable();
}

// Chip registers
#define AR_DATAX0      0x32
#define AR_FIFO_STATUS 0x39
#define AM_READ  0x80
#define AM_MULTI 0x40

#define SET_FIFO_CTL 0x90

#define BYTES_PER_SAMPLE 5

// Query accelerometer data
static void
adxl_query(struct adxl345 *ax, uint8_t oid)
{
    // Read data
    uint8_t msg[9] = { AR_DATAX0 | AM_READ | AM_MULTI, 0, 0, 0, 0, 0, 0, 0, 0 };
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    // Extract x, y, z measurements
    uint_fast8_t fifo_status = msg[8] & ~0x80; // Ignore trigger bit
    uint8_t *d = &ax->sb.data[ax->sb.data_count];
    if (((msg[2] & 0xf0) && (msg[2] & 0xf0) != 0xf0)
        || ((msg[4] & 0xf0) && (msg[4] & 0xf0) != 0xf0)
        || ((msg[6] & 0xf0) && (msg[6] & 0xf0) != 0xf0)
        || (msg[7] != SET_FIFO_CTL) || (fifo_status > 32)) {
        // Data error - may be a CS, MISO, MOSI, or SCLK glitch
        d[0] = d[1] = d[2] = d[3] = d[4] = 0xff;
        fifo_status = 0;
    } else {
        // Copy data
        d[0] = msg[1]; // x low bits
        d[1] = msg[3]; // y low bits
        d[2] = msg[5]; // z low bits
        d[3] = (msg[2] & 0x1f) | (msg[6] << 5); // x high bits and z high bits
        d[4] = (msg[4] & 0x1f) | ((msg[6] << 2) & 0x60); // y high and z high
    }
    ax->sb.data_count += BYTES_PER_SAMPLE;
    if (ax->sb.data_count + BYTES_PER_SAMPLE > ARRAY_SIZE(ax->sb.data))
        sensor_bulk_report(&ax->sb, oid);
    // Check fifo status
    if (fifo_status >= 31)
        ax->sb.possible_overflows++;
    if (fifo_status > 1) {
        // More data in fifo - wake this task again
        sched_wake_task(&adxl345_wake);
    } else {
        // Sleep until next check time
        ax->flags &= ~AX_PENDING;
        adxl_reschedule_timer(ax);
    }
}

void
command_query_adxl345(uint32_t *args)
{
    struct adxl345 *ax = oid_lookup(args[0], command_config_adxl345);

    sched_del_timer(&ax->timer);
    ax->flags = 0;
    if (!args[1])
        // End measurements
        return;

    // Start new measurements query
    ax->rest_ticks = args[1];
    sensor_bulk_reset(&ax->sb);
    adxl_reschedule_timer(ax);
}
DECL_COMMAND(command_query_adxl345, "query_adxl345 oid=%c rest_ticks=%u");

void
command_query_adxl345_status(uint32_t *args)
{
    struct adxl345 *ax = oid_lookup(args[0], command_config_adxl345);
    uint8_t msg[2] = { AR_FIFO_STATUS | AM_READ, 0x00 };

    uint32_t time1 = timer_read_time();
    spidev_transfer(ax->spi, 1, sizeof(msg), msg);
    uint32_t time2 = timer_read_time();

    uint_fast8_t fifo_status = msg[1] & ~0x80; // Ignore trigger bit
    if (fifo_status > 32)
        // Query error - don't send response - host will retry
        return;
    sensor_bulk_status(&ax->sb, args[0], time1, time2-time1
                       , fifo_status * BYTES_PER_SAMPLE);
}
DECL_COMMAND(command_query_adxl345_status, "query_adxl345_status oid=%c");

void
adxl345_task(void)
{
    if (!sched_check_wake(&adxl345_wake))
        return;
    uint8_t oid;
    struct adxl345 *ax;
    foreach_oid(oid, ax, command_config_adxl345) {
        uint_fast8_t flags = ax->flags;
        if (flags & AX_PENDING)
            adxl_query(ax, oid);
    }
}
DECL_TASK(adxl345_task);
