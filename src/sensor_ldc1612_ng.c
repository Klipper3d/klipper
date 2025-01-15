// Support for eddy current sensor data from ldc1612 chip (v2)
//
// Copyright (C) 2023 Alan.Ma <tech@biqu3d.com>
// Copyright (C) 2024  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2025 Vladimir Vukicevic <vladimir@pobox.com>
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
#include "printf.h"

#define LDC_DEBUG 1
#if defined(LDC_DEBUG) && LDC_DEBUG > 0
void dprint(const char *fmt, ...);
#else
#define dprint(...) do { } while (0)
#endif

enum {
    LDC_PENDING = 1<<0, LDC_HAVE_INTB = 1<<1,
    LH_AWAIT_HOMING = 1<<1, LH_AWAIT_TAP = 1<<2,
    LH_CAN_TRIGGER = 1<<3, LH_WANT_TAP = 1<<4
};

#define BYTES_PER_SAMPLE 4

// should match probe_eddy.py
#define REASON_ERROR_SENSOR 0
#define REASON_ERROR_PROBE_TOO_LOW 1
#define REASON_ERROR_TOO_EARLY 2

// Chip registers
#define REG_DATA0_MSB 0x00
#define REG_DATA0_LSB 0x01
#define REG_STATUS    0x18

// Error flags reported in samples: undeer range, over range, watchdog, amplitude 
#define SAMPLE_ERR(data) ((data) >> 28)
#define SAMPLE_ERR_UR 0x8
#define SAMPLE_ERR_OR 0x4
#define SAMPLE_ERR_WD 0x2
#define SAMPLE_ERR_AE 0x1

// conversion under range
#define STATUS_ERR_UR 0x2000
// conversion over range
#define STATUS_ERR_OR 0x1000
// watchdog timeout
#define STATUS_ERR_WD 0x0800
// amplitude high error
#define STATUS_ERR_AHE 0x0400
// amplitude low error
#define STATUS_ERR_ALE 0x0200

// Configuration
#define FREQ_WINDOW_SIZE 16
#define WMA_D_WINDOW_SIZE 4

struct ldc1612_ng_homing {
    // frequencies are always positive, as is their average
    // the derivative however is signed

    uint32_t freq_buffer[FREQ_WINDOW_SIZE];
    int32_t wma_d_buf[WMA_D_WINDOW_SIZE];
    // current index in freq/deriv buffers
    uint8_t freq_i;
    uint8_t wma_d_i;

    uint32_t wma; // last computed weighted moving average
    int32_t wma_d_avg; // last computed wma derivative average

    // where we keep track
    uint32_t tap_accum;
    // the earliest start of this tap
    uint32_t tap_start_time;
    // when we actually fired the trigger
    uint32_t tap_end_time;

    // the time we included when we fired a trigger (same as homing_clock
    // in parent struct, due to code structure) -- either the regular homing
    // threshold hit, or the time we started detecting the last tap
    // sequence
    uint32_t trigger_time;

    //
    // input parameters
    //

    // frequency we must pass through to have a valid home/tap
    uint32_t safe_start_freq;
    // and it must happen after this time
    uint32_t safe_start_time;

    // the frequency to trigger on for homing, or
    // the second threshold before we start looking for a tap
    uint32_t homing_trigger_freq;

    // the tap detection threshold: specifically, the total downward
    // change in the frequency derivative before we see a direction
    // reveral (the windowed moving average of the derivative of the wmd
    // to be exact)
    uint32_t tap_threshold;
};

struct ldc1612_ng {
    struct timer timer;
    struct i2cdev_s *i2c;
    struct sensor_bulk sb;
    struct gpio_in intb_pin;

    uint32_t rest_ticks;
    uint8_t flags;
    uint16_t last_status;
    uint32_t last_read_value;

    // Samples per second (configurable)
    uint32_t data_rate;

    // homing triggers
    struct trsync *ts;
    uint8_t success_reason;
    uint8_t other_reason_base;

    // homing state
    uint8_t homing_flags;
    struct ldc1612_ng_homing homing;
};

void command_config_ldc1612_ng(uint32_t *args);

static void read_reg(struct ldc1612_ng* ld, uint8_t reg, uint8_t* res);
static uint16_t read_reg_status(struct ldc1612_ng* ld);

static uint_fast8_t ldc1612_ng_timer_event(struct timer* timer);

static void ldc1612_ng_update(struct ldc1612_ng* ld, uint8_t oid);
static void ldc1612_ng_check_home(struct ldc1612_ng* ld, uint32_t data, uint32_t time);

//
// Core sample timers and loop
//
static struct task_wake ldc1612_ng_wake;

