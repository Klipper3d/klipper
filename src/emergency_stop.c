// Host independent emergency stop button
//
// Copyright (C) 2020  Lucas Felix <lucas.felix0738@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio_in
#include "command.h" // DECL_COMMAND
#include "sched.h" // struct timer
#include "board/misc.h" // timer_read_time, timer_from_us

#define MONITOR_TICKS CONFIG_CLOCK_FREQ / 100

struct emergency {
    struct timer time;
    struct gpio_in pin;
    uint8_t prev_val, invert;
};

static struct task_wake emergency_reset_wake;

static uint_fast8_t
emergency_reset_event(struct timer *t)
{
    sched_wake_task(&emergency_reset_wake);
    t->waketime += CONFIG_CLOCK_FREQ / 5;
    return SF_RESCHEDULE;
}

static uint_fast8_t
emergency_check_released_event(struct timer *t)
{
    struct emergency *e = container_of(t, struct emergency, time);
    if(!!(gpio_in_read(e->pin)) == e->invert) {
        t->func = emergency_reset_event;
        output("Emergency button released.");
    }
    t->waketime += CONFIG_CLOCK_FREQ / 100;
    return SF_RESCHEDULE;
}

static uint_fast8_t
emergency_check_pressed_event(struct timer *t)
{
    struct emergency *e = container_of(t, struct emergency, time);
    if(!!(gpio_in_read(e->pin)) != e->invert) {
        shutdown("Emergency button pressed.");
    }
    t->waketime += CONFIG_CLOCK_FREQ / 100;
    return SF_RESCHEDULE;
}

void
command_config_emergency(uint32_t *args)
{
    struct emergency *e = oid_alloc(
        args[0], command_config_emergency, sizeof(*e));
    e->pin = gpio_in_setup(args[1], args[2]);
    e->invert = !!(args[3]);
    e->time.func = emergency_check_pressed_event;

    e->time.waketime = timer_read_time() + CONFIG_CLOCK_FREQ / 100;
    sched_add_timer(&e->time);
}
DECL_COMMAND(command_config_emergency,
             "config_emergency oid=%c pin=%u pull_up=%c invert=%c");

void
emergency_reset_task(void)
{
    if (sched_check_wake(&emergency_reset_wake))
        sendf("emergency_reset");
}
DECL_TASK(emergency_reset_task);

void
emergency_shutdown(void)
{
    uint8_t oid;
    struct emergency *e;
    foreach_oid(oid, e, command_config_emergency) {
        e->time.func = emergency_check_released_event;
        e->time.waketime = timer_read_time() + CONFIG_CLOCK_FREQ / 10;
        sched_add_timer(&e->time);
    }
}
DECL_SHUTDOWN(emergency_shutdown);
