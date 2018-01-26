// Report on user interface buttons
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memmove
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio_in
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_is_before
#include "command.h" // DECL_COMMAND
#include "sched.h" // struct timer

#define RETRANSMIT_STATE_TIME timer_from_us(100*1000UL)

struct buttons {
    struct timer time;
    uint32_t rest_ticks;
    struct task_wake wake;
    uint32_t next_report_time;
    uint8_t ack_count, report_count, reports[8];
    uint8_t pressed, last_pressed;
    uint8_t button_count;
    struct gpio_in pins[0];
};

static struct buttons *main_buttons;

static uint_fast8_t
buttons_event(struct timer *t)
{
    struct buttons *b = container_of(t, struct buttons, time);
    uint8_t i, bit, status = 0;
    for (i = 0, bit = 1; i < b->button_count; i++, bit <<= 1) {
        uint8_t val = gpio_in_read(b->pins[i]);
        if (val)
            status |= bit;
    }
    uint8_t diff = status ^ b->pressed;
    if (diff) {
        uint8_t debounced = ~(status ^ b->last_pressed);
        if (diff & debounced) {
            b->pressed = (b->pressed & ~debounced) | (status & debounced);
            if (b->report_count < sizeof(b->reports)) {
                b->reports[b->report_count++] = b->pressed;
                sched_wake_task(&b->wake);
            }
        }
    }
    b->last_pressed = status;
    b->time.waketime += b->rest_ticks;
    return SF_RESCHEDULE;
}

void
command_config_buttons(uint32_t *args)
{
    uint8_t button_count = args[1];
    if (button_count > 8)
        shutdown("Max of 8 buttons");
    if (main_buttons)
        shutdown("buttons already configured");
    struct buttons *b = oid_alloc(
        args[0], command_config_buttons
        , sizeof(*b) + sizeof(b->pins[0]) * button_count);
    b->button_count = button_count;
    b->time.func = buttons_event;
    main_buttons = b;
}
DECL_COMMAND(command_config_buttons, "config_buttons oid=%c button_count=%c");

void
command_buttons_add(uint32_t *args)
{
    struct buttons *b = main_buttons;
    if (!b)
        shutdown("buttons not configured");
    uint8_t pos = args[0];
    if (pos >= b->button_count)
        shutdown("Set button past maximum button count");
    b->pins[pos] = gpio_in_setup(args[1], args[2]);
}
DECL_COMMAND(command_buttons_add, "buttons_add pos=%c pin=%u pull_up=%c");

void
command_buttons_query(uint32_t *args)
{
    struct buttons *b = main_buttons;
    if (!b)
        shutdown("buttons not configured");
    sched_del_timer(&b->time);
    b->time.waketime = args[0];
    b->rest_ticks = args[1];
    b->ack_count = b->report_count = 0;
    if (! b->rest_ticks)
        return;
    sched_add_timer(&b->time);
}
DECL_COMMAND(command_buttons_query, "buttons_query clock=%u rest_ticks=%u");

void
command_buttons_ack(uint32_t *args)
{
    struct buttons *b = main_buttons;
    if (!b)
        shutdown("buttons not configured");
    uint8_t count = args[0];
    b->ack_count += count;
    irq_disable();
    if (count >= b->report_count) {
        b->report_count = 0;
    } else {
        memmove(b->reports, &b->reports[count], b->report_count - count);
        b->report_count -= count;
    }
    irq_enable();
}
DECL_COMMAND(command_buttons_ack, "buttons_ack count=%c");

void
buttons_task(void)
{
    struct buttons *b = main_buttons;
    if (!b)
        return;
    if (!sched_check_wake(&b->wake)) {
        // See if need to retransmit buttons_state
        if (!b->report_count)
            return;
        if (timer_is_before(timer_read_time(), b->next_report_time))
            return;
    }
    sendf("buttons_state ack_count=%c state=%*s"
          , b->ack_count, b->report_count, b->reports);
    b->next_report_time = timer_read_time() + RETRANSMIT_STATE_TIME;
}
DECL_TASK(buttons_task);
