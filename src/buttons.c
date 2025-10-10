// Report on user interface buttons
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio_in
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_COMMAND
#include "sched.h" // struct timer

struct buttons {
    struct timer time;
    uint32_t rest_ticks;
    uint8_t pressed, last_pressed;
    uint8_t report_count, reports[8];
    uint8_t ack_count, retransmit_state, retransmit_count;
    uint8_t button_count;
    struct gpio_in pins[0];
};

enum { BF_NO_RETRANSMIT = 0x80, BF_PENDING = 0xff, BF_ACKED = 0xfe };

static struct task_wake buttons_wake;

static uint_fast8_t
buttons_event(struct timer *t)
{
    struct buttons *b = container_of(t, struct buttons, time);

    // Read pins
    uint8_t i, bit, status = 0;
    for (i = 0, bit = 1; i < b->button_count; i++, bit <<= 1) {
        uint8_t val = gpio_in_read(b->pins[i]);
        if (val)
            status |= bit;
    }

    // Check if any pins have changed since last time
    uint8_t diff = status ^ b->pressed;
    if (diff) {
        // At least one pin has changed - do button debouncing
        uint8_t debounced = ~(status ^ b->last_pressed);
        if (diff & debounced) {
            // Pin has been consistently different - report it
            b->pressed = (b->pressed & ~debounced) | (status & debounced);
            if (b->report_count < sizeof(b->reports)) {
                b->reports[b->report_count++] = b->pressed;
                sched_wake_task(&buttons_wake);
                b->retransmit_state = BF_PENDING;
            }
        }
    }
    b->last_pressed = status;

    // Check if a retransmit is needed
    uint8_t retransmit_state = b->retransmit_state;
    if (!(retransmit_state & BF_NO_RETRANSMIT)) {
        retransmit_state--;
        if (retransmit_state & BF_NO_RETRANSMIT)
            // timeout - do retransmit
            sched_wake_task(&buttons_wake);
        b->retransmit_state = retransmit_state;
    }

    // Reschedule timer
    b->time.waketime += b->rest_ticks;
    return SF_RESCHEDULE;
}

void
command_config_buttons(uint32_t *args)
{
    uint8_t button_count = args[1];
    if (button_count > 8)
        shutdown("Max of 8 buttons");
    struct buttons *b = oid_alloc(
        args[0], command_config_buttons
        , sizeof(*b) + sizeof(b->pins[0]) * button_count);
    b->button_count = button_count;
    b->time.func = buttons_event;
}
DECL_COMMAND(command_config_buttons, "config_buttons oid=%c button_count=%c");

void
command_buttons_add(uint32_t *args)
{
    struct buttons *b = oid_lookup(args[0], command_config_buttons);
    uint8_t pos = args[1];
    if (pos >= b->button_count)
        shutdown("Set button past maximum button count");
    b->pins[pos] = gpio_in_setup(args[2], args[3]);
}
DECL_COMMAND(command_buttons_add,
             "buttons_add oid=%c pos=%c pin=%u pull_up=%c");

void
command_buttons_query(uint32_t *args)
{
    struct buttons *b = oid_lookup(args[0], command_config_buttons);
    sched_del_timer(&b->time);
    b->time.waketime = args[1];
    b->rest_ticks = args[2];
    b->pressed = b->last_pressed = args[4];
    b->ack_count = b->report_count = 0;
    b->retransmit_state = BF_ACKED;
    b->retransmit_count = args[3];
    if (b->retransmit_count >= BF_NO_RETRANSMIT)
        shutdown("Invalid buttons retransmit count");
    if (! b->rest_ticks)
        return;
    sched_add_timer(&b->time);
}
DECL_COMMAND(command_buttons_query,
             "buttons_query oid=%c clock=%u rest_ticks=%u retransmit_count=%c"
             " invert=%c");

void
command_buttons_ack(uint32_t *args)
{
    struct buttons *b = oid_lookup(args[0], command_config_buttons);
    uint8_t count = args[1];
    b->ack_count += count;
    irq_disable();
    if (count >= b->report_count) {
        b->report_count = 0;
        b->retransmit_state = BF_ACKED;
    } else {
        uint8_t pending = b->report_count - count, i;
        for (i=0; i<pending; i++)
            b->reports[i] = b->reports[i+count];
        b->report_count = pending;
    }
    irq_enable();
}
DECL_COMMAND(command_buttons_ack, "buttons_ack oid=%c count=%c");

void
buttons_task(void)
{
    if (!sched_check_wake(&buttons_wake))
        return;
    uint8_t oid;
    struct buttons *b;
    foreach_oid(oid, b, command_config_buttons) {
        // See if need to transmit buttons_state
        if (b->retransmit_state != BF_PENDING)
            continue;
        // Generate message
        irq_disable();
        uint8_t report_count = b->report_count;
        if (!report_count) {
            irq_enable();
            continue;
        }
        b->retransmit_state = b->retransmit_count;
        irq_enable();
        sendf("buttons_state oid=%c ack_count=%c state=%*s"
              , oid, b->ack_count, report_count, b->reports);
    }
}
DECL_TASK(buttons_task);
