// Support for eddy current sensor data from ldc1612 chip (v2)
//
// Copyright (C) 2023 Alan.Ma <tech@biqu3d.com>
// Copyright (C) 2024  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2025 Vladimir Vukicevic <vladimir@pobox.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include <stdbool.h>
#include "autoconf.h"
#include "basecmd.h" // oid_alloc
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "i2ccmds.h" // i2cdev_oid_lookup
#include "sched.h" // DECL_TASK
#include "sensor_bulk.h" // sensor_bulk_report
#include "trsync.h" // trsync_do_trigger

#if !defined(LDC_DEBUG)
#define LDC_DEBUG 0
#endif

#if CONFIG_MACH_STM32F0
// For Cartographer
#include "board/internal.h"
#include "board/gpio.h"
#define SUPPORT_CARTOGRAPHER 1
#else
#define SUPPORT_CARTOGRAPHER 0
#endif

#if LDC_DEBUG > 0
#include "printf.h"
void dprint(const char *fmt, ...);
#else
#define dprint(...) do { } while (0)
#endif

enum {
    // There's a pending sample that needs to be read
    LDC_PENDING = 1<<0,
    // Use the intb pin to detect when a sample is ready
    // vs. just polling
    LDC_HAVE_INTB = 1<<1,
};

// Number of bytes in each ldc1612 sample. Always 4.
#define BYTES_PER_SAMPLE 4

// should match ldc1612_ng.py
#define HOME_MODE_NONE 0
#define HOME_MODE_HOME 1
#define HOME_MODE_WMA 2
#define HOME_MODE_SOS 3

// should match probe_eddy.py
#define REASON_ERROR_SENSOR 0
#define REASON_ERROR_PROBE_TOO_LOW 1
#define REASON_ERROR_TOO_EARLY 2

// should match ldc1612_ng.py
#define PRODUCT_UNKNOWN 0
#define PRODUCT_BTT_EDDY 1
#define PRODUCT_CARTOGRAPHER 2
#define PRODUCT_MELLOW_FLY 3

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

// Homing configuration
#define FREQ_WINDOW_SIZE 16
#define WMA_D_WINDOW_SIZE 4

#define MAX_SOS_SECTIONS 4

struct sosfilter_sos {
    uint8_t num_sections;
    float sos[MAX_SOS_SECTIONS*6];
};

struct ldc1612_ng_homing_wma_tap {
    // the tap detection threshold: specifically, the total downward
    // change in the frequency derivative before we see a direction
    // reveral (the windowed moving average of the derivative of the wmd
    // to be exact)
    int32_t tap_threshold;

    // number of samples to ignore for detection (to avoid spikes before
    // we fill buffers)
    uint8_t init_sample_count;

    // frequencies are always positive, as is their average
    // the derivative however is signed
    uint32_t freq_buffer[FREQ_WINDOW_SIZE];
    int32_t wma_d_buf[WMA_D_WINDOW_SIZE];
    // current index in freq/deriv buffers
    uint8_t freq_i;
    uint8_t wma_d_i;

    uint32_t wma; // last computed weighted moving average
    int32_t wma_d_avg; // last computed wma derivative average

    // the wema_d_avg at the start
    int32_t tap_start_value;
};

struct ldc1612_ng_homing_sos_tap {
    float state[MAX_SOS_SECTIONS*4];
    float tap_threshold;

    float frequency_offset;
    float tap_start_value;
    float last_value;
};

struct ldc1612_ng_homing {
    uint8_t mode;

    // frequency we must pass through to have a valid home/tap
    uint32_t safe_start_freq;
    // and it must happen after this time
    uint32_t safe_start_time;

    // the frequency to trigger on for homing, or
    // the second threshold before we start looking for a tap
    uint32_t homing_trigger_freq;

    // What time we fire with the trigger -- either the time homing
    // triggered, or the computed time for the tap (which will be
    // earlier than when the tap was detected).
    uint32_t trigger_time;

    // If it was a tap, the start of tap detection
    uint32_t tap_start_time;

    // Number of errors we've seen in a row
    uint8_t error_count;
    // Number we're allowed to see, from home setup
    uint8_t error_threshold;
    // The final error that caused an abort
    uint32_t error;

    union {
        struct ldc1612_ng_homing_wma_tap wma_tap;
        struct ldc1612_ng_homing_sos_tap sos_tap;
    };
};

struct ldc1612_ng {
    struct timer timer;
    struct i2cdev_s *i2c;
    struct sensor_bulk sb;
    struct gpio_in intb_pin;

