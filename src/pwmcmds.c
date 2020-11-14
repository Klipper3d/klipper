// Commands for controlling hardware based pulse-width-modulator pins
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio_pwm
#include "board/irq.h" // irq_disable
#include "command.h" // DECL_COMMAND
#include "queue.h" // init_queue
#include "sched.h" // sched_add_timer

struct pwm_out_s {
    struct timer timer;
    struct gpio_pwm pin;
    uint32_t max_duration;
    uint16_t default_value;
    uint16_t current_value;
    struct mq_list queue;
};

struct pwm_event {
    uint32_t waketime;
    uint16_t value;
    struct mq_event event;
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
    struct mq_event  *c = mq_event_peek(&p->queue);
    struct pwm_event *current = container_of(c, struct pwm_event, event);

    gpio_pwm_write(p->pin, current->value);
    p->current_value = current->value;

    //may be NULL if no further elements are queued
    struct mq_event* next = mq_event_pop(&p->queue);

    if(next) {
        //next event scheduled
        uint32_t next_wt = container_of(c, struct pwm_event, event)->waketime;
        if (current->value != p->default_value || !p->max_duration) {
            if(next_wt > p->timer.waketime + p->max_duration) {
                shutdown("Scheduled pwm event will miss max_duration");
            }
        }
        p->timer.waketime = next_wt;
    } else {
        // no next event scheduled
        if (current->value == p->default_value || !p->max_duration) {
            // We either have set the default value
            // or there is no maximum duration
            irqstatus_t irq = irq_save();
            mq_free_event(current);
            irq_restore(irq);
            return SF_DONE;
        }

        //nothing scheduled, so lets start the safety timeout
        p->timer.waketime += p->max_duration;
        p->timer.func = pwm_end_event;
    }

    irqstatus_t irq = irq_save();
    mq_free_event(current);
    irq_restore(irq);
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
    p->current_value = 0;
    mq_init(&p->queue, sizeof(struct pwm_event));
}
DECL_COMMAND(command_config_pwm_out,
             "config_pwm_out oid=%c pin=%u cycle_ticks=%u value=%hu"
             " default_value=%hu max_duration=%u");

void
command_schedule_pwm_out(uint32_t *args)
{
    struct pwm_out_s *p = oid_lookup(args[0], command_config_pwm_out);
    struct pwm_event* new_event = (struct pwm_event*) mq_alloc_event();
    new_event->waketime = args[1];
    new_event->value = args[2];

    if(!mq_event_insert(&p->queue, &new_event->event)) {
        // queue was empty and a timer needs to be added

        if(p->max_duration && p->current_value != p->default_value) {
            // max_duration and currently not default value
            // There has to be a safety timer
            if(new_event->waketime > p->timer.waketime) {
                shutdown("New pwm event too late for max_duration");
            }
        }

        sched_del_timer(&p->timer);
        p->timer.waketime = new_event->waketime;
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
