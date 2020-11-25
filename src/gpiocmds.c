// Commands for controlling GPIO pins
//
// Copyright (C) 2016-2020  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio_out
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_is_before
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_add_timer


/****************************************************************
 * Digital out pins
 ****************************************************************/

struct digital_out_s {
    struct timer timer;
    struct gpio_out pin;
    uint32_t max_duration;
    uint8_t default_value;
    struct move_queue_head mq;
};

struct digital_move {
    struct move_node node;
    uint32_t waketime;
    uint8_t value;
};

static uint_fast8_t
digital_end_event(struct timer *timer)
{
    shutdown("Missed scheduling of next pin event");
}

static uint_fast8_t
digital_out_event(struct timer *timer)
{
    // Apply next update and remove it from queue
    struct digital_out_s *d = container_of(timer, struct digital_out_s, timer);
    struct move_node *mn = move_queue_pop(&d->mq);
    struct digital_move *m = container_of(mn, struct digital_move, node);
    uint8_t value = m->value;
    gpio_out_write(d->pin, value);
    move_free(m);

    // Check if more updates queued
    if (move_queue_empty(&d->mq)) {
        if (value == d->default_value || !d->max_duration)
            return SF_DONE;

        // Start the safety timeout
        d->timer.waketime += d->max_duration;
        d->timer.func = digital_end_event;
        return SF_RESCHEDULE;
    }

    // Schedule next update
    struct move_node *nn = move_queue_first(&d->mq);
    uint32_t wake = container_of(nn, struct digital_move, node)->waketime;
    if (value != d->default_value && d->max_duration
        && timer_is_before(d->timer.waketime + d->max_duration, wake))
        shutdown("Scheduled digital out event will exceed max_duration");
    d->timer.waketime = wake;
    return SF_RESCHEDULE;
}

void
command_config_digital_out(uint32_t *args)
{
    struct gpio_out pin = gpio_out_setup(args[1], args[2]);
    struct digital_out_s *d = oid_alloc(args[0], command_config_digital_out
                                        , sizeof(*d));
    d->pin = pin;
    d->default_value = args[3];
    d->max_duration = args[4];
    d->timer.func = digital_out_event;
    move_queue_setup(&d->mq, sizeof(struct digital_move));
}
DECL_COMMAND(command_config_digital_out,
             "config_digital_out oid=%c pin=%u value=%c default_value=%c"
             " max_duration=%u");

void
command_queue_digital_out(uint32_t *args)
{
    struct digital_out_s *d = oid_lookup(args[0], command_config_digital_out);
    struct digital_move *m = move_alloc();
    m->waketime = args[1];
    m->value = args[2];

    irq_disable();
    int need_add_timer = move_queue_push(&m->node, &d->mq);
    irq_enable();
    if (!need_add_timer)
        return;

    // queue was empty and a timer needs to be added
    sched_del_timer(&d->timer);
    if (d->timer.func == digital_end_event
        && timer_is_before(d->timer.waketime, m->waketime))
        shutdown("Scheduled digital out event will exceed max_duration");
    d->timer.func = digital_out_event;
    d->timer.waketime = m->waketime;
    sched_add_timer(&d->timer);
}
DECL_COMMAND(command_queue_digital_out,
             "queue_digital_out oid=%c clock=%u value=%c");

void
command_update_digital_out(uint32_t *args)
{
    struct digital_out_s *d = oid_lookup(args[0], command_config_digital_out);
    sched_del_timer(&d->timer);
    if (!move_queue_empty(&d->mq))
        shutdown("update_digital_out not valid with active queue");
    uint8_t value = args[1];
    gpio_out_write(d->pin, value);
    if (value != d->default_value && d->max_duration) {
        d->timer.waketime = timer_read_time() + d->max_duration;
        d->timer.func = digital_end_event;
        sched_add_timer(&d->timer);
    }
}
DECL_COMMAND(command_update_digital_out, "update_digital_out oid=%c value=%c");

void
digital_out_shutdown(void)
{
    uint8_t i;
    struct digital_out_s *d;
    foreach_oid(i, d, command_config_digital_out) {
        gpio_out_write(d->pin, d->default_value);
        d->timer.func = digital_out_event;
        move_queue_clear(&d->mq);
    }
}
DECL_SHUTDOWN(digital_out_shutdown);

void
command_set_digital_out(uint32_t *args)
{
    gpio_out_setup(args[0], args[1]);
}
DECL_COMMAND(command_set_digital_out, "set_digital_out pin=%u value=%c");


/****************************************************************
 * Soft PWM output pins
 ****************************************************************/

struct soft_pwm_s {
    struct timer timer;
    uint32_t on_duration, off_duration, end_time;
    struct gpio_out pin;
    uint32_t max_duration, cycle_time;
    struct move_queue_head mq;
    uint8_t flags;
};

struct soft_pwm_move {
    struct move_node node;
    uint32_t waketime, on_duration;
};

enum {
    SPF_ON=1<<0, SPF_TOGGLING=1<<1, SPF_CHECK_END=1<<2, SPF_DEFAULT_ON=1<<4
};

static uint_fast8_t soft_pwm_load_event(struct timer *timer);

// Normal pulse change event
static uint_fast8_t
soft_pwm_toggle_event(struct timer *timer)
{
    struct soft_pwm_s *s = container_of(timer, struct soft_pwm_s, timer);
    gpio_out_toggle_noirq(s->pin);
    s->flags ^= SPF_ON;
    uint32_t waketime = s->timer.waketime;
    if (s->flags & SPF_ON)
        waketime += s->on_duration;
    else
        waketime += s->off_duration;
    if (s->flags & SPF_CHECK_END && !timer_is_before(waketime, s->end_time)) {
        // End of normal pulsing - next event loads new pwm settings
        s->timer.func = soft_pwm_load_event;
        waketime = s->end_time;
    }
    s->timer.waketime = waketime;
    return SF_RESCHEDULE;
}

