// Kinematic input shapers to minimize motion vibrations in XY plane
//
// Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // sqrt, exp
#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "trapq.h" // struct move


/****************************************************************
 * Shaper initialization
 ****************************************************************/

struct shaper_pulses {
    int num_pulses;
    struct {
        double t, a;
    } pulses[5];
};

// Shift pulses around 'mid-point' t=0 so that the input shaper is an identity
// transformation for constant-speed motion (i.e. input_shaper(v * T) = v * T)
static void
shift_pulses(struct shaper_pulses *sp)
{
    int i;
    double ts = 0.;
    for (i = 0; i < sp->num_pulses; ++i)
        ts += sp->pulses[i].a * sp->pulses[i].t;
    for (i = 0; i < sp->num_pulses; ++i)
        sp->pulses[i].t -= ts;
}

static int
init_shaper(int n, double a[], double t[], struct shaper_pulses *sp)
{
    if (n < 0 || n > ARRAY_SIZE(sp->pulses)) {
        sp->num_pulses = 0;
        return -1;
    }
    int i;
    double sum_a = 0.;
    for (i = 0; i < n; ++i)
        sum_a += a[i];
    double inv_a = 1. / sum_a;
    // Reverse pulses vs their traditional definition
    for (i = 0; i < n; ++i) {
        sp->pulses[n-i-1].a = a[i] * inv_a;
        sp->pulses[n-i-1].t = -t[i];
    }
    sp->num_pulses = n;
    shift_pulses(sp);
    return 0;
}


/****************************************************************
 * Generic position calculation via shaper convolution
 ****************************************************************/

static inline double
get_axis_position(struct move *m, int axis, double move_time)
{
    double axis_r = m->axes_r.axis[axis - 'x'];
    double start_pos = m->start_pos.axis[axis - 'x'];
    double move_dist = move_get_distance(m, move_time);
    return start_pos + axis_r * move_dist;
}

static inline double
get_axis_position_across_moves(struct move *m, int axis, double time)
{
    while (likely(time < 0.)) {
        m = list_prev_entry(m, node);
        time += m->move_t;
    }
    while (likely(time > m->move_t)) {
        time -= m->move_t;
        m = list_next_entry(m, node);
    }
    return get_axis_position(m, axis, time);
}

// Calculate the position from the convolution of the shaper with input signal
static inline double
calc_position(struct move *m, int axis, double move_time
              , struct shaper_pulses *sp)
{
    double res = 0.;
    int num_pulses = sp->num_pulses, i;
    for (i = 0; i < num_pulses; ++i) {
        double t = sp->pulses[i].t, a = sp->pulses[i].a;
        res += a * get_axis_position_across_moves(m, axis, move_time + t);
    }
    return res;
}


/****************************************************************
 * Kinematics-related shaper code
 ****************************************************************/

#define DUMMY_T 500.0

struct input_shaper {
    struct stepper_kinematics sk;
    struct stepper_kinematics *orig_sk;
    struct move m;
    struct shaper_pulses sx, sy;
};

// Optimized calc_position when only x axis is needed
static double
shaper_x_calc_position(struct stepper_kinematics *sk, struct move *m
                       , double move_time)
{
    struct input_shaper *is = container_of(sk, struct input_shaper, sk);
    if (!is->sx.num_pulses)
        return is->orig_sk->calc_position_cb(is->orig_sk, m, move_time);
    is->m.start_pos.x = calc_position(m, 'x', move_time, &is->sx);
    return is->orig_sk->calc_position_cb(is->orig_sk, &is->m, DUMMY_T);
}

// Optimized calc_position when only y axis is needed
static double
shaper_y_calc_position(struct stepper_kinematics *sk, struct move *m
                       , double move_time)
{
    struct input_shaper *is = container_of(sk, struct input_shaper, sk);
    if (!is->sy.num_pulses)
        return is->orig_sk->calc_position_cb(is->orig_sk, m, move_time);
    is->m.start_pos.y = calc_position(m, 'y', move_time, &is->sy);
    return is->orig_sk->calc_position_cb(is->orig_sk, &is->m, DUMMY_T);
}

