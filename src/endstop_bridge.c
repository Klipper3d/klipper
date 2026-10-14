// MCU-local shared endstop output for digital inputs and analog triggers
//
// Copyright (C) 2026 Xiaoyue Cui <2508041672@qq.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_save
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_add_timer
#include "trsync.h" // trsync_add_signal

enum { EB_IDLE, EB_INPUT, EB_TEST, EB_TRIGGER };

struct endstop_bridge_input;
struct endstop_bridge {
    struct timer timer;
    struct gpio_out output;
    struct endstop_bridge_input *input;
    struct trsync_signal signal;
    uint8_t mode, inactive, value, count;
};

struct endstop_bridge_input {
    struct endstop_bridge *bridge;
    struct gpio_in pin;
    uint32_t period;
    uint8_t active, filter_count;
};

static void
bridge_write(struct endstop_bridge *bridge, uint8_t value)
{
    bridge->value = !!value;
    gpio_out_write(bridge->output, bridge->inactive ^ bridge->value);
}

static uint_fast8_t
bridge_event(struct timer *timer)
{
    struct endstop_bridge *bridge = container_of(
        timer, struct endstop_bridge, timer);
    if (bridge->mode != EB_INPUT)
        return SF_DONE;
    struct endstop_bridge_input *input = bridge->input;
    if (!!gpio_in_read(input->pin) == input->active) {
        if (bridge->count < input->filter_count)
            bridge->count++;
    } else {
        bridge->count = 0;
    }
    bridge_write(bridge, bridge->count >= input->filter_count);
    bridge->timer.waketime += input->period;
    return SF_RESCHEDULE;
}

void
command_config_endstop_bridge(uint32_t *args)
{
    struct endstop_bridge *bridge = oid_alloc(
        args[0], command_config_endstop_bridge, sizeof(*bridge));
    bridge->inactive = !!args[2];
    bridge->output = gpio_out_setup(args[1], bridge->inactive);
    bridge->timer.func = bridge_event;
}
DECL_COMMAND(command_config_endstop_bridge,
             "config_endstop_bridge oid=%c output_pin=%u invert=%c");

void
command_config_endstop_bridge_input(uint32_t *args)
{
    struct endstop_bridge_input *input = oid_alloc(
        args[0], command_config_endstop_bridge_input, sizeof(*input));
    input->bridge = oid_lookup(args[1], command_config_endstop_bridge);
    input->pin = gpio_in_setup(args[2], (int8_t)args[3]);
    input->active = !args[4];
    input->filter_count = args[5];
    input->period = args[6];
    if (!input->filter_count || !input->period
        || input->period >= 0x80000000u)
        shutdown("Invalid Endstop bridging sample interval");
}
DECL_COMMAND(command_config_endstop_bridge_input,
             "config_endstop_bridge_input oid=%c bridge_oid=%c input_pin=%u"
             " pull_up=%c invert=%c filter_count=%c period_ticks=%u");

void
command_set_endstop_bridge(uint32_t *args)
{
    struct endstop_bridge *bridge = oid_lookup(
        args[0], command_config_endstop_bridge);
    uint8_t mode = args[1];
    if (mode > EB_TEST)
        shutdown("Invalid Endstop bridging mode");
    struct endstop_bridge_input *input = NULL;
    if (mode == EB_INPUT) {
        input = oid_lookup(args[2], command_config_endstop_bridge_input);
        if (input->bridge != bridge)
            shutdown("Endstop bridge mismatch");
    }
    irqstatus_t flag = irq_save();
    // A pending analog trigger must be stopped before giving its pin away.
    if (bridge->signal.func && mode != EB_IDLE)
        shutdown("Endstop bridging trigger still armed");
    sched_del_timer(&bridge->timer);
    bridge->mode = mode;
    bridge->input = input;
    bridge->count = 0;
    bridge_write(bridge, mode == EB_TEST && args[3]);
    if (input) {
        bridge->timer.waketime = timer_read_time() + input->period;
        sched_add_timer(&bridge->timer);
    }
    uint8_t value = bridge->value;
    irq_restore(flag);
    sendf("endstop_bridge_state oid=%c mode=%c value=%c",
          args[0], mode, value);
}
DECL_COMMAND(command_set_endstop_bridge,
             "set_endstop_bridge oid=%c mode=%c input_oid=%c value=%c");

static void
bridge_trigger(struct trsync_signal *signal, uint8_t reason)
{
    struct endstop_bridge *bridge = container_of(
        signal, struct endstop_bridge, signal);
    // Errors and local watchdog expiry must stop the receiving MCU too.
    // Keep the level latched until the host has finished both endstops.
    if (bridge->mode == EB_TRIGGER)
        bridge_write(bridge, 1);
}

void
command_arm_endstop_bridge(uint32_t *args)
{
    struct endstop_bridge *bridge = oid_lookup(
        args[0], command_config_endstop_bridge);
    struct trsync *ts = trsync_oid_lookup(args[1]);
    irqstatus_t flag = irq_save();
    if (bridge->mode != EB_IDLE || bridge->signal.func)
        shutdown("Endstop bridge already active");
    bridge->mode = EB_TRIGGER;
    bridge_write(bridge, 0);
    trsync_add_signal(ts, &bridge->signal, bridge_trigger);
    irq_restore(flag);
}
DECL_COMMAND(command_arm_endstop_bridge,
             "arm_endstop_bridge oid=%c trsync_oid=%c");

void
command_query_endstop_bridge_input(uint32_t *args)
{
    struct endstop_bridge_input *input = oid_lookup(
        args[0], command_config_endstop_bridge_input);
    sendf("endstop_bridge_input_state oid=%c value=%c", args[0],
          !!gpio_in_read(input->pin) == input->active);
}
DECL_COMMAND(command_query_endstop_bridge_input,
             "query_endstop_bridge_input oid=%c");

void
endstop_bridge_shutdown(void)
{
    uint8_t oid;
    struct endstop_bridge *bridge;
    foreach_oid(oid, bridge, command_config_endstop_bridge) {
        sched_del_timer(&bridge->timer);
        bridge->mode = EB_IDLE;
        bridge_write(bridge, 1);
    }
}
DECL_SHUTDOWN(endstop_bridge_shutdown);
