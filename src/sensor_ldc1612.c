// Support for eddy current sensor data from ldc1612 chip
//
// Copyright (C) 2023 Alan.Ma <tech@biqu3d.com>
// Copyright (C) 2024  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "basecmd.h" // oid_alloc
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "i2ccmds.h" // i2cdev_oid_lookup
#include "sched.h" // DECL_TASK
#include "sensor_bulk.h" // sensor_bulk_report
#include "trsync.h" // trsync_do_trigger
#include "sos_filter.h" // sos_filter_oid_lookup

enum {
    LDC_PENDING = 1<<0, LDC_HAVE_INTB = 1<<1,
    LH_AWAIT_HOMING = 1<<1, LH_CAN_TRIGGER = 1<<2,
    LT_AWAIT_HOMING = 1<<3, LT_CAN_TRIGGER = 1<<4,
};

struct ldc1612 {
    struct timer timer;
    uint32_t rest_ticks;
    struct i2cdev_s *i2c;
    uint8_t flags;
    struct sensor_bulk sb;
    struct gpio_in intb_pin;
    // homing
    struct trsync *ts;
    uint8_t homing_flags;
    uint8_t trigger_reason, error_reason;
    int32_t trigger_threshold;
    uint32_t homing_clock;
    // tap
    struct sos_filter *sf;
    int32_t prev_sample;
    int32_t peak_instant_delta;
    uint32_t trigger_start;
};

static struct task_wake ldc1612_wake;

// Check if the intb line is "asserted"
static int
check_intb_asserted(struct ldc1612 *ld)
{
    return !gpio_in_read(ld->intb_pin);
}

// Query ldc1612 data
static uint_fast8_t
ldc1612_event(struct timer *timer)
{
    struct ldc1612 *ld = container_of(timer, struct ldc1612, timer);
    if (ld->flags & LDC_PENDING)
        ld->sb.possible_overflows++;
    if (!(ld->flags & LDC_HAVE_INTB) || check_intb_asserted(ld)) {
        ld->flags |= LDC_PENDING;
        sched_wake_task(&ldc1612_wake);
    }
    ld->timer.waketime += ld->rest_ticks;
    return SF_RESCHEDULE;
}

void
command_config_ldc1612(uint32_t *args)
{
    struct ldc1612 *ld = oid_alloc(args[0], command_config_ldc1612
                                   , sizeof(*ld));
    ld->timer.func = ldc1612_event;
    ld->i2c = i2cdev_oid_lookup(args[1]);
    ld->sf = sos_filter_oid_lookup(args[2]);
}
DECL_COMMAND(command_config_ldc1612,
             "config_ldc1612 oid=%c i2c_oid=%c sos_filter_oid=%c");

void
command_config_ldc1612_with_intb(uint32_t *args)
{
    command_config_ldc1612(args);
    struct ldc1612 *ld = oid_lookup(args[0], command_config_ldc1612);
    ld->intb_pin = gpio_in_setup(args[3], 1);
    ld->flags = LDC_HAVE_INTB;
}
DECL_COMMAND(command_config_ldc1612_with_intb,
             "config_ldc1612_with_intb oid=%c i2c_oid=%c sos_filter_oid=%c"
             " intb_pin=%c");

void
command_ldc1612_setup_home(uint32_t *args)
{
    struct ldc1612 *ld = oid_lookup(args[0], command_config_ldc1612);

    ld->trigger_threshold = args[2];
    if (!ld->trigger_threshold) {
        ld->ts = NULL;
        ld->homing_flags = 0;
        return;
    }
    ld->homing_clock = args[1];
    ld->ts = trsync_oid_lookup(args[3]);
    ld->trigger_reason = args[4];
    ld->error_reason = args[5];
    if (ld->trigger_threshold > 0)
        ld->homing_flags = LH_AWAIT_HOMING | LH_CAN_TRIGGER;
    else
        ld->homing_flags = LT_AWAIT_HOMING | LT_CAN_TRIGGER;
    ld->prev_sample = 0;
    ld->peak_instant_delta = 0;
}
DECL_COMMAND(command_ldc1612_setup_home,
             "ldc1612_setup_home oid=%c clock=%u threshold=%i"
             " trsync_oid=%c trigger_reason=%c error_reason=%c");

void
command_query_ldc1612_home_state(uint32_t *args)
{
    struct ldc1612 *ld = oid_lookup(args[0], command_config_ldc1612);
    uint8_t flags = ld->homing_flags;
    uint8_t in_progress = !!(flags & (LH_CAN_TRIGGER | LT_CAN_TRIGGER));
    sendf("ldc1612_home_state oid=%c homing=%c trigger_clock=%u"
          , args[0], in_progress, ld->homing_clock);
}
DECL_COMMAND(command_query_ldc1612_home_state,
             "query_ldc1612_home_state oid=%c");

// Check if a sample should trigger a tap event
static void
check_tap(struct ldc1612 *ld, uint32_t data)
{
    uint8_t flags = ld->homing_flags;
    if (!(flags & LT_CAN_TRIGGER))
        return;
    // Filter Amplitude error bit
    data &= ~(1 << 28);
    if (data > 0x0fffffff) {
        // Sensor reports an issue - cancel homing
        ld->homing_flags = 0;
        trsync_do_trigger(ld->ts, ld->error_reason);
        return;
    }
    // Raw data is noisy, it is filtered and stabilized
    int32_t idata = sosfilt(ld->sf, data);
    // Wait until homing sequence starts
    uint32_t time = timer_read_time();
    if ((flags & LT_AWAIT_HOMING) && timer_is_before(time, ld->homing_clock)) {
        ld->prev_sample = idata;
        return;
    }
    // Homing move started
    flags &= ~LT_AWAIT_HOMING;
    int32_t delta = (idata - ld->prev_sample);
    ld->prev_sample = idata;
    // As long as the conductive bed is moving towards the sensor
    // Frequency is stable or increasing
    // Delta is >= 0 and increasing
    // Backtrack delta peak
    if (delta > ld->peak_instant_delta) {
        ld->trigger_start = time;
        ld->peak_instant_delta = delta;
    }
    int32_t delta_to_peak = delta - ld->peak_instant_delta;
    // Upon tap delta would start to decrease
    // the delta after the peak is now negative
    if (delta_to_peak < ld->trigger_threshold) {
        flags = 0;
        ld->homing_clock = ld->trigger_start;
        trsync_do_trigger(ld->ts, ld->trigger_reason);
    }
    ld->homing_flags = flags;
}