    uint8_t product;
    float sensor_cvt;

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

    // active sosfilter
    struct sosfilter_sos sos_filter;

    // homing state
    struct ldc1612_ng_homing homing;

#if SUPPORT_CARTOGRAPHER
    struct gpio_out led_gpio;
#endif
};

void command_config_ldc1612_ng(uint32_t *args);

static void read_reg(struct ldc1612_ng* ld, uint8_t reg, uint8_t* res);
static uint16_t read_reg_status(struct ldc1612_ng* ld);

static uint_fast8_t ldc1612_ng_timer_event(struct timer* timer);

static void ldc1612_ng_update(struct ldc1612_ng* ld, uint8_t oid);

static void check_homing(struct ldc1612_ng* ld, uint32_t data, uint32_t time);
static void check_wma_tap(struct ldc1612_ng* ld, uint32_t data, uint32_t time);
static void check_sos_tap(struct ldc1612_ng* ld, uint32_t data, uint32_t time);

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
    ld->homing.mode = 0;
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
config_ldc1612_ng(uint32_t oid, uint32_t i2c_oid, uint8_t product, int32_t intb_pin)
{
    dprint("EDDYng cfg o=%u i=%u b=%d", oid, i2c_oid, intb_pin);

    struct ldc1612_ng *ld = oid_alloc(oid, command_config_ldc1612_ng, sizeof(*ld));

    ld->timer.func = ldc1612_ng_timer_event;
    ld->i2c = i2cdev_oid_lookup(i2c_oid);
    if (intb_pin != -1) {
        ld->intb_pin = gpio_in_setup(intb_pin, 1);
        ld->flags = LDC_HAVE_INTB;
    }
    ld->product = product;

    switch (product) {
    case PRODUCT_UNKNOWN:
    case PRODUCT_BTT_EDDY:
        ld->sensor_cvt = 12000000.0f / (float)(1<<28);
        break;
    case PRODUCT_MELLOW_FLY:
        ld->sensor_cvt = 40000000.0f / (float)(1<<28);
        break;
#if SUPPORT_CARTOGRAPHER
    case PRODUCT_CARTOGRAPHER:
        ld->sensor_cvt = 24000000.0f / (float)(1<<28);

        // This enables the ldc1612 (CS?)
        gpio_out_setup(GPIO('A', 15), 0);

        // The Cartographer hardware uses a timer in the STM32F0
        // to generate a 24MHz reference clock for the ldc1612.
        // Uses a new _with_max setup here because otherwise we
        // can't actually get to 24MHz from 48MHz. This could be
        // configured from the python side but that requires
        // adding a bunch of new commands.
        gpio_pwm_setup_with_max(GPIO('B', 4), 1, 1, 2);

        // There's a LED -- do something with it in the future,
        // showing homing progress
        ld->led_gpio = gpio_out_setup(GPIO('B', 5), 1);
        gpio_out_write(ld->led_gpio, 1);

        // There's also a temp sensor on A4, but we can
        // pull that out on the python side.
        break;
#endif
    default:
        shutdown("ldc1612_ng: unknown product");
    }
}

void
command_config_ldc1612_ng(uint32_t *args)
{
    uint32_t oid = args[0];
    uint32_t i2c_oid = args[1];
    uint8_t product = args[2];

    config_ldc1612_ng(oid, i2c_oid, product, -1);
}
DECL_COMMAND(command_config_ldc1612_ng, "config_ldc1612_ng oid=%c i2c_oid=%c product=%i");

void
command_config_ldc1612_ng_with_intb(uint32_t *args)
{
    uint32_t oid = args[0];
    uint32_t i2c_oid = args[1];
    uint8_t product = args[2];
    uint32_t intb_pin = args[3];

    config_ldc1612_ng(oid, i2c_oid, product, intb_pin);
}
DECL_COMMAND(command_config_ldc1612_ng_with_intb,
             "config_ldc1612_ng_with_intb oid=%c i2c_oid=%c product=%i intb_pin=%c");

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
             "query_ldc1612_ng_latched_status_v2 oid=%c");
