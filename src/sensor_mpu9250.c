// Support for gathering acceleration data from mpu9250 chip
//
// Copyright (C) 2023 Matthew Swabey <matthew@swabey.org>
// Copyright (C) 2022 Harry Beyel <harry3b9@gmail.com>
// Copyright (C) 2020-2021 Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "board/gpio.h" // i2c_read
#include "i2ccmds.h" // i2cdev_oid_lookup

// Chip registers
#define AR_FIFO_SIZE 512

#define AR_PWR_MGMT_1   0x6B
#define AR_PWR_MGMT_2   0x6C
#define AR_FIFO_EN      0x23
#define AR_ACCEL_OUT_XH 0x3B
#define AR_USER_CTRL    0x6A
#define AR_FIFO_COUNT_H 0x72
#define AR_FIFO         0x74
#define AR_INT_STATUS   0x3A

#define SET_ENABLE_FIFO 0x08
#define SET_DISABLE_FIFO 0x00
#define SET_USER_FIFO_RESET 0x04
#define SET_USER_FIFO_EN 0x40

#define SET_PWR_SLEEP   0x40
#define SET_PWR_WAKE    0x00
#define SET_PWR_2_ACCEL 0x07 // only enable accelerometers
#define SET_PWR_2_NONE  0x3F // disable all sensors

#define FIFO_OVERFLOW_INT 0x10

#define BYTES_PER_FIFO_ENTRY 6

struct mpu9250 {
    struct timer timer;
    uint32_t rest_ticks;
    struct i2cdev_s *i2c;
    uint16_t sequence, limit_count, fifo_max, fifo_pkts_bytes;
    uint8_t flags, data_count;
    // msg size must be <= 255 due to Klipper api
    // = SAMPLES_PER_BLOCK (from mpu9250.py) * BYTES_PER_FIFO_ENTRY + 1
    uint8_t data[48];
};

enum {
    AX_HAVE_START = 1<<0, AX_RUNNING = 1<<1, AX_PENDING = 1<<2,
};

static struct task_wake mpu9250_wake;

// Reads the fifo byte count from the device.
static uint16_t
get_fifo_status (struct mpu9250 *mp)
{
    uint8_t reg[] = {AR_FIFO_COUNT_H};
    uint8_t msg[2];
    i2c_read(mp->i2c->i2c_config, sizeof(reg), reg, sizeof(msg), msg);
    msg[0] = 0x1F & msg[0]; // discard 3 MSB per datasheet
    uint16_t bytes_to_read = ((uint16_t)msg[0]) << 8 | msg[1];
    if (bytes_to_read > mp->fifo_max) mp->fifo_max = bytes_to_read;
    return bytes_to_read;
}

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

// Report local measurement buffer
static void
mp9250_report(struct mpu9250 *mp, uint8_t oid)
{
    sendf("mpu9250_data oid=%c sequence=%hu data=%*s"
          , oid, mp->sequence, mp->data_count, mp->data);
    mp->data_count = 0;
    mp->sequence++;
}

// Report buffer and fifo status
static void
mp9250_status(struct mpu9250 *mp, uint_fast8_t oid
            , uint32_t time1, uint32_t time2, uint16_t fifo)
{
    sendf("mpu9250_status oid=%c clock=%u query_ticks=%u next_sequence=%hu"
          " buffered=%c fifo=%u limit_count=%hu"
          , oid, time1, time2-time1, mp->sequence
          , mp->data_count, fifo, mp->limit_count);
}

// Helper code to reschedule the mpu9250_event() timer
static void
mp9250_reschedule_timer(struct mpu9250 *mp)
{
    irq_disable();
    mp->timer.waketime = timer_read_time() + mp->rest_ticks;
    sched_add_timer(&mp->timer);
    irq_enable();
}

// Query accelerometer data
static void
mp9250_query(struct mpu9250 *mp, uint8_t oid)
{
    // Find remaining space in report buffer
    uint8_t data_space = sizeof(mp->data) - mp->data_count;

    // If not enough bytes to fill report read MPU FIFO's fill
    if (mp->fifo_pkts_bytes < data_space) {
        mp->fifo_pkts_bytes = get_fifo_status(mp) / BYTES_PER_FIFO_ENTRY
                                * BYTES_PER_FIFO_ENTRY;
    }

    // If we have enough bytes to fill the buffer do it and send report
    if (mp->fifo_pkts_bytes >= data_space) {
        uint8_t reg = AR_FIFO;
        i2c_read(mp->i2c->i2c_config, sizeof(reg), &reg,
                 data_space, &mp->data[mp->data_count]);
        mp->data_count += data_space;
        mp->fifo_pkts_bytes -= data_space;
        mp9250_report(mp, oid);
    }

    // If we have enough bytes remaining to fill another report wake again
    //  otherwise schedule timed wakeup
    if (mp->fifo_pkts_bytes > data_space) {
        sched_wake_task(&mpu9250_wake);
    } else if (mp->flags & AX_RUNNING) {
        sched_del_timer(&mp->timer);
        mp->flags &= ~AX_PENDING;
        mp9250_reschedule_timer(mp);
    }
}

