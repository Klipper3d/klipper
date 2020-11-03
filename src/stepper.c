// Handling of stepper drivers.
//
// Copyright (C) 2016-2025  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_*
#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_is_before
#include "command.h" // DECL_COMMAND
#include "sched.h" // struct timer
#include "stepper.h" // command_config_stepper
#include <math.h> // sqrt

#define abs_clamp(x, t) (((x) > (t)) ? (t) : (((x) < (-t)) ? (-t) : (x)))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define abs(x) (((x) < 0) ? (-x) : (x))

DECL_CONSTANT("STEPPER_STEP_BOTH_EDGE", 1);

/****************************************************************
 * Steppers
 ****************************************************************/
struct stepper_move {
    struct move_node node;
    uint32_t interval;
    int16_t add;
    uint16_t count;
    uint8_t flags;
};

enum { MF_DIR=1<<0 };

struct speed_mode {
    struct timer update_timer, step_timer;
    uint16_t update_rate;
    uint32_t update_interval, min_freq, max_freq, max_acc;

    int32_t max_delta_freq, freq_limiter;
    int32_t current_speed, target_speed;
    uint32_t current_period;

    int32_t position, min_pos, max_pos;
    // gcc (pre v6) does better optimization when uint8_t are bitfields
    uint8_t flags : 8;
};

enum {
    SM_SLOWING_DOWN=1<<0, SM_DIR_SAVE=1<<1, SM_CUR_DIR=1<<2,
    SM_NEED_UPDATE=1<<3, SM_NEED_SLOWDOWN=1<<4
};

struct stepper {
    struct speed_mode spdm;
    uint16_t steps_per_mm;
    struct timer time, slowdown_timer;
    uint32_t interval;
    int16_t add;
    uint32_t count;
    uint32_t next_step_time, step_pulse_ticks;
    struct gpio_out step_pin, dir_pin;
    uint32_t position;
    struct move_queue_head mq;
    struct trsync_signal stop_signal;
    // gcc (pre v6) does better optimization when uint8_t are bitfields
    uint8_t flags : 8;
};

enum { POSITION_BIAS=0x40000000 };

enum {
    SF_LAST_DIR=1<<0, SF_NEXT_DIR=1<<1, SF_INVERT_STEP=1<<2, SF_HAVE_ADD=1<<3,
    SF_LAST_RESET=1<<4, SF_NO_NEXT_CHECK=1<<5, SF_NEED_RESET=1<<6,
    SF_SPEED_MODE=1<<7
};

static struct task_wake speed_mode_update_wake;
static struct task_wake slowdown_wake;