static int
check_intb_asserted(struct ldc1612_ng *ld)
{
    return !gpio_in_read(ld->intb_pin);
}

void
ldc1612_ng_task(void)
{
    if (!sched_check_wake(&ldc1612_ng_wake))
        return;
    uint8_t oid;
    struct ldc1612_ng *ld;
    foreach_oid(oid, ld, command_config_ldc1612_ng) {
        uint_fast8_t flags = ld->flags;
        if (!(flags & LDC_PENDING))
            continue;
        ldc1612_ng_update(ld, oid);
    }
}
DECL_TASK(ldc1612_ng_task);

uint_fast8_t
ldc1612_ng_timer_event(struct timer *timer)
{
    struct ldc1612_ng *ld = container_of(timer, struct ldc1612_ng, timer);

    if (ld->flags & LDC_PENDING)
        ld->sb.possible_overflows++;

    if (!(ld->flags & LDC_HAVE_INTB) || check_intb_asserted(ld)) {
        ld->flags |= LDC_PENDING;
        sched_wake_task(&ldc1612_ng_wake);
    }

    // reschedule to run in rest_ticks
    ld->timer.waketime += ld->rest_ticks;
    return SF_RESCHEDULE;
}

// Read a register on the ldc1612
void
read_reg(struct ldc1612_ng *ld, uint8_t reg, uint8_t *res)
{
    int ret = i2c_dev_read(ld->i2c, sizeof(reg), &reg, 2, res);
    i2c_shutdown_on_err(ret);
}

// Read the status register on the ldc1612
uint16_t
read_reg_status(struct ldc1612_ng *ld)
{
    uint8_t data_status[2];
    read_reg(ld, REG_STATUS, data_status);
    ld->last_status = (data_status[0] << 8) | data_status[1];
    return ld->last_status;
}

// Notify trsync of event
static void
notify_trigger(struct ldc1612_ng *ld, uint32_t time, uint8_t reason)
{
    ld->homing_flags = 0;
    trsync_do_trigger(ld->ts, reason);
    dprint("ZZZ notify_trigger: %u at %u", reason, time);
}

void
ldc1612_ng_shutdown(void)
{
    // make sure we stop measurements on shutdown so we don't
    // spam host on startup
    uint8_t oid;
    struct ldc1612_ng *ld;
    foreach_oid(oid, ld, command_config_ldc1612_ng) {
        sched_del_timer(&ld->timer);
        ld->flags &= ~LDC_PENDING;
        ld->rest_ticks = 0;
    }
}
DECL_SHUTDOWN(ldc1612_ng_shutdown);

static void
config_ldc1612_ng(uint32_t oid, uint32_t i2c_oid, int32_t intb_pin)
{
    dprint("EDDYng cfg o=%u i=%u b=%d", oid, i2c_oid, intb_pin);

    struct ldc1612_ng *ld = oid_alloc(oid, command_config_ldc1612_ng, sizeof(*ld));

    ld->timer.func = ldc1612_ng_timer_event;
    ld->i2c = i2cdev_oid_lookup(i2c_oid);
    if (intb_pin != -1) {
        ld->intb_pin = gpio_in_setup(intb_pin, 1);
        ld->flags = LDC_HAVE_INTB;
    }
}

void
command_config_ldc1612_ng(uint32_t *args)
{
    uint32_t oid = args[0];
    uint32_t i2c_oid = args[1];

    config_ldc1612_ng(oid, i2c_oid, -1);
}
DECL_COMMAND(command_config_ldc1612_ng, "config_ldc1612_ng oid=%c i2c_oid=%c");

void
command_config_ldc1612_ng_with_intb(uint32_t *args)
{
    uint32_t oid = args[0];
    uint32_t i2c_oid = args[1];
    uint32_t intb_pin = args[2];

    config_ldc1612_ng(oid, i2c_oid, intb_pin);
}
DECL_COMMAND(command_config_ldc1612_ng_with_intb,
             "config_ldc1612_ng_with_intb oid=%c i2c_oid=%c intb_pin=%c");

void
command_query_ldc1612_ng_latched_status(uint32_t *args)
{
    struct ldc1612_ng *ld = oid_lookup(args[0], command_config_ldc1612_ng);

    uint32_t status = ld->last_status;
    uint32_t lastval = ld->last_read_value;

    // If we're not actively running, then read the status and
    // value directly
    if (ld->rest_ticks == 0) {
        irq_disable();
        status = read_reg_status(ld);
        uint8_t d[4];
        read_reg(ld, REG_DATA0_MSB, &d[0]);
        read_reg(ld, REG_DATA0_LSB, &d[2]);
        irq_enable();

        lastval =   ((uint32_t)d[0] << 24)
                  | ((uint32_t)d[1] << 16)
                  | ((uint32_t)d[2] << 8)
                  | ((uint32_t)d[3]);
    }

    sendf("ldc1612_ng_latched_status oid=%c status=%u lastval=%u"
          , args[0], status, lastval);
}
DECL_COMMAND(command_query_ldc1612_ng_latched_status,
             "query_ldc1612_ng_latched_status oid=%c");

