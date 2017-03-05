// Handling of end stops.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_COMMAND
#include "sched.h" // struct timer
#include "stepper.h" // stepper_stop

struct end_stop {
    struct timer time;
    uint32_t rest_time;
    struct stepper *stepper;
    struct gpio_in pin;
    uint8_t flags;
};

enum { ESF_PIN_HIGH=1<<0, ESF_HOMING=1<<1, ESF_REPORT=1<<2 };

// Timer callback for an end stop
static uint_fast8_t
end_stop_event(struct timer *t)
{
    struct end_stop *e = container_of(t, struct end_stop, time);
    uint8_t val = gpio_in_read(e->pin);
    if ((val ? ~e->flags : e->flags) & ESF_PIN_HIGH) {
        // No match - reschedule for the next attempt
        e->time.waketime += e->rest_time;
        return SF_RESCHEDULE;
    }
    // Stop stepper
    e->flags = ESF_REPORT;
    stepper_stop(e->stepper);
    return SF_DONE;
}

void
command_config_end_stop(uint32_t *args)
{
    struct end_stop *e = oid_alloc(args[0], command_config_end_stop, sizeof(*e));
    struct stepper *s = oid_lookup(args[3], command_config_stepper);
    e->time.func = end_stop_event;
    e->stepper = s;
    e->pin = gpio_in_setup(args[1], args[2]);
}
DECL_COMMAND(command_config_end_stop,
             "config_end_stop oid=%c pin=%c pull_up=%c stepper_oid=%c");

// Home an axis
void
command_end_stop_home(uint32_t *args)
{
    struct end_stop *e = oid_lookup(args[0], command_config_end_stop);
    sched_del_timer(&e->time);
    e->time.waketime = args[1];
    e->rest_time = args[2];
    if (!e->rest_time) {
        // Disable end stop checking
        e->flags = 0;
        return;
    }
    e->flags = ESF_HOMING | (args[3] ? ESF_PIN_HIGH : 0);
    sched_timer(&e->time);
}
DECL_COMMAND(command_end_stop_home,
             "end_stop_home oid=%c clock=%u rest_ticks=%u pin_value=%c");

static void
end_stop_report(uint8_t oid, struct end_stop *e)
{
    irq_disable();
    uint32_t position = stepper_get_position(e->stepper);
    uint8_t eflags = e->flags;
    e->flags &= ~ESF_REPORT;
    irq_enable();

    sendf("end_stop_state oid=%c homing=%c pin=%c pos=%i"
          , oid, !!(eflags & ESF_HOMING), gpio_in_read(e->pin)
          , position - STEPPER_POSITION_BIAS);
}

void
command_end_stop_query(uint32_t *args)
{
    uint8_t oid = args[0];
    struct end_stop *e = oid_lookup(oid, command_config_end_stop);
    end_stop_report(oid, e);
}
DECL_COMMAND(command_end_stop_query, "end_stop_query oid=%c");

static void
end_stop_task(void)
{
    static uint16_t next;
    if (!sched_check_periodic(50, &next))
        return;
    uint8_t oid;
    struct end_stop *e;
    foreach_oid(oid, e, command_config_end_stop) {
        if (!(e->flags & ESF_REPORT))
            continue;
        end_stop_report(oid, e);
    }
}
DECL_TASK(end_stop_task);