// Setup a stepper for the next move in its queue
static uint_fast8_t
stepper_load_next(struct stepper *s)
{
    if (move_queue_empty(&s->mq)) {
        // There is no next move - the queue is empty
        s->count = 0;
        return SF_DONE;
    }

    // Read next 'struct stepper_move'
    struct move_node *mn = move_queue_pop(&s->mq);
    struct stepper_move *m = container_of(mn, struct stepper_move, node);
    uint32_t move_interval = m->interval;
    uint_fast16_t move_count = m->count;
    int_fast16_t move_add = m->add;
    uint_fast8_t need_dir_change = m->flags & MF_DIR;
    move_free(m);

    // Add all steps to s->position (stepper_get_position() can calc mid-move)
    s->position = (need_dir_change ? -s->position : s->position) + move_count;

    // Load next move into 'struct stepper'
    s->add = move_add;
    s->interval = move_interval + move_add;
    if (HAVE_EDGE_OPTIMIZATION && s->flags & SF_OPTIMIZED_PATH) {
        // Using optimized stepper_event_edge()
        s->time.waketime += move_interval;
        s->count = move_count;
    } else if (HAVE_AVR_OPTIMIZATION && s->flags & SF_OPTIMIZED_PATH) {
        // Using optimized stepper_event_avr()
        s->time.waketime += move_interval;
        s->count = move_count;
        s->flags = move_add ? s->flags | SF_HAVE_ADD : s->flags & ~SF_HAVE_ADD;
    } else {
        // Using fully scheduled stepper_event_full() code (the scheduler
        // may be called twice for each step)
        uint_fast8_t was_active = !!s->count;
        uint32_t min_next_time = s->time.waketime;
        s->next_step_time += move_interval;
        s->time.waketime = s->next_step_time;
        s->count = (s->flags & SF_SINGLE_SCHED ? move_count
                    : (uint32_t)move_count * 2);
        if (was_active && timer_is_before(s->next_step_time, min_next_time)) {
            // Actively stepping and next step event close to the last unstep
            int32_t diff = s->next_step_time - min_next_time;
            if (diff < (int32_t)-timer_from_us(1000))
                shutdown("Stepper too far in past");
            s->time.waketime = min_next_time;
        }
        s->count = (uint32_t)m->count * 2;
    }
    // Add all steps to s->position (stepper_get_position() can calc mid-move)
    if (m->flags & MF_DIR) {
        s->position = -s->position + m->count;
        if(s->flags & SF_SPEED_MODE) {
            s->spdm.flags ^= SM_DIR_SAVE;
        }
        else {
            gpio_out_toggle_noirq(s->dir_pin);
        }
    } else {
        s->position += m->count;
    }

    // Set new direction (if needed)
    if (need_dir_change)
        gpio_out_toggle_noirq(s->dir_pin);
    return SF_RESCHEDULE;
}

// Edge optimization only enabled when fastest rate notably slower than 100ns
#define EDGE_STEP_TICKS DIV_ROUND_UP(CONFIG_CLOCK_FREQ, 8000000)
#if HAVE_EDGE_OPTIMIZATION
 DECL_CONSTANT("STEPPER_OPTIMIZED_EDGE", EDGE_STEP_TICKS);
#endif

// Optimized step function to step on each step pin edge
static uint_fast8_t
stepper_event_edge(struct timer *t)
{
    struct stepper *s = container_of(t, struct stepper, time);
    gpio_out_toggle_noirq(s->step_pin);
    uint32_t count = s->count - 1;
    if (likely(count)) {
        s->count = count;
        s->time.waketime += s->interval;
        s->interval += s->add;
        return SF_RESCHEDULE;
    }
    return stepper_load_next(s);
}

#define AVR_STEP_TICKS 40 // minimum instructions between step gpio pulses
#if HAVE_AVR_OPTIMIZATION
 DECL_CONSTANT("STEPPER_OPTIMIZED_UNSTEP", AVR_STEP_TICKS);
#endif

// AVR optimized step function
static uint_fast8_t
stepper_event_avr(struct timer *t)
{
    struct stepper *s = container_of(t, struct stepper, time);
    gpio_out_toggle_noirq(s->step_pin);
    uint16_t *pcount = (void*)&s->count, count = *pcount - 1;
    if (likely(count)) {
        *pcount = count;
        s->time.waketime += s->interval;
        gpio_out_toggle_noirq(s->step_pin);
        if (s->flags & SF_HAVE_ADD)
            s->interval += s->add;
        return SF_RESCHEDULE;
    }
    uint_fast8_t ret = stepper_load_next(s);
    gpio_out_toggle_noirq(s->step_pin);
    return ret;
}

// Regular "fully scheduled" step function
static uint_fast8_t
stepper_event_full(struct timer *t)
{
    struct stepper *s = container_of(t, struct stepper, time);
    gpio_out_toggle_noirq(s->step_pin);
    uint32_t curtime = timer_read_time();
    uint32_t min_next_time = curtime + s->step_pulse_ticks;
    uint32_t count = s->count - 1;
    if (likely(count & 1 && !(s->flags & SF_SINGLE_SCHED)))
        // Schedule unstep event
        goto reschedule_min;
    if (likely(count)) {
        s->next_step_time += s->interval;
        s->interval += s->add;
        if (unlikely(timer_is_before(s->next_step_time, min_next_time)))
            // The next step event is too close - push it back
            goto reschedule_min;
        s->count = count;
        s->time.waketime = s->next_step_time;
        return SF_RESCHEDULE;
    }
    s->time.waketime = min_next_time;
    return stepper_load_next(s);
reschedule_min:
    s->count = count;
    s->time.waketime = min_next_time;
    return SF_RESCHEDULE;
}