void
command_ldc1612_ng_start_stop(uint32_t *args)
{
    struct ldc1612_ng *ld = oid_lookup(args[0], command_config_ldc1612_ng);

    sched_del_timer(&ld->timer);
    ld->flags &= ~LDC_PENDING;
    ld->rest_ticks = args[1];

    if (ld->rest_ticks == 0) {
        // End measurements
        dprint("ZZZ stop");
        return;
    }

    dprint("ZZZ start");

    // Start new measurements query
    sensor_bulk_reset(&ld->sb);
    irq_disable();
    ld->timer.waketime = timer_read_time() + ld->rest_ticks;
    sched_add_timer(&ld->timer);
    irq_enable();
}
DECL_COMMAND(command_ldc1612_ng_start_stop, "ldc1612_ng_start_stop oid=%c rest_ticks=%u");

void
command_ldc1612_ng_query_bulk_status(uint32_t *args)
{
    struct ldc1612_ng *ld = oid_lookup(args[0], command_config_ldc1612_ng);

    if (ld->flags & LDC_HAVE_INTB) {
        // Check if a sample is pending in the chip via the intb line
        irq_disable();
        uint32_t time = timer_read_time();
        int p = check_intb_asserted(ld);
        irq_enable();
        sensor_bulk_status(&ld->sb, args[0], time, 0, p ? BYTES_PER_SAMPLE : 0);
    } else {
        // Query sensor to see if a sample is pending
        uint32_t time1 = timer_read_time();
        uint16_t status = read_reg_status(ld);
        uint32_t time2 = timer_read_time();

        uint32_t fifo = status & 0x08 ? BYTES_PER_SAMPLE : 0;
        sensor_bulk_status(&ld->sb, args[0], time1, time2-time1, fifo);
    }
}
DECL_COMMAND(command_ldc1612_ng_query_bulk_status, "ldc1612_ng_query_bulk_status oid=%c");

#if defined(LDC_DEBUG) && LDC_DEBUG > 0
void dprint(const char *fmt, ...)
{
    char buf[60];

    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buf, sizeof(buf)-1, fmt, args);
    va_end(args);

    sendf("debug_print m=%*s", len, buf);
}
#endif

//
// Set up and start homing.  This assumes the sensor has been started;
// it will error otherwise.
//
void
command_ldc1612_ng_setup_home(uint32_t *args)
{
    struct ldc1612_ng *ld = oid_lookup(args[0], command_config_ldc1612_ng);
    struct ldc1612_ng_homing *lh = &ld->homing;

    uint32_t trsync_oid = args[1];
    uint8_t trigger_reason = args[2];
    uint8_t other_reason_base = args[3];
    uint32_t trigger_freq = args[4];
    uint32_t start_freq = args[5];
    uint32_t start_time = args[6];
    int32_t tap_threshold = args[7];

    if (trigger_freq == 0 || trsync_oid == 0) {
        dprint("ZZZ resetting homing/tapping");
        ld->ts = NULL;
        ld->homing_flags = 0;
        return;
    }

    if (ld->rest_ticks == 0) {
        notify_trigger(ld, 0, other_reason_base);
        dprint("ZZZ sensor not started!");
        return;
    }

    if (ld->homing_flags & LH_CAN_TRIGGER) {
        notify_trigger(ld, 0, other_reason_base);
        dprint("ZZZ homing already set up!");
        return;
    }

    // Clear the homing state before setting up
    memset(lh, 0, sizeof(*lh));

    lh->safe_start_freq = start_freq;
    lh->safe_start_time = start_time;
    lh->homing_trigger_freq = trigger_freq;
    lh->tap_threshold = tap_threshold;

    ld->ts = trsync_oid_lookup(trsync_oid);
    ld->success_reason = trigger_reason;
    ld->other_reason_base = other_reason_base;

    ld->homing_flags = LH_AWAIT_HOMING | LH_CAN_TRIGGER;
    if (tap_threshold) {
        ld->homing_flags |= LH_WANT_TAP;
    }

    dprint("ZZZ setup sf=%u tf=%u tap=%u", start_freq, trigger_freq, tap_threshold);
}
DECL_COMMAND(command_ldc1612_ng_setup_home,
             "ldc1612_ng_setup_home oid=%c"
             " trsync_oid=%c trigger_reason=%c other_reason_base=%c"
             " trigger_freq=%u start_freq=%u start_time=%u"
             " tap_threshold=%i");

