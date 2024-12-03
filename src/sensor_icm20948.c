// Support for gathering acceleration data from icm20948 chip
//
// Copyright (C) 2024 Paul Hansel <github@paulhansel.com>
// Copyright (C) 2023 Matthew Swabey <matthew@swabey.org>
// Copyright (C) 2022 Harry Beyel <harry3b9@gmail.com>
// Copyright (C) 2020-2023 Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "sensor_bulk.h" // sensor_bulk_report
#include "i2ccmds.h" // i2cdev_oid_lookup

// Chip registers
#define AR_FIFO_COUNT_H 0x70
#define AR_FIFO         0x72
#define AR_INT_STATUS   0x1B // INT_STATUS_2

#define FIFO_OVERFLOW_INT 0x0F // from datasheet

#define BYTES_PER_FIFO_ENTRY 6
#define BYTES_PER_BLOCK 48

struct icm20948 {
    struct timer timer;
    uint32_t rest_ticks;
    struct i2cdev_s *i2c;
    uint16_t fifo_max, fifo_pkts_bytes;
    uint8_t flags;
    struct sensor_bulk sb;
};

enum {
    AX_PENDING = 1<<0,
};

static struct task_wake icm20948_wake;

// Event handler that wakes icm20948_task() periodically
static uint_fast8_t
icm20948_event(struct timer *timer)
{
    struct icm20948 *ax = container_of(timer, struct icm20948, timer);
    ax->flags |= AX_PENDING;
    sched_wake_task(&icm20948_wake);
    return SF_DONE;
}

void
command_config_icm20948(uint32_t *args)
{
    struct icm20948 *ic = oid_alloc(args[0], command_config_icm20948
                                   , sizeof(*ic));
    ic->timer.func = icm20948_event;
    ic->i2c = i2cdev_oid_lookup(args[1]);
}
DECL_COMMAND(command_config_icm20948, "config_icm20948 oid=%c i2c_oid=%c");

// Helper code to reschedule the icm20948_event() timer
static void
ic20948_reschedule_timer(struct icm20948 *ic)
{
    irq_disable();
    ic->timer.waketime = timer_read_time() + ic->rest_ticks;
    sched_add_timer(&ic->timer);
    irq_enable();
}

static void
read_mpu(struct i2cdev_s *i2c, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
    int ret = i2c_dev_read(i2c, reg_len, reg, read_len, read);
    i2c_shutdown_on_err(ret);
}

// Reads the fifo byte count from the device.
static uint16_t
get_fifo_status(struct icm20948 *ic)
{
    uint8_t reg[] = {AR_FIFO_COUNT_H};
    uint8_t msg[2];
    read_mpu(ic->i2c, sizeof(reg), reg, sizeof(msg), msg);
    uint16_t fifo_bytes = ((msg[0] & 0x1f) << 8) | msg[1];
    if (fifo_bytes > ic->fifo_max)
        ic->fifo_max = fifo_bytes;
    return fifo_bytes;
}

// Query accelerometer data
static void
ic20948_query(struct icm20948 *ic, uint8_t oid)
{
    // If not enough bytes to fill report read MPU FIFO's fill
    if (ic->fifo_pkts_bytes < BYTES_PER_BLOCK)
        ic->fifo_pkts_bytes = get_fifo_status(ic);

    // If we have enough bytes to fill the buffer do it and send report
    if (ic->fifo_pkts_bytes >= BYTES_PER_BLOCK) {
        uint8_t reg = AR_FIFO;
        read_mpu(ic->i2c, sizeof(reg), &reg, BYTES_PER_BLOCK, &ic->sb.data[0]);
        ic->sb.data_count = BYTES_PER_BLOCK;
        ic->fifo_pkts_bytes -= BYTES_PER_BLOCK;
        sensor_bulk_report(&ic->sb, oid);
    }

    // If we have enough bytes remaining to fill another report wake again
    //  otherwise schedule timed wakeup
    if (ic->fifo_pkts_bytes >= BYTES_PER_BLOCK) {
        sched_wake_task(&icm20948_wake);
    } else {
        ic->flags &= ~AX_PENDING;
        ic20948_reschedule_timer(ic);
    }
}

void
command_query_icm20948(uint32_t *args)
{
    struct icm20948 *ic = oid_lookup(args[0], command_config_icm20948);

    sched_del_timer(&ic->timer);
    ic->flags = 0;
    if (!args[1]) {
        // End measurements

        // Uncomment and rebuilt to check for FIFO overruns when tuning
        //output("mpu9240 fifo_max=%u", ic->fifo_max);
        return;
    }

    // Start new measurements query
    ic->rest_ticks = args[1];
    sensor_bulk_reset(&ic->sb);
    ic->fifo_max = 0;
    ic->fifo_pkts_bytes = 0;
    ic20948_reschedule_timer(ic);
}
DECL_COMMAND(command_query_icm20948, "query_icm20948 oid=%c rest_ticks=%u");

void
command_query_icm20948_status(uint32_t *args)
{
    struct icm20948 *ic = oid_lookup(args[0], command_config_icm20948);

    // Detect if a FIFO overrun occurred
    uint8_t int_reg[] = {AR_INT_STATUS};
    uint8_t int_msg;
    read_mpu(ic->i2c, sizeof(int_reg), int_reg, sizeof(int_msg), &int_msg);
    if (int_msg & FIFO_OVERFLOW_INT)
        ic->sb.possible_overflows++;

    // Read latest FIFO count (with precise timing)
    uint8_t reg[] = {AR_FIFO_COUNT_H};
    uint8_t msg[2];
    uint32_t time1 = timer_read_time();
    read_mpu(ic->i2c, sizeof(reg), reg, sizeof(msg), msg);
    uint32_t time2 = timer_read_time();
    uint16_t fifo_bytes = ((msg[0] & 0x1f) << 8) | msg[1];

    // Report status
    sensor_bulk_status(&ic->sb, args[0], time1, time2-time1, fifo_bytes);
}
DECL_COMMAND(command_query_icm20948_status, "query_icm20948_status oid=%c");

void
icm20948_task(void)
{
    if (!sched_check_wake(&icm20948_wake))
        return;
    uint8_t oid;
    struct icm20948 *ic;
    foreach_oid(oid, ic, command_config_icm20948) {
        uint_fast8_t flags = ic->flags;
        if (flags & AX_PENDING)
            ic20948_query(ic, oid);
    }
}
DECL_TASK(icm20948_task);