// Optimized entry point for step function (may be inlined into sched.c code)
uint_fast8_t
stepper_event(struct timer *t)
{
    if (HAVE_EDGE_OPTIMIZATION)
        return stepper_event_edge(t);
    if (HAVE_AVR_OPTIMIZATION)
        return stepper_event_avr(t);
    return stepper_event_full(t);
}

void
command_config_stepper(uint32_t *args)
{
    struct stepper *s = oid_alloc(args[0], command_config_stepper, sizeof(*s));
    int_fast8_t invert_step = args[3];
    if (invert_step > 0)
        s->flags = SF_INVERT_STEP;
    else if (invert_step < 0)
        s->flags = SF_SINGLE_SCHED;
    s->step_pin = gpio_out_setup(args[1], s->flags & SF_INVERT_STEP);
    s->dir_pin = gpio_out_setup(args[2], 0);
    s->position = -POSITION_BIAS;
    s->spdm.flags = 0;
    s->steps_per_mm = args[5]; // This is a rounded value, but it only
                               // slightly affects speed and
                               // acceleration limits.
    move_request_size(sizeof(struct stepper_move));
}
DECL_COMMAND(command_config_stepper,
             "config_stepper oid=%c step_pin=%c dir_pin=%c"
             " min_stop_interval=%u invert_step=%c steps_per_mm=%hu");

void
command_config_stepper_speed_mode(uint32_t *args)
{
    struct stepper *s = stepper_oid_lookup(args[0]);
    s->spdm.update_rate = args[1];
    s->spdm.max_freq    = args[2] * s->steps_per_mm; // from mm.s-1 to step.s-1
    s->spdm.max_acc     = args[3] * s->steps_per_mm; // from mm.s-2 to step.s-2

    s->spdm.update_interval = CONFIG_CLOCK_FREQ / s->spdm.update_rate;
    s->spdm.max_delta_freq  = s->spdm.max_acc / s->spdm.update_rate;

    // to ensure start off is possible, min_freq is never above max_delta_freq
    s->spdm.min_freq = min(100, s->spdm.max_delta_freq);
}
DECL_COMMAND(command_config_stepper_speed_mode,
             "config_stepper_speed_mode oid=%c rate=%hu max_velocity=%u"
             " max_accel=%u");

// Return the 'struct stepper' for a given stepper oid
static struct stepper *
stepper_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_stepper);
}

// Schedule a set of steps with a given timing
void
command_queue_step(uint32_t *args)
{
    struct stepper *s = stepper_oid_lookup(args[0]);
    struct stepper_move *m = move_alloc();
    m->interval = args[1];
    m->count = args[2];
    if (!m->count)
        shutdown("Invalid count parameter");
    m->add = args[3];
    m->flags = 0;

    irq_disable();
    uint8_t flags = s->flags;
    if (!!(flags & SF_LAST_DIR) != !!(flags & SF_NEXT_DIR)) {
        flags ^= SF_LAST_DIR;
        m->flags |= MF_DIR;
    }
    if (s->count) {
        s->flags = flags;
        move_queue_push(&m->node, &s->mq);
    } else if (flags & SF_NEED_RESET) {
        move_free(m);
    } else {
        s->flags = flags;
        move_queue_push(&m->node, &s->mq);
        stepper_load_next(s);
        sched_add_timer(&s->time);
    }
    irq_enable();
}
DECL_COMMAND(command_queue_step,
             "queue_step oid=%c interval=%u count=%hu add=%hi");

