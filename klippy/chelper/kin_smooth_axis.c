// Kinematic filter to smooth out cartesian XY movements
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "pyhelper.h" // errorf
#include "trapq.h" // trapq_integrate

// Calculate the definitive integral of the axis position
static double
move_integrate_distance(struct move *m, double start, double end
                        , double base, double axis_r)
{
    double half_v = .5 * m->start_v, sixth_a = (1. / 3.) * m->half_accel;
    double si = start * (base + start * axis_r * (half_v + sixth_a * start));
    double ei = end * (base + end * axis_r * (half_v + sixth_a * end));
    return ei - si;
}

// Calculate the definitive integral of time weighted axis position
static double
move_integrate_time_distance(struct move *m, double start, double end
                             , double base, double axis_r)
{
    double half_b = .5 * base, third_v = (1. / 3.) * m->start_v;
    double eighth_a = .25 * m->half_accel;
    double si = start * start * (half_b + start * axis_r * (
                                     third_v + eighth_a * start));
    double ei = end * end * (half_b + end * axis_r * (
                                 third_v + eighth_a * end));
    return ei - si;
}

// Calculate the definitive integral on part of a move
static double
move_integrate(struct move *m, int axis, double start, double end
               , double time_offset)
{
    if (start < 0.)
        start = 0.;
    if (end > m->move_t)
        end = m->move_t;
    double start_pos = m->start_pos.axis[axis - 'x'];
    double axis_r = m->axes_r.axis[axis - 'x'];
    double iaxis = move_integrate_distance(m, start, end, start_pos, axis_r);
    double wgt_iaxis = move_integrate_time_distance(m, start, end
                                                    , start_pos, axis_r);
    return wgt_iaxis - time_offset * iaxis;
}

// Calculate the definitive integral for a range of moves
static double
range_integrate(struct move *m, int axis, double move_time, double hst)
{
    // Calculate integral for the current move
    double res = 0., start = move_time - hst, end = move_time + hst;
    res += move_integrate(m, axis, start, move_time, start);
    res -= move_integrate(m, axis, move_time, end, end);
    // Integrate over previous moves
    struct move *prev = m;
    while (unlikely(start < 0.)) {
        prev = list_prev_entry(prev, node);
        start += prev->move_t;
        res += move_integrate(prev, axis, start, prev->move_t, start);
    }
    // Integrate over future moves
    while (unlikely(end > m->move_t)) {
        end -= m->move_t;
        m = list_next_entry(m, node);
        res -= move_integrate(m, axis, 0., end, end);
    }
    return res;
}

// Calculate average position over smooth_time window
static inline double
calc_position(struct move *m, int axis, double move_time
              , double hst, double inv_half_smooth_time2)
{
    double area = range_integrate(m, axis, move_time, hst);
    return area * inv_half_smooth_time2;
}

struct smooth_axis {
    struct stepper_kinematics sk;
    struct stepper_kinematics *orig_sk;
    double x_half_smooth_time, x_inv_half_smooth_time2;
    double y_half_smooth_time, y_inv_half_smooth_time2;
    struct move m;
};

#define DUMMY_T 500.0

// Optimized calc_position when only x axis is needed
static double
smooth_x_calc_position(struct stepper_kinematics *sk, struct move *m
                       , double move_time)
{
    struct smooth_axis *sa = container_of(sk, struct smooth_axis, sk);
    double hst = sa->x_half_smooth_time;
    if (!hst)
        return sa->orig_sk->calc_position_cb(sa->orig_sk, m, move_time);
    sa->m.start_pos.x = calc_position(m, 'x', move_time, hst
                                      , sa->x_inv_half_smooth_time2);
    return sa->orig_sk->calc_position_cb(sa->orig_sk, &sa->m, DUMMY_T);
}

// Optimized calc_position when only y axis is needed
static double
smooth_y_calc_position(struct stepper_kinematics *sk, struct move *m
                       , double move_time)
{
    struct smooth_axis *sa = container_of(sk, struct smooth_axis, sk);
    double hst = sa->y_half_smooth_time;
    if (!hst)
        return sa->orig_sk->calc_position_cb(sa->orig_sk, m, move_time);
    sa->m.start_pos.y = calc_position(m, 'y', move_time, hst
                                      , sa->y_inv_half_smooth_time2);
    return sa->orig_sk->calc_position_cb(sa->orig_sk, &sa->m, DUMMY_T);
}

// General calc_position for both x and y axes
static double
smooth_xy_calc_position(struct stepper_kinematics *sk, struct move *m
                        , double move_time)
{
    struct smooth_axis *sa = container_of(sk, struct smooth_axis, sk);
    double x_hst = sa->x_half_smooth_time;
    double y_hst = sa->y_half_smooth_time;
    if (!x_hst && !y_hst)
        return sa->orig_sk->calc_position_cb(sa->orig_sk, m, move_time);
    sa->m.start_pos = move_get_coord(m, move_time);
    if (x_hst)
        sa->m.start_pos.x = calc_position(m, 'x', move_time, x_hst
                                          , sa->x_inv_half_smooth_time2);
    if (y_hst)
        sa->m.start_pos.y = calc_position(m, 'y', move_time, y_hst
                                          , sa->y_inv_half_smooth_time2);
    return sa->orig_sk->calc_position_cb(sa->orig_sk, &sa->m, DUMMY_T);
}

void __visible
smooth_axis_set_time(struct stepper_kinematics *sk
                     , double smooth_x, double smooth_y)
{
    struct smooth_axis *sa = container_of(sk, struct smooth_axis, sk);
    double x_hst = .5 * smooth_x, y_hst = .5 * smooth_y;
    sa->x_half_smooth_time = x_hst;
    sa->x_inv_half_smooth_time2 = 1. / (x_hst * x_hst);
    sa->y_half_smooth_time = y_hst;
    sa->y_inv_half_smooth_time2 = 1. / (y_hst * y_hst);

    double hst = 0.;
    if (sa->sk.active_flags & AF_X)
        hst = x_hst;
    if (sa->sk.active_flags & AF_Y)
        hst = y_hst > hst ? y_hst : hst;
    sa->sk.gen_steps_pre_active = sa->sk.gen_steps_post_active = hst;
}

int __visible
smooth_axis_set_sk(struct stepper_kinematics *sk
                   , struct stepper_kinematics *orig_sk)
{
    struct smooth_axis *sa = container_of(sk, struct smooth_axis, sk);
    int af = orig_sk->active_flags & (AF_X | AF_Y);
    if (af == (AF_X | AF_Y))
        sa->sk.calc_position_cb = smooth_xy_calc_position;
    else if (af & AF_X)
        sa->sk.calc_position_cb = smooth_x_calc_position;
    else if (af & AF_Y)
        sa->sk.calc_position_cb = smooth_y_calc_position;
    else
        return -1;
    sa->sk.active_flags = orig_sk->active_flags;
    sa->orig_sk = orig_sk;
    return 0;
}

struct stepper_kinematics * __visible
smooth_axis_alloc(void)
{
    struct smooth_axis *sa = malloc(sizeof(*sa));
    memset(sa, 0, sizeof(*sa));
    sa->m.move_t = 2. * DUMMY_T;
    return &sa->sk;
}