// Startup measurements
static void
mp9250_start(struct mpu9250 *mp, uint8_t oid)
{
    sched_del_timer(&mp->timer);
    mp->flags = AX_RUNNING;
    uint8_t msg[2];

    msg[0] = AR_FIFO_EN;
    msg[1] = SET_DISABLE_FIFO; // disable FIFO
    i2c_write(mp->i2c->i2c_config, sizeof(msg), msg);

    msg[0] = AR_USER_CTRL;
    msg[1] = SET_USER_FIFO_RESET; // reset FIFO buffer
    i2c_write(mp->i2c->i2c_config, sizeof(msg), msg);

    msg[0] = AR_USER_CTRL;
    msg[1] = SET_USER_FIFO_EN; // enable FIFO buffer access
    i2c_write(mp->i2c->i2c_config, sizeof(msg), msg);

    uint8_t int_reg[] = {AR_INT_STATUS}; // clear FIFO overflow flag
    i2c_read(mp->i2c->i2c_config, sizeof(int_reg), int_reg, 1, msg);

    msg[0] = AR_FIFO_EN;
    msg[1] = SET_ENABLE_FIFO; // enable accel output to FIFO
    i2c_write(mp->i2c->i2c_config, sizeof(msg), msg);

    mp9250_reschedule_timer(mp);
}

// End measurements
static void
mp9250_stop(struct mpu9250 *mp, uint8_t oid)
{
    // Disable measurements
    sched_del_timer(&mp->timer);
    mp->flags = 0;

    // disable accel FIFO
    uint8_t msg[2] = { AR_FIFO_EN, SET_DISABLE_FIFO };
    uint32_t end1_time = timer_read_time();
    i2c_write(mp->i2c->i2c_config, sizeof(msg), msg);
    uint32_t end2_time = timer_read_time();

    // Detect if a FIFO overrun occured
    uint8_t int_reg[] = {AR_INT_STATUS};
    uint8_t int_msg;
    i2c_read(mp->i2c->i2c_config, sizeof(int_reg), int_reg, sizeof(int_msg),
                &int_msg);
    if (int_msg & FIFO_OVERFLOW_INT)
        mp->limit_count++;

    // Report final data
    if (mp->data_count > 0)
        mp9250_report(mp, oid);
    uint16_t bytes_to_read = get_fifo_status(mp);
    mp9250_status(mp, oid, end1_time, end2_time,
                    bytes_to_read / BYTES_PER_FIFO_ENTRY);

    // Uncomment and rebuilt to check for FIFO overruns when tuning
    //output("mpu9240 limit_count=%u fifo_max=%u",
    //       mp->limit_count, mp->fifo_max);
}

void
command_query_mpu9250(uint32_t *args)
{
    struct mpu9250 *mp = oid_lookup(args[0], command_config_mpu9250);

    if (!args[2]) {
        // End measurements
        mp9250_stop(mp, args[0]);
        return;
    }
    // Start new measurements query
    sched_del_timer(&mp->timer);
    mp->timer.waketime = args[1];
    mp->rest_ticks = args[2];
    mp->flags = AX_HAVE_START;
    mp->sequence = 0;
    mp->limit_count = 0;
    mp->data_count = 0;
    mp->fifo_max = 0;
    mp->fifo_pkts_bytes = 0;
    sched_add_timer(&mp->timer);
}
DECL_COMMAND(command_query_mpu9250,
             "query_mpu9250 oid=%c clock=%u rest_ticks=%u");

void
command_query_mpu9250_status(uint32_t *args)
{
    struct mpu9250 *mp = oid_lookup(args[0], command_config_mpu9250);
    uint8_t msg[2];
    uint32_t time1 = timer_read_time();
    uint8_t reg[] = {AR_FIFO_COUNT_H};
    i2c_read(mp->i2c->i2c_config, sizeof(reg), reg, sizeof(msg), msg);
    uint32_t time2 = timer_read_time();
    msg[0] = 0x1F & msg[0]; // discard 3 MSB
    mp9250_status(mp, args[0], time1, time2, mp->fifo_pkts_bytes
                  / BYTES_PER_FIFO_ENTRY);
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
        if (!(flags & AX_PENDING)) {
            continue;
        }
        if (flags & AX_HAVE_START) {
            mp9250_start(mp, oid);
        }
        else {
            mp9250_query(mp, oid);
        }
    }
}
DECL_TASK(mpu9250_task);