// ^ this command name is also used as an API version of sorts

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
    uint8_t mode = args[7];
    int32_t tap_threshold = args[8];
    uint8_t err_max = args[9];

    if (trigger_freq == 0 || trsync_oid == 0) {
        dprint("ZZZ resetting homing/tapping");
        ld->ts = NULL;
        lh->mode = 0;
        return;
    }

    if (ld->rest_ticks == 0) {
        notify_trigger(ld, 0, other_reason_base);
        dprint("ZZZ sensor not started!");
        return;
    }

    if (lh->mode > 0) {
        notify_trigger(ld, 0, other_reason_base);
        dprint("ZZZ homing already set up!");
        return;
    }

    // Clear the homing state before setting up
    memset(lh, 0, sizeof(*lh));

    lh->safe_start_freq = start_freq;
    lh->safe_start_time = start_time;
    lh->homing_trigger_freq = trigger_freq;

    lh->error_threshold = err_max;

    ld->ts = trsync_oid_lookup(trsync_oid);
    ld->success_reason = trigger_reason;
    ld->other_reason_base = other_reason_base;

    lh->mode = mode;

    switch (mode) {
    case HOME_MODE_HOME:
        dprint("ZZZ setup home sf=%u tf=%u", start_freq, trigger_freq);
        break;
    case HOME_MODE_WMA:
        lh->wma_tap.tap_threshold = tap_threshold >> 16;
        lh->wma_tap.init_sample_count = FREQ_WINDOW_SIZE * 2;
        dprint("ZZZ setup wma sf=%u tf=%u tap=%u", start_freq, trigger_freq, tap_threshold);
        break;
    case HOME_MODE_SOS:
        lh->sos_tap.tap_threshold = tap_threshold / 65536.0f;
        dprint("ZZZ setup sos sf=%u tf=%u tap=%f", start_freq, trigger_freq, lh->sos_tap.tap_threshold);
        break;
    default:
        shutdown("bad homing mode");
    }
}
DECL_COMMAND(command_ldc1612_ng_setup_home,
             "ldc1612_ng_setup_home oid=%c"
             " trsync_oid=%c trigger_reason=%c other_reason_base=%c"
             " trigger_freq=%u start_freq=%u start_time=%u"
             " mode=%c tap_threshold=%i err_max=%c");

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
    uint32_t tap_start_time = lh->tap_start_time;
    uint32_t error = lh->error;

    ld->ts = NULL;
    lh->mode = 0;

    sendf("ldc1612_ng_finish_home_reply oid=%c trigger_clock=%u tap_start_clock=%u error=%u"
          , args[0], trigger_time, tap_start_time, error);

    dprint("ZZZ finish tap_s=%u trig_t=%u", tap_start_time, trigger_time);
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

    switch (ld->homing.mode) {
    case HOME_MODE_HOME: check_homing(ld, data, time); break;
    case HOME_MODE_WMA: check_wma_tap(ld, data, time); break;
    case HOME_MODE_SOS: check_sos_tap(ld, data, time); break;
    }

    // Flush local buffer if needed
    if (ld->sb.data_count + BYTES_PER_SAMPLE > ARRAY_SIZE(ld->sb.data))
        sensor_bulk_report(&ld->sb, oid);
}

static inline uint32_t
windowed_moving_average_u32(uint32_t* buf, uint8_t buf_size, uint8_t start_i)
{
    // TODO: We can avoid 64-bit integers here by just offseting
    // the input numbers to ensure that we can always add buf_size values
    // without overflow into an uint32_t. For frequencies, it should be safe
    // to subtract the safe_start_freq and just deal with offsets above that,
    // because ultimately we only care about the derivative.
    // But this 64-bit math is here for now to keep the logic simple
    // during development.
    uint64_t wma_sum = 0;
    for (uint8_t i = 0; i < buf_size; i++) {
        uint8_t j = (start_i + i) % buf_size;
        wma_sum += buf[j] * (i+1);
    }

    uint32_t freq_weight_sum = (buf_size * (buf_size + 1)) / 2;

    return (uint32_t)(wma_sum / freq_weight_sum);
}

static inline int32_t
simple_average_i32(int32_t* buf, uint8_t buf_size)
{
    // This assumes that the sum can fit in an i32.
    int32_t sum = 0;
    for (uint8_t i = 0; i < buf_size; i++) {
        sum += buf[i];
    }

    return sum / buf_size;
}

static float
sosfilter(float value, struct sosfilter_sos* filter, float* state)
{
    const uint8_t num_sections = filter->num_sections;
    const float* sos = filter->sos;

    for (int k = 0; k < num_sections; k++) {
        float w1 = state[2*k];
        float w2 = state[2*k+1];
        float b0 = *sos++; //sos[6*k];
        float b1 = *sos++; //sos[6*k+1];
        float b2 = *sos++; //sos[6*k+2];
        sos++; // a0 unused
        float a1 = *sos++; //sos[6*k+4];
        float a2 = *sos++; //sos[6*k+5];

        float w0 = value - a1 * w1 - a2 * w2;
        value = b0 * w0 + b1 * w1 + b2 * w2;

        state[2*k] = w0;
        state[2*k+1] = w1;
    }

    return value;
}