// Load next pwm settings
static uint_fast8_t
soft_pwm_load_event(struct timer *timer)
{
    // Apply next update and remove it from queue
    struct soft_pwm_s *s = container_of(timer, struct soft_pwm_s, timer);
    if (move_queue_empty(&s->mq))
        shutdown("Missed scheduling of next pwm event");
    struct move_node *mn = move_queue_pop(&s->mq);
    struct soft_pwm_move *m = container_of(mn, struct soft_pwm_move, node);
    uint32_t on_duration = m->on_duration;
    uint8_t flags = on_duration ? SPF_ON : 0;
    gpio_out_write(s->pin, flags);
    move_free(m);

    // Calculate next end_time and flags
    uint32_t end_time = 0;
    if (!flags || on_duration >= s->cycle_time) {
        // Pin is in an always on or always off state
        if (!flags != !(s->flags & SPF_DEFAULT_ON) && s->max_duration) {
            end_time = s->timer.waketime + s->max_duration;
            flags |= SPF_CHECK_END;
        }
    } else {
        flags |= SPF_TOGGLING;
        if (s->max_duration) {
            end_time = s->timer.waketime + s->max_duration;
            flags |= SPF_CHECK_END;
        }
    }
    if (!move_queue_empty(&s->mq)) {
        struct move_node *nn = move_queue_first(&s->mq);
        uint32_t wake = container_of(nn, struct soft_pwm_move, node)->waketime;
        if (flags & SPF_CHECK_END && timer_is_before(end_time, wake))
            shutdown("Scheduled soft pwm event will exceed max_duration");
        end_time = wake;
        flags |= SPF_CHECK_END;
    }
    s->end_time = end_time;
    s->flags = flags | (s->flags & SPF_DEFAULT_ON);

    // Schedule next event
    if (!(flags & SPF_TOGGLING)) {
        if (!(flags & SPF_CHECK_END))
            // Pin not toggling and nothing scheduled
            return SF_DONE;
        s->timer.waketime = end_time;
        return SF_RESCHEDULE;
    }
    uint32_t waketime = s->timer.waketime + on_duration;
    if (flags & SPF_CHECK_END && !timer_is_before(waketime, end_time)) {
        s->timer.waketime = end_time;
        return SF_RESCHEDULE;
    }
    s->timer.func = soft_pwm_toggle_event;
    s->timer.waketime = waketime;
    s->on_duration = on_duration;
    s->off_duration = s->cycle_time - on_duration;
    return SF_RESCHEDULE;
}

void
command_config_soft_pwm_out(uint32_t *args)
{
    struct gpio_out pin = gpio_out_setup(args[1], !!args[2]);
    struct soft_pwm_s *s = oid_alloc(args[0], command_config_soft_pwm_out
                                     , sizeof(*s));
    s->pin = pin;
    s->flags = (args[2] ? SPF_ON : 0) | (args[3] ? SPF_DEFAULT_ON : 0);
    s->max_duration = args[4];
    move_queue_setup(&s->mq, sizeof(struct soft_pwm_move));
}
DECL_COMMAND(command_config_soft_pwm_out,
             "config_soft_pwm_out oid=%c pin=%u value=%c"
             " default_value=%c max_duration=%u");

void
command_set_soft_pwm_cycle_ticks(uint32_t *args)
{
    struct soft_pwm_s *s = oid_lookup(args[0], command_config_soft_pwm_out);
    irq_disable();
    if (!move_queue_empty(&s->mq))
        shutdown("Can not set soft pwm cycle ticks while updates pending");
    s->cycle_time = args[1];
    irq_enable();
}
DECL_COMMAND(command_set_soft_pwm_cycle_ticks,
             "set_soft_pwm_cycle_ticks oid=%c cycle_ticks=%u");

void
command_queue_soft_pwm_out(uint32_t *args)
{
    struct soft_pwm_s *s = oid_lookup(args[0], command_config_soft_pwm_out);
    struct soft_pwm_move *m = move_alloc();
    uint32_t time = m->waketime = args[1];
    m->on_duration = args[2];

    irq_disable();
    int first_on_queue = move_queue_push(&m->node, &s->mq);
    if (!first_on_queue) {
        irq_enable();
        return;
    }
    uint8_t flags = s->flags;
    if (flags & SPF_CHECK_END && timer_is_before(s->end_time, time))
        shutdown("Scheduled soft pwm event will exceed max_duration");
    s->end_time = time;
    s->flags = flags | SPF_CHECK_END;
    if (flags & SPF_TOGGLING && timer_is_before(s->timer.waketime, time)) {
        // soft_pwm_toggle_event() will schedule a load event when ready
    } else {
        // Schedule the loading of the pwm parameters at the requested time
        sched_del_timer(&s->timer);
        s->timer.waketime = time;
        s->timer.func = soft_pwm_load_event;
        sched_add_timer(&s->timer);
    }
    irq_enable();
}
DECL_COMMAND(command_queue_soft_pwm_out,
             "queue_soft_pwm_out oid=%c clock=%u on_ticks=%u");

void
soft_pwm_shutdown(void)
{
    uint8_t i;
    struct soft_pwm_s *s;
    foreach_oid(i, s, command_config_soft_pwm_out) {
        gpio_out_write(s->pin, s->flags & SPF_DEFAULT_ON);
        s->flags = s->flags & SPF_DEFAULT_ON ? SPF_ON | SPF_DEFAULT_ON : 0;
        move_queue_clear(&s->mq);
    }
}
DECL_SHUTDOWN(soft_pwm_shutdown);
