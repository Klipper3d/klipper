// Support for gathering acceleration data from mpu9250 chip
//
// Copyright (C) 2020  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "generic/irq.h" // irq_disable
#include "generic/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK
#include "i2ccmds.c" // i2c_read

struct mpu9250 {
    struct timer timer;
    uint32_t rest_ticks;
    struct i2cdev_s *i2c;
    uint16_t sequence, limit_count;
    uint8_t flags, data_count;
    uint8_t data[48];
};

enum {
    AX_HAVE_START = 1<<0, AX_RUNNING = 1<<1, AX_PENDING = 1<<2,
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
    mp->i2c = oid_lookup(args[1], command_config_i2c);
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

// Chip registers
#define AR_FIFO_SIZE 512

#define AR_PWR_MGMT_1  0x6B
#define AR_FIFO_EN     0x23
#define AR_ACCEL_OUT_XH 0x3B
#define AR_FIFO_COUNT_H  0x72

#define SET_ENABLE_FIFO 0x80
#define SET_DISABLE_FIFO 0x00

#define SET_PWR_SLEEP 0x80
#define SET_PWR_WAKE  0x00

// Query accelerometer data
static void
mp9250_query(struct mpu9250 *mp, uint8_t oid)
{
    // Read data
    // Regs are: [Xh, Xl, Yh, Yl, Zh, Zl] and [FIFO_CNTh, FIFO_CNTl]
    uint8_t regs[] = {AR_ACCEL_OUT_XH, AR_FIFO_COUNT_H};
    uint8_t data_lens[] = {6, 2};
    uint8_t fifo_count[2];

    // Extract x, y, z measurements into data holder
    uint8_t *d = &mp->data[mp->data_count];
    i2c_read(mp->i2c->i2c_config, 1, &regs[0], data_lens[0], d);
    i2c_read(mp->i2c->i2c_config, 1, &regs[1], data_lens[1], fifo_count);

    // Get FIFO size
    fifo_count[0] = 0x1F & fifo_count[0]; // discard 3 MSB of fifo size
    uint16_t fifo_status = (((uint16_t)fifo_count[0]) << 8) | fifo_count[1];
    
    mp->data_count += 6;
    if (mp->data_count + 6 > ARRAY_SIZE(mp->data))
        mp9250_report(mp, oid);

    // Check fifo status
    if (fifo_status >= AR_FIFO_SIZE-1)
        mp->limit_count++;
    if (fifo_status > 1 && fifo_status <= AR_FIFO_SIZE) {
        // More data in fifo - wake this task again
        sched_wake_task(&mpu9250_wake);
    } else if (mp->flags & AX_RUNNING) {
        // Sleep until next check time
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
    uint8_t msg[2] = { AR_PWR_MGMT_1, 0x00 }; // wake up
    i2c_write(mp->i2c->i2c_config, sizeof(msg), msg);
    uint8_t msg2[2] = { AR_FIFO_EN, SET_ENABLE_FIFO }; // enable accel FIFO
    i2c_write(mp->i2c->i2c_config, sizeof(msg2), msg2);
    mp9250_reschedule_timer(mp);
}

// End measurements
static void
mp9250_stop(struct mpu9250 *mp, uint8_t oid)
{
    // Disable measurements
    sched_del_timer(&mp->timer);
    mp->flags = 0;

    uint8_t msg[2] = { AR_FIFO_EN, SET_DISABLE_FIFO }; // disable accel FIFO
    uint32_t end1_time = timer_read_time();
    i2c_write(mp->i2c->i2c_config, sizeof(msg), msg);
    uint32_t end2_time = timer_read_time();

    // uint8_t msg[2] = { AR_PWR_MGMT_1, SET_PWR_SLEEP }; // set to sleep
    // i2c_write(mp->i2c->i2c_config, sizeof(msg), msg);

    // Drain any measurements still in fifo
    uint16_t fifo_status;
    uint_fast8_t i;
    uint8_t regs[] = {AR_FIFO_COUNT_H};
    for (i=0; i<33; i++) {
        i2c_read(mp->i2c->i2c_config, 1, regs, 2, msg);
        msg[0] = 0x1F & msg[0]; // discard 3 MSB
        fifo_status = (((uint16_t)msg[0]) << 8) | msg[1];

        if (!fifo_status)
            break;
        if (fifo_status <= AR_FIFO_SIZE)
            mp9250_query(mp, oid);
    }

    // Report final data
    if (mp->data_count)
        mp9250_report(mp, oid);
    mp9250_status(mp, oid, end1_time, end2_time, fifo_status);
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
    mp->sequence = mp->limit_count = 0;
    mp->data_count = 0;
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
    uint8_t regs[] = {AR_FIFO_COUNT_H};
    i2c_read(mp->i2c->i2c_config, 1, regs, 2, msg);
    uint32_t time2 = timer_read_time();
    msg[0] = 0x1F & msg[0]; // discard 3 MSB
    uint16_t fifo_status = (((uint16_t)msg[0]) << 8) | msg[1];
    mp9250_status(mp, args[0], time1, time2, fifo_status);
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
        if (!(flags & AX_PENDING))
            continue;
        if (flags & AX_HAVE_START)
            mp9250_start(mp, oid);
        else
            mp9250_query(mp, oid);
    }
}
DECL_TASK(mpu9250_task);