// Check whether the sample has error bits set, and decide what to do
// if it does
bool
check_error(struct ldc1612_ng* ld, uint32_t data, uint32_t time)
{
    struct ldc1612_ng_homing *lh = &ld->homing;
    if (!SAMPLE_ERR(data)) {
        lh->error_count = 0;
        return true;
    }

    uint8_t is_tap = lh->mode > 0;

    // Ignore amplitude too high errors for homing,
    // because this is generally the probe being very
    // far from the build plate.
    if (!is_tap && (ld->last_status & STATUS_ERR_AHE) != 0) {
        lh->error_count = 0;
        return false;
    }

    lh->error_count++;

    dprint("ZZZ err=%u t=%u s=%u cnt=%u", data, time, ld->last_status,
        lh->error_count);

    if (lh->error_count <= lh->error_threshold)
        return false;

    lh->error = data;

    // Sensor reports an issue - cancel homing
    notify_trigger(ld, 0, ld->other_reason_base + REASON_ERROR_SENSOR);
    return false;
}

// Check whether we've passed the safety thresholds in order for the
// operation to proceed
bool
check_safe_start(struct ldc1612_ng* ld, uint32_t data, uint32_t time)
{
    struct ldc1612_ng_homing *lh = &ld->homing;
    uint8_t is_tap = lh->mode > 0;

    if (lh->safe_start_freq == 0)
        return true;

    // We need to pass through this frequency threshold to be a valid dive.
    // We just use the simple data value here.
    if (data < lh->safe_start_freq)
        return false;

    // And we need to do it _after_ this time, to make sure we didn't
    // start below the threshold
    if (lh->safe_start_time != 0 && timer_is_before(time, lh->safe_start_time)) {
        dprint("ZZZ EARLY! time=%u < %u", time, lh->safe_start_time);
        notify_trigger(ld, 0, ld->other_reason_base + REASON_ERROR_TOO_EARLY);
        return false;
    }

    if (is_tap && lh->homing_trigger_freq != 0) {
        // If we're tapping, then make the homing trigger freq a second thershold.
        // These would typically be set to something like the 3.0mm freq for the first,
        // then the 2.0mm homing freq.
        lh->safe_start_freq = lh->homing_trigger_freq;
        lh->homing_trigger_freq = 0;
        return false;
    }

    dprint("ZZZ safe start");

    // Ok, we've passed all the safety thresholds. Values from this point on
    // will be considered for homing/tapping
    lh->safe_start_freq = 0;

    return true;
}

//
// Basic homing (simple threshold)
//
void
check_homing(struct ldc1612_ng* ld, uint32_t data, uint32_t time)
{
    struct ldc1612_ng_homing *lh = &ld->homing;

    if (!check_error(ld, data, time))
        return;
    
    if (!check_safe_start(ld, data, time))
        return;
    
    if (data > lh->homing_trigger_freq) {
        notify_trigger(ld, time, ld->success_reason);
        lh->trigger_time = time;
        dprint("ZZZ home t=%u f=%u", time, data);
    }
}

