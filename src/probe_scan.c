// Continuous bed scanning with binary probe - GPIO monitor with timestamped
// state transitions streamed to host via bulk sensor subsystem.
//
// Copyright (C) 2026  Matti Airas <matti.airas@hatlabs.fi>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio_in
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "sched.h" // struct timer
#include "sensor_bulk.h" // sensor_bulk_reset

// Per-event record: MCU clock (4 bytes) + state (1 byte)
#define EVENT_SIZE 5

// Flush buffered events after this many poll ticks without a send.
// At 50us poll interval: 1000 ticks = 50ms.
#define FLUSH_TICKS 1000

struct probe_scan {
    struct timer time;
    struct gpio_in pin;
    uint32_t rest_ticks;
    uint8_t invert;
    uint8_t last_state;
    uint16_t flush_countdown;
    struct sensor_bulk sb;
};

static struct task_wake probe_scan_wake;

static void
record_event(struct probe_scan *ps, uint32_t clock, uint8_t state)
{
    if (ps->sb.data_count + EVENT_SIZE > sizeof(ps->sb.data)) {
        ps->sb.possible_overflows++;
        return;
    }
    uint8_t *d = &ps->sb.data[ps->sb.data_count];
    d[0] = clock;
    d[1] = clock >> 8;
    d[2] = clock >> 16;
    d[3] = clock >> 24;
    d[4] = state;
    ps->sb.data_count += EVENT_SIZE;
    if (ps->sb.data_count + EVENT_SIZE > sizeof(ps->sb.data))
        sched_wake_task(&probe_scan_wake);
}

static uint_fast8_t
probe_scan_event(struct timer *t)
{
    struct probe_scan *ps = container_of(t, struct probe_scan, time);
    uint8_t val = gpio_in_read(ps->pin) ? 1 : 0;
    uint8_t state = val ^ ps->invert;

    if (state != ps->last_state) {
        uint32_t clock = timer_read_time();
        ps->last_state = state;
        record_event(ps, clock, state);
        ps->flush_countdown = FLUSH_TICKS;
    }

    // Periodic flush for sparse events
    if (ps->sb.data_count && ps->flush_countdown) {
        ps->flush_countdown--;
        if (!ps->flush_countdown)
            sched_wake_task(&probe_scan_wake);
    }

    ps->time.waketime += ps->rest_ticks;
    return SF_RESCHEDULE;
}

void
command_config_probe_scan(uint32_t *args)
{
    struct probe_scan *ps = oid_alloc(
        args[0], command_config_probe_scan, sizeof(*ps));
    ps->pin = gpio_in_setup(args[1], args[2]);
}
DECL_COMMAND(command_config_probe_scan,
             "config_probe_scan oid=%c pin=%c pull_up=%c");

void
command_probe_scan_start(uint32_t *args)
{
    struct probe_scan *ps = oid_lookup(args[0], command_config_probe_scan);
    sched_del_timer(&ps->time);

    ps->time.waketime = args[1];
    ps->rest_ticks = args[2];
    ps->invert = args[3];
    ps->flush_countdown = 0;

    sensor_bulk_reset(&ps->sb);

    uint8_t val = gpio_in_read(ps->pin) ? 1 : 0;
    ps->last_state = val ^ ps->invert;

    ps->time.func = probe_scan_event;
    sched_add_timer(&ps->time);
}
DECL_COMMAND(command_probe_scan_start,
             "probe_scan_start oid=%c clock=%u rest_ticks=%u invert=%c");

void
command_probe_scan_stop(uint32_t *args)
{
    struct probe_scan *ps = oid_lookup(args[0], command_config_probe_scan);
    sched_del_timer(&ps->time);
    if (ps->sb.data_count > 0)
        sched_wake_task(&probe_scan_wake);
}
DECL_COMMAND(command_probe_scan_stop, "probe_scan_stop oid=%c");

void
command_query_probe_scan_status(uint32_t *args)
{
    struct probe_scan *ps = oid_lookup(args[0], command_config_probe_scan);
    uint32_t time1 = timer_read_time();
    uint32_t time2 = timer_read_time();
    sensor_bulk_status(&ps->sb, args[0], time1, time2 - time1, 0);
}
DECL_COMMAND(command_query_probe_scan_status,
             "query_probe_scan_status oid=%c");

void
probe_scan_task(void)
{
    if (!sched_check_wake(&probe_scan_wake))
        return;
    uint8_t oid;
    struct probe_scan *ps;
    foreach_oid(oid, ps, command_config_probe_scan) {
        uint8_t buf[sizeof(ps->sb.data)];
        uint8_t count;
        uint16_t seq;
        irq_disable();
        count = ps->sb.data_count;
        if (count > 0) {
            __builtin_memcpy(buf, ps->sb.data, count);
            seq = ps->sb.sequence;
            ps->sb.data_count = 0;
            ps->sb.sequence++;
            ps->flush_countdown = FLUSH_TICKS;
        }
        irq_enable();
        if (count > 0)
            sendf("sensor_bulk_data oid=%c sequence=%hu data=%*s"
                  , oid, seq, count, buf);
    }
}
DECL_TASK(probe_scan_task);
