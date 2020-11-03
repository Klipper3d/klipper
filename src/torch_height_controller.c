// Support for torch height controller
//
// Copyright (C) 2020  Lucas Felix <lucas.felix0738@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_*
#include "board/gpio.h" // gpio_out_write
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // struct timer
#include "stepper.h"
#include <math.h> // pow

#define SESSION_BUFFER_SIZE 2

static inline int16_t div_pow2_16(int16_t x, uint8_t n) {
    return (x + ((x >> 15) & ((1 << n) + ~0))) >> n;
}
static inline int32_t div_pow2_32(int32_t x, uint8_t n) {
    return (x + ((x >> 31) & ((1 << n) + ~0))) >> n;
}

struct thc_session {
    uint8_t x_stepper_oid, y_stepper_oid, z_stepper_oid, has_end;
    uint32_t begin, end, threshold;
    int32_t target_mv, speed_coeff, min_pos, max_pos;
};

struct thc {
    struct timer update_timer, toggle_timer;
    struct stepper *x_stepper, *y_stepper, *z_stepper;
    struct i2c_config i2c_config;
    uint8_t flags;

    uint32_t update_interval, threshold;
    int32_t speed_coeff, target_mv, a_coeff, b_coeff;

    struct thc_session sbuf[SESSION_BUFFER_SIZE];
    uint8_t sbuf_current, sbuf_size;
};

enum { THC_ACTIVE=1<<0, THC_NEED_TOGGLE=1<<1, THC_NEED_UPDATE=1<<2 };

static struct task_wake thc_update_wake, thc_toggle_wake;
void schedule_start(struct thc*, struct thc_session*);
void schedule_stop(struct thc*, struct thc_session*);

static uint_fast8_t
thc_update_event(struct timer *t)
{
    struct thc *thc = container_of(t, struct thc, update_timer);
    t->waketime += thc->update_interval;
    thc->flags |= THC_NEED_UPDATE;
    sched_wake_task(&thc_update_wake);
    return SF_RESCHEDULE;
}

uint_fast8_t
toggle_event(struct timer *t)
{
    struct thc *thc = container_of(t, struct thc, toggle_timer);
    thc->flags |= THC_NEED_TOGGLE;
    sched_wake_task(&thc_toggle_wake);
    return SF_DONE;
}

void
thc_toggle(struct thc* thc)
{
    if(thc->flags & THC_ACTIVE) { // stop
        // stepper stop on his own as its slowdown has been scheduled
        sched_del_timer(&thc->update_timer);
        thc->sbuf_current = (thc->sbuf_current + 1) % SESSION_BUFFER_SIZE;
        thc->sbuf_size--;
        thc->flags &= ~THC_ACTIVE;
        if (thc->sbuf_size){ // schedule next start
            schedule_start(thc, &thc->sbuf[thc->sbuf_current]);
            sched_add_timer(&thc->toggle_timer);
        }
        else {
            thc->toggle_timer.func = NULL; // tells there is no pending toggle
        }
    }
    else { // start
        struct thc_session *session = &thc->sbuf[thc->sbuf_current];
        stepper_set_speed_mode(thc->z_stepper, session->min_pos,
                               session->max_pos);

        thc->update_timer.func = thc_update_event;
        thc->update_timer.waketime = thc->toggle_timer.waketime +
                                     thc->update_interval;
        sched_add_timer(&thc->update_timer);

        thc->flags |=  THC_ACTIVE | THC_NEED_UPDATE;
        sched_wake_task(&thc_update_wake);

        if (session->has_end) { // schedule next stop
            schedule_stop(thc, session);
            sched_add_timer(&thc->toggle_timer);
        }
        else {
            thc->toggle_timer.func = NULL; // tells there is no pending toggle
        }
    }
}

void
schedule_start(struct thc* thc, struct thc_session* session)
{
    thc->x_stepper   = stepper_oid_lookup(session->x_stepper_oid);
    thc->y_stepper   = stepper_oid_lookup(session->y_stepper_oid);
    thc->z_stepper   = stepper_oid_lookup(session->z_stepper_oid);
    thc->target_mv   = session->target_mv;
    thc->speed_coeff = session->speed_coeff;
    thc->threshold   = session->threshold;
    thc->toggle_timer.func = toggle_event;
    thc->toggle_timer.waketime = session->begin;
}

void
schedule_stop(struct thc* thc, struct thc_session* session)
{
    stepper_schedule_position_mode(thc->z_stepper, session->end);
    thc->toggle_timer.func = toggle_event;
    thc->toggle_timer.waketime = session->end;
}

void
command_config_thc(uint32_t *args)
{
    struct thc *thc = oid_alloc(args[0], command_config_thc, sizeof(*thc));
    thc->update_interval = CONFIG_CLOCK_FREQ / args[1];
    thc->a_coeff         = args[2];
    thc->b_coeff         = args[3];
    thc->flags           = 0;
    thc->sbuf_current    = 0;
    thc->sbuf_size       = 0;
    thc->toggle_timer.func = NULL;

    // TODO make i2c config more generic and redo it on thc start
    thc->i2c_config = i2c_setup(0, 400000, 0x48);
    uint8_t ads1015_conf[3] = {0x01, 0x42, 0x63};
    i2c_write(thc->i2c_config, 3, ads1015_conf);
}
DECL_COMMAND(command_config_thc,
             "config_thc oid=%c rate=%u a_coeff=%i b_coeff=%i");

