// Support for the Chipsea CS1237 24-bit ADC
//
// Copyright (C) 2026  Xiaoyue Cui <2508041672@qq.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h>
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_poll
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_add_timer
#include "sensor_bulk.h" // sensor_bulk_report
#include "trigger_analog.h" // trigger_analog_update

struct cs1237_adc {
    struct timer timer;
    uint32_t rest_ticks;
    uint8_t config;
    uint8_t flags;
    uint8_t is_configured;
    struct gpio_in dout;
    struct gpio_out dout_out;
    struct gpio_out sclk;
    struct sensor_bulk sb;
    struct trigger_analog *ta;
};

enum {
    CS_PENDING = 1<<0, CS_OVERFLOW = 1<<1,
    CS_CONFIG_PENDING = 1<<2, CS_CONFIG_VERIFY = 1<<3,
};

#define BYTES_PER_SAMPLE 4
#define SAMPLE_ERROR_DESYNC (1L << 31)
#define SAMPLE_ERROR_READ_TOO_LONG (1L << 30)
#define SAMPLE_ERROR_CONFIG (1L << 29)
#define CS1237_WRITE_CONFIG 0x65

static struct task_wake wake_cs1237;

// The data sheet specifies at least 455ns for each SCLK high/low pulse.
#define MIN_PULSE_TIME nsecs_to_ticks(455)

static uint32_t
nsecs_to_ticks(uint32_t ns)
{
    return DIV_ROUND_UP(timer_from_us(ns * 1000), 1000000);
}

static void
cs1237_delay_noirq(void)
{
    uint32_t end = timer_read_time() + MIN_PULSE_TIME;
    while (timer_is_before(timer_read_time(), end))
        ;
}

static void
cs1237_delay(void)
{
    uint32_t end = timer_read_time() + MIN_PULSE_TIME;
    while (timer_is_before(timer_read_time(), end))
        irq_poll();
}

static uint32_t
cs1237_read_bits(struct cs1237_adc *cs, uint_fast8_t num_bits)
{
    uint32_t value = 0;
    while (num_bits--) {
        irq_disable();
        gpio_out_write(cs->sclk, 1);
        cs1237_delay_noirq();
        uint_fast8_t bit = gpio_in_read(cs->dout);
        gpio_out_write(cs->sclk, 0);
        irq_enable();
        cs1237_delay();
        value = (value << 1) | bit;
    }
    return value;
}

static void
cs1237_clock_pulses(struct cs1237_adc *cs, uint_fast8_t count)
{
    while (count--) {
        irq_disable();
        gpio_out_write(cs->sclk, 1);
        cs1237_delay_noirq();
        gpio_out_write(cs->sclk, 0);
        irq_enable();
        cs1237_delay();
    }
}

static void
cs1237_write_bits(struct cs1237_adc *cs, uint32_t value, uint32_t mask)
{
    while (mask) {
        gpio_out_write(cs->dout_out, !!(value & mask));
        cs1237_delay();
        irq_disable();
        gpio_out_write(cs->sclk, 1);
        cs1237_delay_noirq();
        gpio_out_write(cs->sclk, 0);
        irq_enable();
        cs1237_delay();
        mask >>= 1;
    }
}

// Called immediately after the 24 ADC data clocks. Clocks 25-29 announce a
// command, clocks 30-36 contain the seven bit command, clock 37 changes bus
// direction, clocks 38-45 contain Config, and clock 46 releases DRDY/DOUT.
static void
cs1237_write_config(struct cs1237_adc *cs)
{
    cs1237_clock_pulses(cs, 5);
    gpio_out_reset(cs->dout_out, 0);
    cs1237_write_bits(cs, CS1237_WRITE_CONFIG, 0x40);
    cs1237_clock_pulses(cs, 1);
    cs1237_write_bits(cs, cs->config, 0x80);
    gpio_in_reset(cs->dout, 0);
    cs1237_clock_pulses(cs, 1);
}

static uint_fast8_t
cs1237_is_data_ready(struct cs1237_adc *cs)
{
    return !gpio_in_read(cs->dout);
}

static uint_fast8_t
cs1237_event(struct timer *timer)
{
    struct cs1237_adc *cs = container_of(timer, struct cs1237_adc, timer);
    uint32_t rest_ticks = cs->rest_ticks;
    if (cs->flags & CS_PENDING) {
        cs->sb.possible_overflows++;
        cs->flags |= CS_OVERFLOW;
        rest_ticks *= 4;
    } else if (cs1237_is_data_ready(cs)) {
        cs->flags |= CS_PENDING;
        sched_wake_task(&wake_cs1237);
        rest_ticks *= 8;
    }
    cs->timer.waketime += rest_ticks;
    return SF_RESCHEDULE;
}

static void
add_sample(struct cs1237_adc *cs, uint8_t oid, uint32_t counts)
{
    cs->sb.data[cs->sb.data_count] = counts;
    cs->sb.data[cs->sb.data_count + 1] = counts >> 8;
    cs->sb.data[cs->sb.data_count + 2] = counts >> 16;
    cs->sb.data[cs->sb.data_count + 3] = counts >> 24;
    cs->sb.data_count += BYTES_PER_SAMPLE;
    if (cs->sb.data_count + BYTES_PER_SAMPLE > ARRAY_SIZE(cs->sb.data))
        sensor_bulk_report(&cs->sb, oid);
}