// Set the direction of the next queued step
void
command_set_next_step_dir(uint32_t *args)
{
    struct stepper *s = stepper_oid_lookup(args[0]);
    uint8_t nextdir = args[1] ? SF_NEXT_DIR : 0;
    irq_disable();
    s->flags = (s->flags & ~SF_NEXT_DIR) | nextdir;
    irq_enable();
}
DECL_COMMAND(command_set_next_step_dir, "set_next_step_dir oid=%c dir=%c");

// Set an absolute time that the next step will be relative to
void
command_reset_step_clock(uint32_t *args)
{
    struct stepper *s = stepper_oid_lookup(args[0]);
    uint32_t waketime = args[1];
    irq_disable();
    if (s->count)
        shutdown("Can't reset time when stepper active");
    s->next_step_time = s->time.waketime = waketime;
    s->flags &= ~SF_NEED_RESET;
    irq_enable();
}
DECL_COMMAND(command_reset_step_clock, "reset_step_clock oid=%c clock=%u");

// Return the current stepper position.  Caller must disable irqs.
static uint32_t
stepper_get_position(struct stepper *s)
{
    uint32_t position = s->position;
    // If stepper is mid-move, subtract out steps not yet taken
    if (s->flags & SF_SINGLE_SCHED)
        position -= s->count;
    else
        position -= s->count / 2;
    // The top bit of s->position is an optimized reverse direction flag
    if (position & 0x80000000)
        return -position;
    return position;
}

// Report the current position of the stepper
void
command_stepper_get_position(uint32_t *args)
{
    uint8_t oid = args[0];
    struct stepper *s = stepper_oid_lookup(oid);
    irq_disable();
    uint32_t position = stepper_get_position(s);
    irq_enable();
    sendf("stepper_position oid=%c pos=%i", oid, position - POSITION_BIAS);
}
DECL_COMMAND(command_stepper_get_position, "stepper_get_position oid=%c");

void
stepper_set_target_speed(struct stepper* s, int32_t target_speed)
{
        s->spdm.target_speed = target_speed * s->steps_per_mm;
        s->spdm.target_speed = abs_clamp(s->spdm.target_speed,
                                         (int32_t)s->spdm.max_freq);
}

// Return the current stepper position without bias and no matter which mode is
// running. Caller must disable irqs.
int32_t
stepper_position(struct stepper *s)
{
    if(s->flags & SF_SPEED_MODE)
        return s->spdm.position;
    else
        return stepper_get_position(s) - POSITION_BIAS;
}

uint16_t
stepper_speed(struct stepper *s)
{
    return s->count ? CONFIG_CLOCK_FREQ / (s->steps_per_mm * s->interval) : 0;
}