//
// Once homing has finished, call this to clear the homing state and
// retrieve the tap end time and tap final threshold amount.
//
void
command_ldc1612_ng_finish_home(uint32_t *args)
{
    struct ldc1612_ng *ld = oid_lookup(args[0], command_config_ldc1612_ng);
    struct ldc1612_ng_homing *lh = &ld->homing;

    uint32_t trigger_time = lh->trigger_time; // note: same as homing_clock in parent struct
    uint32_t tap_end_time = lh->tap_end_time;
    uint32_t tap_amount = lh->tap_accum;

    ld->ts = NULL;
    ld->homing_flags = 0;

    sendf("ldc1612_ng_finish_home_reply oid=%c trigger_clock=%u tap_end_clock=%u tap_amount=%u"
          , args[0], trigger_time, tap_end_time, tap_amount);

    dprint("ZZZ finish trig_t=%u tap_t=%u tap=%u", trigger_time, tap_end_time, tap_amount);
}
DECL_COMMAND(command_ldc1612_ng_finish_home,
             "ldc1612_ng_finish_home oid=%c");

// Read a value from the chip if one is ready, put it in the bulk data buffer,
// and do any processing if we're homing.
void
ldc1612_ng_update(struct ldc1612_ng *ld, uint8_t oid)
{
    uint16_t status = read_reg_status(ld);
    irq_disable();
    ld->flags &= ~LDC_PENDING;
    irq_enable();
    if (!(status & 0x08)) // UNREADCONV1
        return;

    uint32_t time = timer_read_time();

    // Read coil0 frequency
    uint8_t *d = &ld->sb.data[ld->sb.data_count];
    read_reg(ld, REG_DATA0_MSB, &d[0]);
    read_reg(ld, REG_DATA0_LSB, &d[2]);
    ld->sb.data_count += BYTES_PER_SAMPLE;

    uint32_t data =   ((uint32_t)d[0] << 24)
                    | ((uint32_t)d[1] << 16)
                    | ((uint32_t)d[2] << 8)
                    | ((uint32_t)d[3]);

    ld->last_read_value = data;

    if (ld->homing_flags & LH_CAN_TRIGGER) {
        ldc1612_ng_check_home(ld, data, time);
    }

    // Flush local buffer if needed
    if (ld->sb.data_count + BYTES_PER_SAMPLE > ARRAY_SIZE(ld->sb.data))
        sensor_bulk_report(&ld->sb, oid);
}

#define WEIGHT_SUM(size) ((size * (size + 1)) / 2)

