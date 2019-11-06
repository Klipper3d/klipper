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

// Calculate the definitive integral of the move distance
static double
move_integrate_distance(struct move *m, double start, double end)
{
    double half_v = .5 * m->start_v, sixth_a = (1. / 3.) * m->half_accel;
    double si = start * start * (half_v + sixth_a * start);
    double ei = end * end * (half_v + sixth_a * end);
    return ei - si;
}

// Calculate the definitive integral on part of a move
static double
move_integrate(struct move *m, int axis, double start, double end)
{
    if (start < 0.)
        start = 0.;
    if (end > m->move_t)
        end = m->move_t;
    double base = m->start_pos.axis[axis - 'x'] * (end - start);
    double integral = move_integrate_distance(m, start, end);
    return base + integral * m->axes_r.axis[axis - 'x'];
}

// Calculate the definitive integral for a range of moves
static double
range_integrate(struct move *m, int axis, double start, double end)
{
    double res = move_integrate(m, axis, start, end);
    // Integrate over previous moves
    struct move *prev = m;
    while (unlikely(start < 0.)) {
        prev = list_prev_entry(prev, node);
        start += prev->move_t;
        res += move_integrate(prev, axis, start, prev->move_t);
    }
    // Integrate over future moves
    while (unlikely(end > m->move_t)) {
        end -= m->move_t;
        m = list_next_entry(m, node);
        res += move_integrate(m, axis, 0., end);
    }
    return res;
}

// Calculate average position over smooth_time window
static inline double
calc_position(struct move *m, int axis, double move_time
              , double hst, double inv_smooth_time)
{
    double area = range_integrate(m, axis, move_time - hst, move_time + hst);
    return area * inv_smooth_time;
}

struct smooth_axis {
    struct stepper_kinematics sk;
    struct stepper_kinematics *orig_sk;
    double x_half_smooth_time, x_inv_smooth_time;
    double y_half_smooth_time, y_inv_smooth_time;
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
                                      , sa->x_inv_smooth_time);
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
                                      , sa->y_inv_smooth_time);
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
                                          , sa->x_inv_smooth_time);
    if (y_hst)
        sa->m.start_pos.y = calc_position(m, 'y', move_time, y_hst
                                          , sa->y_inv_smooth_time);
    return sa->orig_sk->calc_position_cb(sa->orig_sk, &sa->m, DUMMY_T);
}

void __visible
smooth_axis_set_time(struct stepper_kinematics *sk
                     , double smooth_x, double smooth_y)
{
    struct smooth_axis *sa = container_of(sk, struct smooth_axis, sk);
    sa->x_half_smooth_time = .5 * smooth_x;
    sa->x_inv_smooth_time = 1. / smooth_x;
    sa->y_half_smooth_time = .5 * smooth_y;
    sa->y_inv_smooth_time = 1. / smooth_y;

    double hst = 0.;
    if (sa->sk.active_flags & AF_X)
        hst = sa->x_half_smooth_time;
    if (sa->sk.active_flags & AF_Y)
        hst = sa->y_half_smooth_time > hst ? sa->y_half_smooth_time : hst;
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
