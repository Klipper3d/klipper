// Commands for controlling hardware based pulse-width-modulator pins
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio_pwm
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_add_timer
#include "gpioqueue.h" // init_queue

struct pwm_out_s {
    struct timer timer;
    struct gpio_pwm pin;
    uint32_t max_duration;
    uint16_t default_value;
    struct gpio_queue queue;
};

static uint_fast8_t
pwm_end_event(struct timer *timer)
{
    shutdown("Missed scheduling of next hard pwm event");
}

static uint_fast8_t
pwm_event(struct timer *timer)
{
    struct pwm_out_s *p = container_of(timer, struct pwm_out_s, timer);
    struct gpio_event *current = get_current_event(&p->queue);

    if(!current) {
        // no pwm value, queue is empty
        // this should not happen because we were scheduled for an event
        return SF_DONE;
    }

    gpio_pwm_write(p->pin, current->value);

    //may be NULL if no further elements are queued
    struct gpio_event *next = get_next_event(&p->queue);

    if(next) {
        //next event scheduled
        p->timer.waketime = next->waketime;

    } else {
        // no next event scheduled
        if (current->value == p->default_value || !p->max_duration) {
            // We either have set the default value
            // or there is no maximum duration
            free_gpio_event(current);
            return SF_DONE;
        }

        //nothing scheduled, so lets start the safety timeout
        p->timer.waketime += p->max_duration;
        p->timer.func = pwm_end_event;
    }

    free_gpio_event(current);
    return SF_RESCHEDULE;
}

void
command_config_pwm_out(uint32_t *args)
{
    struct gpio_pwm pin = gpio_pwm_setup(args[1], args[2], args[3]);
    struct pwm_out_s *p = oid_alloc(args[0], command_config_pwm_out
                                    , sizeof(*p));
    p->pin = pin;
    p->default_value = args[4];
    p->max_duration = args[5];
    init_queue(&p->queue);
}
DECL_COMMAND(command_config_pwm_out,
             "config_pwm_out oid=%c pin=%u cycle_ticks=%u value=%hu"
             " default_value=%hu max_duration=%u");

void
command_schedule_pwm_out(uint32_t *args)
{
    struct pwm_out_s *p = oid_lookup(args[0], command_config_pwm_out);

    if(!insert_gpio_event(&p->queue, args[1], args[2])) {
        //queue was empty and a timer needs to be added
        p->timer.waketime = args[1];
        p->timer.func = pwm_event;
        sched_add_timer(&p->timer);
    }
}
DECL_COMMAND(command_schedule_pwm_out,
             "schedule_pwm_out oid=%c clock=%u value=%hu");

void
pwm_shutdown(void)
{
    uint8_t i;
    struct pwm_out_s *p;
    foreach_oid(i, p, command_config_pwm_out) {
        gpio_pwm_write(p->pin, p->default_value);
    }
}
DECL_SHUTDOWN(pwm_shutdown);

void
command_set_pwm_out(uint32_t *args)
{
    gpio_pwm_setup(args[0], args[1], args[2]);
}
DECL_COMMAND(command_set_pwm_out,
             "set_pwm_out pin=%u cycle_ticks=%u value=%hu");