//
// Tap detection using a windowed moving average of the frequency derivative
//
void
check_wma_tap(struct ldc1612_ng* ld, uint32_t data, uint32_t time)
{
    struct ldc1612_ng_homing *lh = &ld->homing;
    struct ldc1612_ng_homing_wma_tap *wma_tap = &lh->wma_tap;

    if (!check_error(ld, data, time))
        return;

    if (!check_safe_start(ld, data, time))
        return;
    
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

    // TODO: the below can absolutely be made more efficient, but I'm
    // keeping it simple while things are dialed in.

    // Helpers to clean up the adds/mods/etc. to make the below more readable
    #define NEXT_FREQ_I(i) (((i) + 1) % FREQ_WINDOW_SIZE)
    #define NEXT_WMA_D_I(i) (((i) + 1) % WMA_D_WINDOW_SIZE)

    wma_tap->freq_buffer[wma_tap->freq_i] = data;
    wma_tap->freq_i = NEXT_FREQ_I(wma_tap->freq_i);

    uint32_t wma = windowed_moving_average_u32(wma_tap->freq_buffer, FREQ_WINDOW_SIZE, wma_tap->freq_i);
    int32_t wma_d = wma - wma_tap->wma;

    // A simple average of wma_d to smooth it out a bit. Without this,
    // we'll see some small spikes which will reset the accumulator;
    // I think this is due to the drip move.
    wma_tap->wma_d_buf[wma_tap->wma_d_i] = wma_d;
    wma_tap->wma_d_i = NEXT_WMA_D_I(wma_tap->wma_d_i);

    int32_t wma_d_avg = simple_average_i32(wma_tap->wma_d_buf, WMA_D_WINDOW_SIZE);
    int32_t last_wma_d_avg = wma_tap->wma_d_avg;

    wma_tap->wma = wma;
    wma_tap->wma_d_avg = wma_d_avg;

    if (wma_tap->init_sample_count) {
        wma_tap->init_sample_count--;
        return;
    }

    // The core tap threshold computation. If the derivative is
    // increasing, keep resetting the tap start until we hit a peak.

    if (wma_d_avg > last_wma_d_avg) {
        // derivative is increasing; reset the accumulator,
        // and reset the tap time
        lh->tap_start_time = time;
        wma_tap->tap_start_value = wma_d_avg;
        return;
    }

    if (wma_tap->tap_start_value - wma_d_avg >= wma_tap->tap_threshold) {
        // Note: we notify with the time the tap started, not the current time
        notify_trigger(ld, lh->tap_start_time, ld->success_reason);
        lh->trigger_time = time;
        dprint("ZZZ tap t=%u n=%u l=%u (f=%u)", lh->tap_start_time, time, wma_tap->tap_start_value - wma_d_avg, data);
    }
}

void
check_sos_tap(struct ldc1612_ng* ld, uint32_t data, uint32_t time)
{
    struct ldc1612_ng_homing *lh = &ld->homing;
    struct ldc1612_ng_homing_sos_tap *sos_tap = &lh->sos_tap;

    if (!check_error(ld, data, time))
        return;

    float freq = data * ld->sensor_cvt;

    // We need to offset the frequencies by the first
    // one we feed to the filter so we don't get a crazy
    // response at the start.

    // if we haven't even hit the safe_start_freq
    if (lh->homing_trigger_freq != 0) {
        sos_tap->frequency_offset = freq;
        if (check_safe_start(ld, data, time))
            shutdown("bug"); // this should never return true in here
        return;
    }

    float val = sosfilter(freq - sos_tap->frequency_offset, &ld->sos_filter, sos_tap->state);
    //dprint("%f,%f", freq, val);

    // this is the second threshold; but we want to feed the filter values
    // before this to avoid the initial impulse response
    if (!check_safe_start(ld, data, time))
        return;

    // Note: == is explicitly excluded below. We don't want to
    // overwrite the "start" time (so >= won't work), and
    // it can't make a difference to the last diff check
    if (val < sos_tap->last_value) {
        float diff = sos_tap->tap_start_value - val;
        if (diff >= sos_tap->tap_threshold) {
            lh->trigger_time = time;
            notify_trigger(ld, time, ld->success_reason);
            dprint("ZZZ tap st=%u tt=%u l=%f (f=%f)", lh->tap_start_time, time, sos_tap->tap_start_value - val, freq);
            return;
        }
    } else if (val > sos_tap->last_value) {
        // This keeps getting updated even on the rise, so that
        // the values are correct for the start of the tap (i.e. the peak)
        // once we realize the value is falling.
        sos_tap->tap_start_value = val;
        lh->tap_start_time = time;
    }

    sos_tap->last_value = val;
}

void
command_ldc1612_ng_set_sos_section(uint32_t *args)
{
    struct ldc1612_ng *ld = oid_lookup(args[0], command_config_ldc1612_ng);
    uint8_t section = args[1];
    uint8_t values_len = args[2];
    if (values_len == 0) {
        // reset filter
        ld->sos_filter.num_sections = 0;
        return;
    }

    uint8_t* data = command_decode_ptr(args[3]);
    if (values_len != 4*6) {
        shutdown("ldc1612_ng: wrong sos section length");
    }

    // these commands need to come in order of increasing section
    ld->sos_filter.num_sections = section + 1;
    memcpy(&ld->sos_filter.sos[section*6], data, values_len);
}
DECL_COMMAND(command_ldc1612_ng_set_sos_section,
             "ldc1612_ng_set_sos_section oid=%c section=%c values=%*s");
