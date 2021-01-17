// Commands for controlling hardware based pulse-width-modulator pins
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio_pwm
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_is_before
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_add_timer

struct pwm_out_s {
    struct timer timer;
    struct gpio_pwm pin;
    uint32_t max_duration;
    uint16_t default_value;
    struct move_queue_head mq;
};

struct pwm_move {
    struct move_node node;
    uint32_t waketime;
    uint16_t value;
};

static uint_fast8_t
pwm_end_event(struct timer *timer)
{
    shutdown("Missed scheduling of next hard pwm event");
}

static uint_fast8_t
pwm_event(struct timer *timer)
{
    // Apply next update and remove it from queue
    struct pwm_out_s *p = container_of(timer, struct pwm_out_s, timer);
    struct move_node *mn = move_queue_pop(&p->mq);
    struct pwm_move *m = container_of(mn, struct pwm_move, node);
    uint16_t value = m->value;
    gpio_pwm_write(p->pin, value);
    move_free(m);

    // Check if more updates queued
    if (move_queue_empty(&p->mq)) {
        if (value == p->default_value || !p->max_duration)
            return SF_DONE;

        // Start the safety timeout
        p->timer.waketime += p->max_duration;
        p->timer.func = pwm_end_event;
        return SF_RESCHEDULE;
    }

    // Schedule next update
    struct move_node *nn = move_queue_first(&p->mq);
    uint32_t wake = container_of(nn, struct pwm_move, node)->waketime;
    if (value != p->default_value && p->max_duration
        && timer_is_before(p->timer.waketime + p->max_duration, wake))
        shutdown("Scheduled pwm event will exceed max_duration");
    p->timer.waketime = wake;
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
    p->timer.func = pwm_event;
    move_queue_setup(&p->mq, sizeof(struct pwm_move));
}
DECL_COMMAND(command_config_pwm_out,
             "config_pwm_out oid=%c pin=%u cycle_ticks=%u value=%hu"
             " default_value=%hu max_duration=%u");

void
command_queue_pwm_out(uint32_t *args)
{
    struct pwm_out_s *p = oid_lookup(args[0], command_config_pwm_out);
    struct pwm_move *m = move_alloc();
    m->waketime = args[1];
    m->value = args[2];

    irq_disable();
    int need_add_timer = move_queue_push(&m->node, &p->mq);
    irq_enable();
    if (!need_add_timer)
        return;

    // queue was empty and a timer needs to be added
    sched_del_timer(&p->timer);
    if (p->timer.func == pwm_end_event
        && timer_is_before(p->timer.waketime, m->waketime))
        shutdown("Scheduled pwm event will exceed max_duration");
    p->timer.func = pwm_event;
    p->timer.waketime = m->waketime;
    sched_add_timer(&p->timer);
}
DECL_COMMAND(command_queue_pwm_out, "queue_pwm_out oid=%c clock=%u value=%hu");

void
pwm_shutdown(void)
{
    uint8_t i;
    struct pwm_out_s *p;
    foreach_oid(i, p, command_config_pwm_out) {
        gpio_pwm_write(p->pin, p->default_value);
        p->timer.func = pwm_event;
        move_queue_clear(&p->mq);
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