void
speed_mode_update(struct stepper *s)
{
    s->spdm.flags &= ~SM_NEED_UPDATE;

    // apply position based limiter (to avoid stepper max position overrun)
    int32_t dist_to_min = max(0, s->spdm.position - (s->spdm.min_pos + 1));
    int32_t dist_to_max = max(0, (s->spdm.max_pos - 1) - s->spdm.position);
    uint32_t steps_to_stop = pow(s->spdm.max_freq, 2) / (2 * s->spdm.max_acc)
                             + 2 * s->spdm.max_freq / s->spdm.update_rate;

    if(dist_to_min <= steps_to_stop)
    {
        int32_t limit = sqrt((float)s->spdm.max_acc * dist_to_min);
        s->spdm.target_speed = max(s->spdm.target_speed, -limit);
    }
    if(dist_to_max <= steps_to_stop)
    {
        int32_t limit = sqrt((float)s->spdm.max_acc * dist_to_max);
        s->spdm.target_speed = min(s->spdm.target_speed, limit);
    }

    // time based limiter (for slowdown)
    if(s->spdm.flags & SM_SLOWING_DOWN) {
        if(s->spdm.freq_limiter < s->spdm.max_delta_freq) {
            sched_del_timer(&s->spdm.step_timer);
            sched_del_timer(&s->spdm.update_timer);
            if(!!(s->spdm.flags & SM_CUR_DIR) !=
               !!(s->spdm.flags & SM_DIR_SAVE)) {
                gpio_out_toggle_noirq(s->dir_pin);
            }
            if(s->position & 0x80000000) {
                s->position = -(s->spdm.position + POSITION_BIAS) | 0x80000000;
            }
            else {
                s->position = s->spdm.position + POSITION_BIAS;
            }
            s->flags &= ~SF_SPEED_MODE;
            return;
        }
        s->spdm.freq_limiter -= s->spdm.max_delta_freq;
        s->spdm.target_speed = abs_clamp(s->spdm.target_speed,
                                         s->spdm.freq_limiter);
    }

    // compute new reachable speed according to acceleration
    int32_t delta = s->spdm.target_speed - s->spdm.current_speed;
    s->spdm.current_speed += abs_clamp(delta, s->spdm.max_delta_freq);

    // speed is either above min_speed, either null
    if (abs(s->spdm.current_speed) < s->spdm.min_freq) {
        s->spdm.current_speed = 0;
    }

    // store previous direction
    uint8_t prev_dir = s->spdm.current_speed < 0;

    // compute period according to speed, period of zero means no speed
    if(abs(s->spdm.current_speed) > 0) {
        s->spdm.current_period = CONFIG_CLOCK_FREQ / abs(s->spdm.current_speed);
    }
    else {
        s->spdm.current_period = 0;
    }

    irq_disable();
    // possibly apply direction change
    if (!!(prev_dir) != !!(s->spdm.flags & SM_CUR_DIR)) {
        gpio_out_toggle_noirq(s->dir_pin);
        s->spdm.flags ^= SM_CUR_DIR;
    }
    irq_enable();
}

static uint_fast8_t
speed_mode_step_event(struct timer *t)
{
    struct stepper *s = container_of(
                        container_of(t, struct speed_mode, step_timer),
                        struct stepper, spdm);
    if(s->spdm.current_period == 0) {
        t->waketime += s->spdm.update_interval;
    }
    else {
        gpio_out_toggle_noirq(s->step_pin);
        t->waketime += s->spdm.current_period;
        s->spdm.position += (s->spdm.flags & SM_CUR_DIR) ? -1 : 1;
        gpio_out_toggle_noirq(s->step_pin);
    }
    return SF_RESCHEDULE;
}

static uint_fast8_t
speed_mode_update_event(struct timer *t)
{
    struct stepper *s = container_of(
                        container_of(t, struct speed_mode, update_timer),
                        struct stepper, spdm);
    t->waketime += s->spdm.update_interval;
    s->spdm.flags |= SM_NEED_UPDATE;
    sched_wake_task(&speed_mode_update_wake);
    return SF_RESCHEDULE;
}

uint_fast8_t
slowdown_event(struct timer *t)
{
    struct stepper *s = container_of(t, struct stepper, slowdown_timer);
    s->spdm.flags |= SM_NEED_SLOWDOWN;
    sched_wake_task(&slowdown_wake);
    return SF_DONE;
}

void
stepper_schedule_position_mode(struct stepper *s, uint32_t clock)
{
    uint32_t slowdown_duration = s->spdm.update_interval *
                                 (s->spdm.max_freq / s->spdm.max_delta_freq);

    // start slowing down now
    if(timer_is_before(clock - slowdown_duration, timer_read_time())) {
        uint32_t rest_time = clock - timer_read_time();
        s->spdm.freq_limiter = s->spdm.max_delta_freq * rest_time /
                               s->spdm.update_interval;
        s->spdm.flags |= SM_SLOWING_DOWN;
    }
    // set a timer for later
    else {
        sched_del_timer(&s->slowdown_timer);
        s->slowdown_timer.waketime = clock - slowdown_duration;
        s->slowdown_timer.func = slowdown_event;
        sched_add_timer(&s->slowdown_timer);
    }
}

