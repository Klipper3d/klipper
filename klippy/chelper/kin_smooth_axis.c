// Kinematic filter to smooth out cartesian XY movements
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "integrate.h" // integrate_weighted
#include "itersolve.h" // struct stepper_kinematics
#include "pyhelper.h" // errorf
#include "trapq.h" // trapq_integrate

// Calculate the definitive integral on part of a move
static double
move_integrate(struct move *m, int axis, double start, double end
               , double time_offset, double hst
               , double accel_comp)
{
    if (start < 0.)
        start = 0.;
    if (end > m->move_t)
        end = m->move_t;
    double axis_r = m->axes_r.axis[axis - 'x'];
    double start_pos = m->start_pos.axis[axis - 'x']
        + 2. * axis_r * m->half_accel * accel_comp;
    return integrate_weighted(start_pos, axis_r * m->start_v
            , axis_r * m->half_accel, start, end, time_offset, hst);
}

// Calculate the definitive integral for a range of moves
static double
range_integrate(struct move *m, int axis, double move_time, double hst
                , double accel_comp)
{
    // Calculate integral for the current move
    double res = 0., start = move_time - hst, end = move_time + hst;
    double offset = -move_time;
    res += move_integrate(m, axis, start, end, offset, hst, accel_comp);
    // Integrate over previous moves
    struct move *prev = m;
    while (unlikely(start < 0.)) {
        prev = list_prev_entry(prev, node);
        start += prev->move_t;
        offset -= prev->move_t;
        res += move_integrate(prev, axis, start, prev->move_t, offset, hst,
                              accel_comp);
    }
    // Integrate over future moves
    offset = -move_time;
    while (unlikely(end > m->move_t)) {
        end -= m->move_t;
        offset += m->move_t;
        m = list_next_entry(m, node);
        res += move_integrate(m, axis, 0., end, offset, hst, accel_comp);
    }
    return res;
}

// Calculate average position over smooth_time window
static inline double
calc_position(struct move *m, int axis, double move_time
              , double hst, double inv_norm
              , double accel_comp)
{
    double area = range_integrate(m, axis, move_time, hst, accel_comp);
    return area * inv_norm;
}

struct smooth_axis {
    struct stepper_kinematics sk;
    struct stepper_kinematics *orig_sk;
    double x_half_smooth_time, x_inv_norm;
    double y_half_smooth_time, y_inv_norm;
    double x_accel_comp, y_accel_comp;
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
                                      , sa->x_inv_norm
                                      , sa->x_accel_comp);
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
                                      , sa->y_inv_norm
                                      , sa->y_accel_comp);
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
                                          , sa->x_inv_norm
                                          , sa->x_accel_comp);
    if (y_hst)
        sa->m.start_pos.y = calc_position(m, 'y', move_time, y_hst
                                          , sa->y_inv_norm
                                          , sa->y_accel_comp);
    return sa->orig_sk->calc_position_cb(sa->orig_sk, &sa->m, DUMMY_T);
}

void __visible
smooth_axis_set_time(struct stepper_kinematics *sk
                     , double smooth_x, double smooth_y)
{
    struct smooth_axis *sa = container_of(sk, struct smooth_axis, sk);
    double x_hst = .5 * smooth_x, y_hst = .5 * smooth_y;
    sa->x_half_smooth_time = x_hst;
    sa->x_inv_norm = calc_inv_norm(x_hst);
    sa->y_half_smooth_time = y_hst;
    sa->y_inv_norm = calc_inv_norm(y_hst);

    double hst = 0.;
    if (sa->sk.active_flags & AF_X)
        hst = x_hst;
    if (sa->sk.active_flags & AF_Y)
        hst = y_hst > hst ? y_hst : hst;
    sa->sk.gen_steps_pre_active = sa->sk.gen_steps_post_active = hst;
}

void __visible
smooth_axis_set_accel_comp(struct stepper_kinematics *sk
                           , double accel_comp_x, double accel_comp_y)
{
    struct smooth_axis *sa = container_of(sk, struct smooth_axis, sk);
    sa->x_accel_comp = accel_comp_x;
    sa->y_accel_comp = accel_comp_y;
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