static void
cs1237_read_adc(struct cs1237_adc *cs, uint8_t oid)
{
    uint8_t old_flags = cs->flags;
    uint32_t raw;

    if (old_flags & CS_CONFIG_PENDING) {
        // Read and discard the sample produced with the power-on settings.
        raw = cs1237_read_bits(cs, 24);
        cs1237_write_config(cs);
    } else {
        // Bits 25-27 are update1, reserved zero, and forced high.
        raw = cs1237_read_bits(cs, 27);
    }

    irq_disable();
    old_flags |= cs->flags;
    cs->flags &= ~(CS_PENDING | CS_OVERFLOW);
    if (old_flags & CS_CONFIG_PENDING) {
        cs->flags &= ~CS_CONFIG_PENDING;
        cs->flags |= CS_CONFIG_VERIFY;
    }
    irq_enable();

    if (old_flags & CS_CONFIG_PENDING)
        return;

    uint_fast8_t status = raw & 0x07;
    uint32_t counts = raw >> 3;
    if (counts & 0x800000)
        counts |= 0xff000000;

    uint32_t error = 0;
    if ((status & 0x03) != 0x01)
        error = SAMPLE_ERROR_DESYNC;
    else if ((old_flags & CS_CONFIG_VERIFY) && !(status & 0x04))
        error = SAMPLE_ERROR_CONFIG;
    else if (old_flags & CS_OVERFLOW)
        error = SAMPLE_ERROR_READ_TOO_LONG;

    if (old_flags & CS_CONFIG_VERIFY) {
        irq_disable();
        cs->flags &= ~CS_CONFIG_VERIFY;
        if (error)
            cs->flags |= CS_CONFIG_PENDING;
        else
            cs->is_configured = 1;
        irq_enable();
    }
    if (error)
        counts = error;
    else
        trigger_analog_update(cs->ta, counts);
    add_sample(cs, oid, counts);
}

void
command_config_cs1237(uint32_t *args)
{
    struct cs1237_adc *cs = oid_alloc(args[0], command_config_cs1237,
                                      sizeof(*cs));
    cs->timer.func = cs1237_event;
    cs->config = args[1] & 0x7f;
    cs->dout = gpio_in_setup(args[2], 0);
    cs->dout_out = gpio_out_setup(args[2], 0);
    gpio_in_reset(cs->dout, 0);
    cs->sclk = gpio_out_setup(args[3], 0);
    gpio_out_write(cs->sclk, 1);
}
DECL_COMMAND(command_config_cs1237, "config_cs1237 oid=%c config=%c"
             " dout_pin=%u sclk_pin=%u");

void
cs1237_attach_trigger_analog(uint32_t *args)
{
    struct cs1237_adc *cs = oid_lookup(args[0], command_config_cs1237);
    cs->ta = trigger_analog_oid_lookup(args[1]);
}
#if CONFIG_WANT_TRIGGER_ANALOG
DECL_COMMAND(cs1237_attach_trigger_analog, "cs1237_attach_trigger_analog oid=%c"
             " trigger_analog_oid=%c");
#endif

void
command_query_cs1237(uint32_t *args)
{
    struct cs1237_adc *cs = oid_lookup(args[0], command_config_cs1237);
    sched_del_timer(&cs->timer);
    cs->flags = 0;
    cs->rest_ticks = args[1];
    if (!cs->rest_ticks) {
        gpio_out_write(cs->sclk, 1);
        return;
    }
    gpio_in_reset(cs->dout, 0);
    gpio_out_write(cs->sclk, 0);
    if (!cs->is_configured)
        cs->flags = CS_CONFIG_PENDING;
    sensor_bulk_reset(&cs->sb);
    irq_disable();
    cs->timer.waketime = timer_read_time() + cs->rest_ticks;
    sched_add_timer(&cs->timer);
    irq_enable();
}
DECL_COMMAND(command_query_cs1237, "query_cs1237 oid=%c rest_ticks=%u");

void
command_query_cs1237_status(const uint32_t *args)
{
    uint8_t oid = args[0];
    struct cs1237_adc *cs = oid_lookup(oid, command_config_cs1237);
    irq_disable();
    const uint32_t start_t = timer_read_time();
    uint8_t ready = cs1237_is_data_ready(cs);
    irq_enable();
    sensor_bulk_status(&cs->sb, oid, start_t, 0,
                       ready ? BYTES_PER_SAMPLE : 0);
}
DECL_COMMAND(command_query_cs1237_status, "query_cs1237_status oid=%c");

void
cs1237_capture_task(void)
{
    if (!sched_check_wake(&wake_cs1237))
        return;
    uint8_t oid;
    struct cs1237_adc *cs;
    foreach_oid(oid, cs, command_config_cs1237) {
        if (cs->flags & CS_PENDING)
            cs1237_read_adc(cs, oid);
    }
}
DECL_TASK(cs1237_capture_task);