void
stepper_set_speed_mode(struct stepper *s, int32_t min_pos, int32_t max_pos)
{
        s->spdm.min_pos = min_pos;
        s->spdm.max_pos = max_pos;

        // save direction from position mode
        if(!(s->flags & SF_LAST_DIR)) {
            gpio_out_toggle_noirq(s->dir_pin);
            s->spdm.flags |= SM_DIR_SAVE;
        }
        else {
            s->spdm.flags &= ~SM_DIR_SAVE;
        }
        s->spdm.flags &= ~(SM_CUR_DIR | SM_SLOWING_DOWN);
        s->spdm.position = stepper_get_position(s) - POSITION_BIAS;
        s->spdm.current_period = 0;
        s->spdm.current_speed  = 0;
        s->spdm.target_speed   = 0;

        uint32_t now = timer_read_time();
        sched_wake_task(&speed_mode_update_wake);
        s->spdm.update_timer.func = speed_mode_update_event;
        s->spdm.update_timer.waketime = now + s->spdm.update_interval;
        sched_add_timer(&s->spdm.update_timer);

        s->spdm.step_timer.func = speed_mode_step_event;
        s->spdm.step_timer.waketime = now + timer_from_us(200);
        sched_add_timer(&s->spdm.step_timer);

        s->flags |= SF_SPEED_MODE | SM_NEED_UPDATE;
}

// Stop all moves for a given stepper (used in end stop homing).  IRQs
// must be off.
void
stepper_stop(struct stepper *s)
{
    struct stepper *s = container_of(tss, struct stepper, stop_signal);
    sched_del_timer(&s->time);
    s->next_step_time = s->time.waketime = 0;
    s->position = -stepper_get_position(s);
    s->count = 0;
    s->flags = ((s->flags & (SF_INVERT_STEP|SF_SINGLE_SCHED|SF_OPTIMIZED_PATH))
                | SF_NEED_RESET);
    gpio_out_write(s->dir_pin, 0);
    if (!(s->flags & SF_SINGLE_SCHED)
        || (HAVE_AVR_OPTIMIZATION && s->flags & SF_OPTIMIZED_PATH))
        // Must return step pin to "unstep" state
        gpio_out_write(s->step_pin, s->flags & SF_INVERT_STEP);
    while (!move_queue_empty(&s->mq)) {
        struct move_node *mn = move_queue_pop(&s->mq);
        struct stepper_move *m = container_of(mn, struct stepper_move, node);
        move_free(m);
    }
}

// Set the stepper to stop on a "trigger event" (used in homing)
void
command_stepper_stop_on_trigger(uint32_t *args)
{
    struct stepper *s = stepper_oid_lookup(args[0]);
    struct trsync *ts = trsync_oid_lookup(args[1]);
    trsync_add_signal(ts, &s->stop_signal, stepper_stop);
}
DECL_COMMAND(command_stepper_stop_on_trigger,
             "stepper_stop_on_trigger oid=%c trsync_oid=%c");

void
slowdown_task(void)
{
    if (!sched_check_wake(&slowdown_wake))
        return;

    uint8_t i;
    struct stepper *s;
    foreach_oid(i, s, command_config_stepper) {
        if(s->spdm.flags & SM_NEED_SLOWDOWN) {
            s->spdm.freq_limiter = s->spdm.max_freq;
            s->spdm.flags |= SM_SLOWING_DOWN;
            s->spdm.flags &= ~SM_NEED_SLOWDOWN;
        }
    }
}
DECL_TASK(slowdown_task);

void
speed_mode_update_task(void)
{
    if (!sched_check_wake(&speed_mode_update_wake))
        return;

    uint8_t i;
    struct stepper *s;
    foreach_oid(i, s, command_config_stepper) {
        if((s->flags & SF_SPEED_MODE) && (s->spdm.flags & SM_NEED_UPDATE))
            speed_mode_update(s);
    }
}
DECL_TASK(speed_mode_update_task);

void
stepper_shutdown(void)
{
    uint8_t i;
    struct stepper *s;
    foreach_oid(i, s, command_config_stepper) {
        move_queue_clear(&s->mq);
        stepper_stop(&s->stop_signal, 0);
    }
}
DECL_SHUTDOWN(stepper_shutdown);