// General calc_position for both x and y axes
static double
shaper_xy_calc_position(struct stepper_kinematics *sk, struct move *m
                        , double move_time)
{
    struct input_shaper *is = container_of(sk, struct input_shaper, sk);
    if (!is->sx.num_pulses && !is->sy.num_pulses)
        return is->orig_sk->calc_position_cb(is->orig_sk, m, move_time);
    is->m.start_pos = move_get_coord(m, move_time);
    if (is->sx.num_pulses)
        is->m.start_pos.x = calc_position(m, 'x', move_time, &is->sx);
    if (is->sy.num_pulses)
        is->m.start_pos.y = calc_position(m, 'y', move_time, &is->sy);
    return is->orig_sk->calc_position_cb(is->orig_sk, &is->m, DUMMY_T);
}

int __visible
input_shaper_set_sk(struct stepper_kinematics *sk
                    , struct stepper_kinematics *orig_sk)
{
    struct input_shaper *is = container_of(sk, struct input_shaper, sk);
    if (orig_sk->active_flags == AF_X)
        is->sk.calc_position_cb = shaper_x_calc_position;
    else if (orig_sk->active_flags == AF_Y)
        is->sk.calc_position_cb = shaper_y_calc_position;
    else if (orig_sk->active_flags & (AF_X | AF_Y))
        is->sk.calc_position_cb = shaper_xy_calc_position;
    else
        return -1;
    is->sk.active_flags = orig_sk->active_flags;
    is->orig_sk = orig_sk;
    is->sk.commanded_pos = orig_sk->commanded_pos;
    is->sk.last_flush_time = orig_sk->last_flush_time;
    is->sk.last_move_time = orig_sk->last_move_time;
    return 0;
}

static void
shaper_note_generation_time(struct input_shaper *is)
{
    double pre_active = 0., post_active = 0.;
    if ((is->sk.active_flags & AF_X) && is->sx.num_pulses) {
        pre_active = is->sx.pulses[is->sx.num_pulses-1].t;
        post_active = -is->sx.pulses[0].t;
    }
    if ((is->sk.active_flags & AF_Y) && is->sy.num_pulses) {
        pre_active = is->sy.pulses[is->sy.num_pulses-1].t > pre_active
            ? is->sy.pulses[is->sy.num_pulses-1].t : pre_active;
        post_active = -is->sy.pulses[0].t > post_active
            ? -is->sy.pulses[0].t : post_active;
    }
    is->sk.gen_steps_pre_active = pre_active;
    is->sk.gen_steps_post_active = post_active;
}

int __visible
input_shaper_set_shaper_params(struct stepper_kinematics *sk, char axis
                               , int n, double a[], double t[])
{
    if (axis != 'x' && axis != 'y')
        return -1;
    struct input_shaper *is = container_of(sk, struct input_shaper, sk);
    struct shaper_pulses *sp = axis == 'x' ? &is->sx : &is->sy;
    int status = 0;
    if (is->orig_sk->active_flags & (axis == 'x' ? AF_X : AF_Y))
        status = init_shaper(n, a, t, sp);
    else
        sp->num_pulses = 0;
    shaper_note_generation_time(is);
    return status;
}

double __visible
input_shaper_get_step_generation_window(struct stepper_kinematics *sk)
{
    struct input_shaper *is = container_of(sk, struct input_shaper, sk);
    return is->sk.gen_steps_pre_active > is->sk.gen_steps_post_active
         ? is->sk.gen_steps_pre_active : is->sk.gen_steps_post_active;
}

struct stepper_kinematics * __visible
input_shaper_alloc(void)
{
    struct input_shaper *is = malloc(sizeof(*is));
    memset(is, 0, sizeof(*is));
    is->m.move_t = 2. * DUMMY_T;
    return &is->sk;
}
