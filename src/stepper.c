// Handling of stepper drivers.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stddef.h> // NULL
#include "autoconf.h" // CONFIG_*
#include "basecmd.h" // alloc_oid
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_save
#include "command.h" // DECL_COMMAND
#include "sched.h" // struct timer
#include "stepper.h" // command_config_stepper


/****************************************************************
 * Steppers
 ****************************************************************/

struct stepper {
    struct timer time;
    uint32_t interval;
    int16_t add;
    uint16_t count;
    struct gpio_out step_pin, dir_pin;
    uint32_t position;
    struct move *first, **plast;
    uint32_t min_stop_interval;
    // gcc (pre v6) does better optimization when uint8_t are bitfields
    uint8_t flags : 8;
};

enum { MF_DIR=1 };
enum { SF_LAST_DIR=1, SF_NEXT_DIR=2, SF_INVERT_STEP=4 };

// Setup a stepper for the next move in its queue
static uint8_t
stepper_load_next(struct stepper *s)
{
    struct move *m = s->first;
    if (!m) {
        if (s->interval - s->add < s->min_stop_interval)
            shutdown("No next step");
        s->count = 0;
        return SF_DONE;
    }

    s->interval = m->interval;
    s->time.waketime += s->interval;
    s->add = m->add;
    s->interval += s->add;
    s->count = m->count;
    if (m->flags & MF_DIR) {
        s->position = -s->position + s->count;
        gpio_out_toggle(s->dir_pin);
    } else {
        s->position += s->count;
    }

    s->first = m->next;
    move_free(m);
    return SF_RESCHEDULE;
}

// Timer callback - step the given stepper.
uint8_t
stepper_event(struct timer *t)
{
    struct stepper *s = container_of(t, struct stepper, time);
    gpio_out_toggle(s->step_pin);
    uint16_t count = s->count - 1;
    if (likely(count)) {
        s->count = count;
        s->time.waketime += s->interval;
        s->interval += s->add;
        gpio_out_toggle(s->step_pin);
        return SF_RESCHEDULE;
    }
    uint8_t ret = stepper_load_next(s);
    gpio_out_toggle(s->step_pin);
    return ret;
}

void
command_config_stepper(uint32_t *args)
{
    struct stepper *s = alloc_oid(args[0], command_config_stepper, sizeof(*s));
    if (!CONFIG_INLINE_STEPPER_HACK)
        s->time.func = stepper_event;
    s->flags = args[4] ? SF_INVERT_STEP : 0;
    s->step_pin = gpio_out_setup(args[1], s->flags & SF_INVERT_STEP ? 1 : 0);
    s->dir_pin = gpio_out_setup(args[2], 0);
    s->min_stop_interval = args[3];
    s->position = STEPPER_POSITION_BIAS;
}
DECL_COMMAND(command_config_stepper,
             "config_stepper oid=%c step_pin=%c dir_pin=%c"
             " min_stop_interval=%u invert_step=%c");

// Schedule a set of steps with a given timing
void
command_queue_step(uint32_t *args)
{
    struct stepper *s = lookup_oid(args[0], command_config_stepper);
    struct move *m = move_alloc();
    m->flags = 0;
    if (!!(s->flags & SF_LAST_DIR) != !!(s->flags & SF_NEXT_DIR)) {
        s->flags ^= SF_LAST_DIR;
        m->flags |= MF_DIR;
    }
    m->interval = args[1];
    m->count = args[2];
    if (!m->count)
        shutdown("Invalid count parameter");
    m->add = args[3];
    m->next = NULL;

    uint8_t flag = irq_save();
    if (s->count) {
        if (s->first)
            *s->plast = m;
        else
            s->first = m;
        s->plast = &m->next;
    } else {
        s->first = m;
        stepper_load_next(s);
        sched_timer(&s->time);
    }
    irq_restore(flag);
}
DECL_COMMAND(command_queue_step,
             "queue_step oid=%c interval=%u count=%hu add=%hi");

// Set the direction of the next queued step
void
command_set_next_step_dir(uint32_t *args)
{
    struct stepper *s = lookup_oid(args[0], command_config_stepper);
    s->flags = (s->flags & ~SF_NEXT_DIR) | (args[1] ? SF_NEXT_DIR : 0);
}
DECL_COMMAND(command_set_next_step_dir, "set_next_step_dir oid=%c dir=%c");

// Set an absolute time that the next step will be relative to
void
command_reset_step_clock(uint32_t *args)
{
    struct stepper *s = lookup_oid(args[0], command_config_stepper);
    uint32_t waketime = args[1];
    if (s->count)
        shutdown("Can't reset time when stepper active");
    s->time.waketime = waketime;
}
DECL_COMMAND(command_reset_step_clock, "reset_step_clock oid=%c clock=%u");

// Return the current stepper position.  Caller must disable irqs.
uint32_t
stepper_get_position(struct stepper *s)
{
    uint32_t position = s->position - s->count;
    if (position & 0x80000000)
        return -position;
    return position;
}

// Reset the internal state of a 'struct stepper'
static void
stepper_reset(struct stepper *s)
{
    s->position = stepper_get_position(s);
    s->count = 0;
    s->flags &= SF_INVERT_STEP;
    gpio_out_write(s->dir_pin, 0);
}

// Stop all moves for a given stepper (used in end stop homing).  IRQs
// must be off.
void
stepper_stop(struct stepper *s)
{
    sched_del_timer(&s->time);
    stepper_reset(s);
    while (s->first) {
        struct move *next = s->first->next;
        move_free(s->first);
        s->first = next;
    }
}

static void
stepper_shutdown(void)
{
    uint8_t i;
    struct stepper *s;
    foreach_oid(i, s, command_config_stepper) {
        stepper_reset(s);
        s->first = NULL;
        gpio_out_write(s->step_pin, s->flags & SF_INVERT_STEP ? 1 : 0);
    }
}
DECL_SHUTDOWN(stepper_shutdown);