// Check if a sample should trigger a homing event
static void
check_home(struct ldc1612 *ld, uint32_t data)
{
    uint8_t homing_flags = ld->homing_flags;
    if (!(homing_flags & LH_CAN_TRIGGER))
        return;
    if (data > 0x0fffffff) {
        // Sensor reports an issue - cancel homing
        ld->homing_flags = 0;
        trsync_do_trigger(ld->ts, ld->error_reason);
        return;
    }
    uint32_t time = timer_read_time();
    if ((homing_flags & LH_AWAIT_HOMING)
        && timer_is_before(time, ld->homing_clock))
        return;
    homing_flags &= ~LH_AWAIT_HOMING;
    if (data > ld->trigger_threshold) {
        homing_flags = 0;
        ld->homing_clock = time;
        trsync_do_trigger(ld->ts, ld->trigger_reason);
    }
    ld->homing_flags = homing_flags;
}

// Chip registers
#define REG_DATA0_MSB 0x00
#define REG_DATA0_LSB 0x01
#define REG_STATUS    0x18

// Read a register on the ldc1612
static void
read_reg(struct ldc1612 *ld, uint8_t reg, uint8_t *res)
{
    int ret = i2c_dev_read(ld->i2c, sizeof(reg), &reg, 2, res);
    i2c_shutdown_on_err(ret);
}

// Read the status register on the ldc1612
static uint16_t
read_reg_status(struct ldc1612 *ld)
{
    uint8_t data_status[2];
    read_reg(ld, REG_STATUS, data_status);
    return (data_status[0] << 8) | data_status[1];
}

#define BYTES_PER_SAMPLE 4

// Query ldc1612 data
static void
ldc1612_query(struct ldc1612 *ld, uint8_t oid)
{
    // Check if data available (and clear INTB line)
    uint16_t status = read_reg_status(ld);
    irq_disable();
    ld->flags &= ~LDC_PENDING;
    irq_enable();
    if (!(status & 0x08))
        return;

    // Read coil0 frequency
    uint8_t *d = &ld->sb.data[ld->sb.data_count];
    read_reg(ld, REG_DATA0_MSB, &d[0]);
    read_reg(ld, REG_DATA0_LSB, &d[2]);
    ld->sb.data_count += BYTES_PER_SAMPLE;

    // Check for endstop trigger
    uint32_t data =   ((uint32_t)d[0] << 24)
                    | ((uint32_t)d[1] << 16)
                    | ((uint32_t)d[2] << 8)
                    | ((uint32_t)d[3]);
    check_home(ld, data);
    check_tap(ld, data);

    // Flush local buffer if needed
    if (ld->sb.data_count + BYTES_PER_SAMPLE > ARRAY_SIZE(ld->sb.data))
        sensor_bulk_report(&ld->sb, oid);
}

void
command_query_ldc1612(uint32_t *args)
{
    struct ldc1612 *ld = oid_lookup(args[0], command_config_ldc1612);

    sched_del_timer(&ld->timer);
    ld->flags &= ~LDC_PENDING;
    if (!args[1])
        // End measurements
        return;

    // Start new measurements query
    ld->rest_ticks = args[1];
    sensor_bulk_reset(&ld->sb);
    irq_disable();
    ld->timer.waketime = timer_read_time() + ld->rest_ticks;
    sched_add_timer(&ld->timer);
    irq_enable();
}
DECL_COMMAND(command_query_ldc1612, "query_ldc1612 oid=%c rest_ticks=%u");

void
command_query_status_ldc1612(uint32_t *args)
{
    struct ldc1612 *ld = oid_lookup(args[0], command_config_ldc1612);

    if (ld->flags & LDC_HAVE_INTB) {
        // Check if a sample is pending in the chip via the intb line
        irq_disable();
        uint32_t time = timer_read_time();
        int p = check_intb_asserted(ld);
        irq_enable();
        sensor_bulk_status(&ld->sb, args[0], time, 0, p ? BYTES_PER_SAMPLE : 0);
        return;
    }

    // Query sensor to see if a sample is pending
    uint32_t time1 = timer_read_time();
    uint16_t status = read_reg_status(ld);
    uint32_t time2 = timer_read_time();

    uint32_t fifo = status & 0x08 ? BYTES_PER_SAMPLE : 0;
    sensor_bulk_status(&ld->sb, args[0], time1, time2-time1, fifo);
}
DECL_COMMAND(command_query_status_ldc1612, "query_status_ldc1612 oid=%c");

void
ldc1612_task(void)
{
    if (!sched_check_wake(&ldc1612_wake))
        return;
    uint8_t oid;
    struct ldc1612 *ld;
    foreach_oid(oid, ld, command_config_ldc1612) {
        uint_fast8_t flags = ld->flags;
        if (!(flags & LDC_PENDING))
            continue;
        ldc1612_query(ld, oid);
    }
}
DECL_TASK(ldc1612_task);