// Return the 'struct thc' for a given thc oid
struct thc *
thc_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_thc);
}

struct thc_session *
allocate_session(struct thc *thc)
{
    if(thc->sbuf_size >= SESSION_BUFFER_SIZE) {
        shutdown("Maximum THC session number reached.");
    }
    thc->sbuf_size++;
    uint8_t next_idx = (thc->sbuf_current + thc->sbuf_size - 1) %
                       SESSION_BUFFER_SIZE;
    struct thc_session *session = &thc->sbuf[next_idx];
    return session;
}

// Set end clock for the first session to not have end
void
append_session_end(struct thc *thc, uint32_t end)
{
    struct thc_session *session;
    for(uint8_t i=0;i<thc->sbuf_size;i++) {
        session = &thc->sbuf[(thc->sbuf_current + i) % SESSION_BUFFER_SIZE];
        if(!session->has_end) {
            session->has_end = 1;
            session->end = end;
            return;
        }
    }
}

// Schedule THC start
void
command_start_thc(uint32_t *args)
{
    struct thc *thc = thc_oid_lookup(args[0]);
    struct thc_session *session = allocate_session(thc);
    session->x_stepper_oid = args[1];
    session->y_stepper_oid = args[2];
    session->z_stepper_oid = args[3];
    session->begin         = args[4];
    session->target_mv     = args[5];
    session->speed_coeff   = args[6];
    session->threshold     = args[7];
    session->min_pos       = args[8];
    session->max_pos       = args[9];
    session->has_end       = 0;

    // if no pending toggle, kick timer
    if (thc->toggle_timer.func == NULL) {
        schedule_start(thc, session);
        sched_add_timer(&thc->toggle_timer);
    }
}
DECL_COMMAND(command_start_thc,
          "start_thc oid=%c x_stepper_oid=%c y_stepper_oid=%c z_stepper_oid=%c"
          " clock=%u voltage_mv=%i speed_coeff=%i threshold=%u min_pos=%i"
          " max_pos=%i");

// Schedule THC stop
void
command_stop_thc(uint32_t *args)
{
    struct thc *thc = thc_oid_lookup(args[0]);
    append_session_end(thc, args[1]);
    // if no pending toggle, kick timer
    if (thc->toggle_timer.func == NULL) {
        schedule_stop(thc, &thc->sbuf[thc->sbuf_current]);
        sched_add_timer(&thc->toggle_timer);
    }
}
DECL_COMMAND(command_stop_thc, "stop_thc oid=%c clock=%u");

int32_t
read_voltage_mv(struct thc *thc)
{
    uint8_t buf[2];
    i2c_read(thc->i2c_config, 1, 0x00, 2, buf);
    int16_t read_mv = div_pow2_16((int16_t)buf[0] << 8 | buf[1], 3);
    return div_pow2_32(thc->a_coeff * read_mv, 10) + thc->b_coeff;
}

void
thc_update(struct thc *thc)
{
    // prevent running task a last time with parameters from a future session
    if(!(thc->flags & THC_ACTIVE))
        return;
    int32_t voltage_mv = read_voltage_mv(thc);
    uint32_t xy_speed_squared = pow(stepper_speed(thc->x_stepper), 2) +
                                pow(stepper_speed(thc->y_stepper), 2);
    int32_t target_speed;
    if(xy_speed_squared >= thc->threshold) {
        int32_t error_mv = voltage_mv - thc->target_mv;
        target_speed = div_pow2_32(error_mv * thc->speed_coeff, 14);
    }
    else {
        target_speed = 0;
    }

    irq_disable();
    stepper_set_target_speed(thc->z_stepper, target_speed);
    int32_t z_pos = stepper_position(thc->z_stepper);
    irq_enable();
    sendf("thc_sample z_pos=%i voltage_mv=%i xy_speed_squared=%u",
          z_pos, voltage_mv, xy_speed_squared);
    thc->flags &= ~THC_NEED_UPDATE;
}

void
thc_toggle_task(void)
{
    if (!sched_check_wake(&thc_toggle_wake))
        return;

    uint8_t i;
    struct thc *thc;
    foreach_oid(i, thc, command_config_thc) {
        if(thc->flags & THC_NEED_TOGGLE)
            thc_toggle(thc);
    }
}
DECL_TASK(thc_toggle_task);

void
thc_update_task(void)
{
    if (!sched_check_wake(&thc_update_wake))
        return;

    uint8_t i;
    struct thc *thc;
    foreach_oid(i, thc, command_config_thc) {
        if(thc->flags & THC_NEED_UPDATE)
            thc_update(thc);
    }
}
DECL_TASK(thc_update_task);
