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

struct pwm_value {
    uint32_t waketime;
    uint16_t value;
    struct pwm_value *next;
};

struct pwm_out_s {
    struct timer timer;
    struct gpio_pwm pin;
    uint32_t max_duration;
    uint16_t default_value;
    struct pwm_value *first, **plast;
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
    struct pwm_value *v = p->first;

    if(!v)
    {
        // no next pwm value, queue is empty
        // this should not happen because we check the v->next
        return SF_DONE;
    }

    gpio_pwm_write(p->pin, v->value);

    //v->next may be NULL if no further elements are queued
    p->first = v->next;

    if(p->first){
        //next event scheduled
        p->timer.waketime = p->first->waketime;

    } else {
        // no next event scheduled
        if (v->value == p->default_value || !p->max_duration) {
            // We either have set the default value
            // or there is no maximum duration
            irq_disable();
            move_free(v);
            irq_enable();

            return SF_DONE;
        }

        //nothing scheduled, so lets start the safety timeout
        p->timer.waketime += p->max_duration;
        p->timer.func = pwm_end_event;
    }

    irq_disable();
    move_free(v);
    irq_enable();

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
    p->first = NULL;
    p->plast = NULL;

    move_request_size(sizeof(struct pwm_value));
}
DECL_COMMAND(command_config_pwm_out,
             "config_pwm_out oid=%c pin=%u cycle_ticks=%u value=%hu"
             " default_value=%hu max_duration=%u");

void
command_schedule_pwm_out(uint32_t *args)
{

    struct pwm_out_s *p = oid_lookup(args[0], command_config_pwm_out);

    struct pwm_value* v = move_alloc();
    v->waketime = args[1];
    v->value = args[2];
    v->next = NULL;

    //gpio_out_toggle_noirq(p->debug5);

    irq_disable();
    if(p->first) {
        // there exists an element in queue
        //if there is a p->first, there has to be a p->plast
        //if there is no first, plast is invalid.
        *p->plast = v;  //enqueue new v into the last's "next" element
    }
    else {
        // no first element set
        p->first = v;
        p->timer.waketime = args[1];
        p->timer.func = pwm_event;
        sched_add_timer(&p->timer);
    }

    //plast to point to this new element's next pointer
    p->plast = &v->next;
    irq_enable();
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
