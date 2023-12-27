// Support for gathering acceleration data from mpu9250 chip
//
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
#include "board/gpio.h" // i2c_read
#include "i2ccmds.h" // i2cdev_oid_lookup

// Chip registers
#define AR_FIFO_COUNT_H 0x72
#define AR_FIFO         0x74
#define AR_INT_STATUS   0x3A

#define FIFO_OVERFLOW_INT 0x10

#define BYTES_PER_FIFO_ENTRY 6
#define BYTES_PER_BLOCK 48

struct mpu9250 {
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

static struct task_wake mpu9250_wake;

// Event handler that wakes mpu9250_task() periodically
static uint_fast8_t
mpu9250_event(struct timer *timer)
{
    struct mpu9250 *ax = container_of(timer, struct mpu9250, timer);
    ax->flags |= AX_PENDING;
    sched_wake_task(&mpu9250_wake);
    return SF_DONE;
}

void
command_config_mpu9250(uint32_t *args)
{
    struct mpu9250 *mp = oid_alloc(args[0], command_config_mpu9250
                                   , sizeof(*mp));
    mp->timer.func = mpu9250_event;
    mp->i2c = i2cdev_oid_lookup(args[1]);
}
DECL_COMMAND(command_config_mpu9250, "config_mpu9250 oid=%c i2c_oid=%c");

// Helper code to reschedule the mpu9250_event() timer
static void
mp9250_reschedule_timer(struct mpu9250 *mp)
{
    irq_disable();
    mp->timer.waketime = timer_read_time() + mp->rest_ticks;
    sched_add_timer(&mp->timer);
    irq_enable();
}

// Reads the fifo byte count from the device.
static uint16_t
get_fifo_status(struct mpu9250 *mp)
{
    uint8_t reg[] = {AR_FIFO_COUNT_H};
    uint8_t msg[2];
    i2c_read(mp->i2c->i2c_config, sizeof(reg), reg, sizeof(msg), msg);
    uint16_t fifo_bytes = ((msg[0] & 0x1f) << 8) | msg[1];
    if (fifo_bytes > mp->fifo_max)
        mp->fifo_max = fifo_bytes;
    return fifo_bytes;
}

// Query accelerometer data
static void
mp9250_query(struct mpu9250 *mp, uint8_t oid)
{
    // If not enough bytes to fill report read MPU FIFO's fill
    if (mp->fifo_pkts_bytes < BYTES_PER_BLOCK)
        mp->fifo_pkts_bytes = get_fifo_status(mp);

    // If we have enough bytes to fill the buffer do it and send report
    if (mp->fifo_pkts_bytes >= BYTES_PER_BLOCK) {
        uint8_t reg = AR_FIFO;
        i2c_read(mp->i2c->i2c_config, sizeof(reg), &reg
                 , BYTES_PER_BLOCK, &mp->sb.data[0]);
        mp->sb.data_count = BYTES_PER_BLOCK;
        mp->fifo_pkts_bytes -= BYTES_PER_BLOCK;
        sensor_bulk_report(&mp->sb, oid);
    }

    // If we have enough bytes remaining to fill another report wake again
    //  otherwise schedule timed wakeup
    if (mp->fifo_pkts_bytes >= BYTES_PER_BLOCK) {
        sched_wake_task(&mpu9250_wake);
    } else {
        mp->flags &= ~AX_PENDING;
        mp9250_reschedule_timer(mp);
    }
}

void
command_query_mpu9250(uint32_t *args)
{
    struct mpu9250 *mp = oid_lookup(args[0], command_config_mpu9250);

    sched_del_timer(&mp->timer);
    mp->flags = 0;
    if (!args[1]) {
        // End measurements

        // Uncomment and rebuilt to check for FIFO overruns when tuning
        //output("mpu9240 fifo_max=%u", mp->fifo_max);
        return;
    }

    // Start new measurements query
    mp->rest_ticks = args[1];
    sensor_bulk_reset(&mp->sb);
    mp->fifo_max = 0;
    mp->fifo_pkts_bytes = 0;
    mp9250_reschedule_timer(mp);
}
DECL_COMMAND(command_query_mpu9250, "query_mpu9250 oid=%c rest_ticks=%u");

void
command_query_mpu9250_status(uint32_t *args)
{
    struct mpu9250 *mp = oid_lookup(args[0], command_config_mpu9250);

    // Detect if a FIFO overrun occurred
    uint8_t int_reg[] = {AR_INT_STATUS};
    uint8_t int_msg;
    i2c_read(mp->i2c->i2c_config, sizeof(int_reg), int_reg, sizeof(int_msg),
             &int_msg);
    if (int_msg & FIFO_OVERFLOW_INT)
        mp->sb.possible_overflows++;

    // Read latest FIFO count (with precise timing)
    uint8_t reg[] = {AR_FIFO_COUNT_H};
    uint8_t msg[2];
    uint32_t time1 = timer_read_time();
    i2c_read(mp->i2c->i2c_config, sizeof(reg), reg, sizeof(msg), msg);
    uint32_t time2 = timer_read_time();
    uint16_t fifo_bytes = ((msg[0] & 0x1f) << 8) | msg[1];

    // Report status
    sensor_bulk_status(&mp->sb, args[0], time1, time2-time1, fifo_bytes);
}
DECL_COMMAND(command_query_mpu9250_status, "query_mpu9250_status oid=%c");

void
mpu9250_task(void)
{
    if (!sched_check_wake(&mpu9250_wake))
        return;
    uint8_t oid;
    struct mpu9250 *mp;
    foreach_oid(oid, mp, command_config_mpu9250) {
        uint_fast8_t flags = mp->flags;
        if (flags & AX_PENDING)
            mp9250_query(mp, oid);
    }
}
DECL_TASK(mpu9250_task);