void
ldc1612_ng_check_home(struct ldc1612_ng* ld, uint32_t data, uint32_t time)
{
    // should be a constexpr, the compiler will sort it out
    const uint64_t s_freq_weight_sum = WEIGHT_SUM(FREQ_WINDOW_SIZE);

    struct ldc1612_ng_homing *lh = &ld->homing;
    uint8_t homing_flags = ld->homing_flags;
    uint8_t is_tap = !!(homing_flags & LH_WANT_TAP);

    if (SAMPLE_ERR(data)) {
        // TODO: test homing from very high up

        // Ignore amplitude errors (likely probe too far),
        // unless we're tapping, in which case we should consider
        // all errors for safety -- it means the tap wasn't started
        // at an appropriate distance
        if (!is_tap && (ld->last_status & STATUS_ERR_AHE) != 0)
            return;

        dprint("ZZZ err=%u t=%u s=%u", data, time, ld->last_status);

        // Sensor reports an issue - cancel homing
        notify_trigger(ld, 0, ld->other_reason_base + REASON_ERROR_SENSOR);
        return;
    }

    //
    // Update the sensor averages and derivatives
    //
    // We use a windowed moving average for the frequencies. This seems to give a
    // better signal after staring at a jupyter notebook with plotly plots
    // for far too long. Because the values are always increasing as we probe,
    // WMA undershoots the true value by by a bit but it does a great job of
    // smoothing out the noise in the sensor.
    //
    // Because the sensor is always going to be used at the same ranges,
    // we could calibrate a fixed offset to apply to the frequency values (by
    // calculating the average offset between the true centered average vs.
    // the WMA to get a more accurate number.
    //
    // However, the actual frequency value itself is only used for coarse homing;
    // and because we're not doing any temperature calibration coarse homing
    // is never going to be super accurate anyway.
    //
    // Tap detection is done by looking at the derivative of this value only.
    //

    lh->freq_buffer[lh->freq_i] = data;
    lh->freq_i = (lh->freq_i + 1) % FREQ_WINDOW_SIZE;

    // TODO: We can avoid 64-bit integers here by just offseting
    // the numbers -- it should be safe to subtract the safe_start_freq
    // and just deal with offsets above that, because ultimately we
    // only care about the derivative. But do 64-bit math here
    // for now
    uint64_t wma_numerator = 0;
    for (int i = 0; i < FREQ_WINDOW_SIZE; i++) {
        int j = (lh->freq_i + i) % FREQ_WINDOW_SIZE;
        uint64_t weight = i + 1;
        uint64_t val = lh->freq_buffer[j];
        wma_numerator += val * weight;
    }

    // WMA and derivative of the WMA
    uint32_t wma = (uint32_t)(wma_numerator / s_freq_weight_sum);
    int32_t wma_d = (int32_t)wma - (int32_t)lh->wma;

    // A simple average of wma_d to smooth it out a bit. Without this,
    // we'll see some small spikes which will reset the accumulator;
    // I think this is due to the drip move.
    lh->wma_d_buf[lh->wma_d_i] = wma_d;
    lh->wma_d_i = (lh->wma_d_i + 1) % WMA_D_WINDOW_SIZE;
    int32_t wma_d_avg = 0;
    for (int i = 0; i < WMA_D_WINDOW_SIZE; i++) {
        wma_d_avg += lh->wma_d_buf[i];
    }
    wma_d_avg = wma_d_avg / WMA_D_WINDOW_SIZE;

    // The core tap threshold computation.  If the derivative is decreasing,
    // accumulate the amount of decrease; that's checked at the end
    // against the set threshold. If it ever goes back up,
    // reset the accumulator.
    if (wma_d_avg < lh->wma_d_avg) {
        // derivative is decreasing; track it
        lh->tap_accum += lh->wma_d_avg - wma_d_avg;
    } else {
        // derivative is increasing; reset the accumulator,
        // and reset the tap time
        lh->tap_accum = 0;
        lh->tap_start_time = time;
    }

    lh->wma = wma;
    lh->wma_d_avg = wma_d_avg;

    // Safety threshold checks
    //
    // (We do the calculations above this check so that when these thresholds
    // are passed, we have accurate wma/wma_d values already in progress)
    if (homing_flags & LH_AWAIT_HOMING) {
        // We need to pass through this frequency threshold to be a valid dive.
        // We just use the simple data value here.
        if (data < lh->safe_start_freq)
            return;

        // And we need to do it _after_ this time, to make sure we didn't
        // start below the thershold
        if (lh->safe_start_time != 0 && timer_is_before(time, lh->safe_start_time)) {
            dprint("ZZZ EARLY! time=%u < %u", time, lh->safe_start_time);
            notify_trigger(ld, 0, ld->other_reason_base + REASON_ERROR_TOO_EARLY);
            return;
        }

        if (is_tap && lh->homing_trigger_freq != 0) {
            // If we're tapping, then make the homing trigger freq a second thershold.
            // These would typically be set to something like the 3.0mm freq for the first,
            // then the 2.0mm homing freq.
            lh->safe_start_freq = lh->homing_trigger_freq;
            lh->homing_trigger_freq = 0;
            return;
        }

        dprint("ZZZ safe start");

        // Ok, we've passed all the safety thresholds. Values from this point on
        // will be considered for homing/tapping
        lh->tap_accum = 0;
        ld->homing_flags = homing_flags = homing_flags & ~LH_AWAIT_HOMING;
    }

    //dprint("data=%u avg %u", data, avg);

    if (!is_tap) {
        // Non-tap: a simple trigger on the homing frequency.  We just use the
        // raw value, because the wma is always going to undershoot more than the
        // noise; this provides a more accurate result. (Yes this means
        // we don't actually have to do the computations above for non-tap.)
        if (data > lh->homing_trigger_freq) {
            notify_trigger(ld, time, ld->success_reason);
            lh->trigger_time = time;
            dprint("ZZZ home t=%u f=%u", time, data);
        }
    } else {
        if (lh->tap_accum > lh->tap_threshold) {
            // Note: we notify with the time the tap started, not the current time
            notify_trigger(ld, lh->tap_start_time, ld->success_reason);
            lh->trigger_time = lh->tap_start_time;
            lh->tap_end_time = time;
            dprint("ZZZ tap t=%u n=%u l=%u (f=%u)", lh->tap_start_time, time, lh->tap_accum, data);
        }
    }
}