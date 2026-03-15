// Continuous bed scanning with binary probe - GPIO monitor with timestamped
// state transitions streamed to host.
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

// Per-event record: MCU clock (4 bytes) + state (1 byte)
#define EVENT_SIZE 5
#define MAX_EVENTS 16

// Retransmit state machine (following buttons.c pattern)
// Values >= RT_NO_RETRANSMIT are "idle" states
// Values < RT_NO_RETRANSMIT are countdown ticks until retransmit
enum {
    RT_NO_RETRANSMIT = 0x80,
    RT_PENDING = 0xff,  // New data waiting to be sent
    RT_ACKED = 0xfe     // All data acknowledged
};

struct probe_scan {
    struct timer time;
    struct gpio_in pin;
    uint32_t rest_ticks;
    uint8_t invert;
    uint8_t last_state;
    // Event buffer — written by timer ISR, read by task after gating
    uint8_t event_count;
    uint8_t ack_count;
    uint8_t overflow;
    uint8_t retransmit_state;
    uint8_t retransmit_count;  // Configured countdown value for retransmit
    uint8_t events[MAX_EVENTS * EVENT_SIZE];
};

static struct task_wake probe_scan_wake;

static void
record_event(struct probe_scan *ps, uint32_t clock, uint8_t state)
{
    if (ps->event_count >= MAX_EVENTS) {
        if (ps->overflow < 255)
            ps->overflow++;
        return;
    }
    uint8_t *p = &ps->events[ps->event_count * EVENT_SIZE];
    p[0] = clock & 0xff;
    p[1] = (clock >> 8) & 0xff;
    p[2] = (clock >> 16) & 0xff;
    p[3] = (clock >> 24) & 0xff;
    p[4] = state;
    ps->event_count++;
    sched_wake_task(&probe_scan_wake);
    ps->retransmit_state = RT_PENDING;
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
    }

    // Retransmit countdown (fires every poll tick)
    uint8_t rs = ps->retransmit_state;
    if (!(rs & RT_NO_RETRANSMIT)) {
        rs--;
        if (rs & RT_NO_RETRANSMIT)
            // Countdown expired — trigger retransmit
            sched_wake_task(&probe_scan_wake);
        ps->retransmit_state = rs;
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
    ps->retransmit_count = args[4];
    if (ps->retransmit_count >= RT_NO_RETRANSMIT)
        shutdown("Invalid probe_scan retransmit count");
    ps->event_count = 0;
    ps->ack_count = 0;
    ps->overflow = 0;
    ps->retransmit_state = RT_ACKED;

    uint8_t val = gpio_in_read(ps->pin) ? 1 : 0;
    ps->last_state = val ^ ps->invert;

    ps->time.func = probe_scan_event;
    sched_add_timer(&ps->time);
}
DECL_COMMAND(command_probe_scan_start,
             "probe_scan_start oid=%c clock=%u rest_ticks=%u"
             " invert=%c retransmit_count=%c");

void
command_probe_scan_stop(uint32_t *args)
{
    struct probe_scan *ps = oid_lookup(args[0], command_config_probe_scan);
    sched_del_timer(&ps->time);
    // Wake task to flush remaining events
    if (ps->event_count > 0)
        ps->retransmit_state = RT_PENDING;
    sched_wake_task(&probe_scan_wake);
}
DECL_COMMAND(command_probe_scan_stop, "probe_scan_stop oid=%c");

void
command_probe_scan_ack(uint32_t *args)
{
    struct probe_scan *ps = oid_lookup(args[0], command_config_probe_scan);
    uint8_t count = args[1];
    uint8_t need_wake = 0;
    irq_disable();
    ps->ack_count += count;
    if (count >= ps->event_count) {
        ps->event_count = 0;
        ps->retransmit_state = RT_ACKED;
    } else {
        uint8_t pending = ps->event_count - count;
        uint8_t i;
        for (i = 0; i < pending * EVENT_SIZE; i++)
            ps->events[i] = ps->events[(count * EVENT_SIZE) + i];
        ps->event_count = pending;
        // More data pending — schedule transmission
        ps->retransmit_state = RT_PENDING;
        need_wake = 1;
    }
    irq_enable();

    if (need_wake)
        sched_wake_task(&probe_scan_wake);
}
DECL_COMMAND(command_probe_scan_ack, "probe_scan_ack oid=%c count=%c");

void
probe_scan_task(void)
{
    if (!sched_check_wake(&probe_scan_wake))
        return;
    uint8_t oid;
    struct probe_scan *ps;
    foreach_oid(oid, ps, command_config_probe_scan) {
        if (ps->retransmit_state != RT_PENDING)
            continue;

        irq_disable();
        uint8_t count = ps->event_count;
        uint8_t overflow = ps->overflow;
        if (!count && !overflow) {
            irq_enable();
            continue;
        }
        // Reset overflow after capturing — prevents duplicate reports
        ps->overflow = 0;
        // Set retransmit countdown — if ack doesn't arrive in time,
        // the timer callback will re-wake this task
        ps->retransmit_state = ps->retransmit_count;
        // Copy events under IRQ protection
        uint8_t send_buf[MAX_EVENTS * EVENT_SIZE];
        uint8_t send_bytes = count * EVENT_SIZE;
        uint8_t i;
        for (i = 0; i < send_bytes; i++)
            send_buf[i] = ps->events[i];
        irq_enable();

        sendf("probe_scan_state oid=%c ack_count=%c overflow=%c events=%*s"
              , oid, ps->ack_count, overflow
              , send_bytes, send_buf);
    }
}
DECL_TASK(probe_scan_task);
