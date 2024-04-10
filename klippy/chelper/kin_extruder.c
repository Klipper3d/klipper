// Extruder stepper pulse time generation
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "pyhelper.h" // errorf
#include "trapq.h" // move_get_distance

// Calculate the definitive integral of the move distance
static double
move_integrate_distance(struct move *m, double start, double end)
{
    double half_v = .5 * m->start_v, sixth_a = (1. / 3.) * m->half_accel;
    double si = start * start * (half_v + sixth_a * start);
    double ei = end * end * (half_v + sixth_a * end);
    return ei - si;
}

// Calculate the definitive integral of extruder with pressure advance
static double
pa_move_integrate(struct move *m, double start, double end)
{
    if (start < 0.)
        start = 0.;
    if (end > m->move_t)
        end = m->move_t;
    double pressure_advance = m->axes_r.y;
    double avg_v = m->start_v + (start + end) * m->half_accel;
    double pa_add = pressure_advance * avg_v;
    double base = (m->start_pos.x + pa_add) * (end - start);
    return base + move_integrate_distance(m, start, end);
}

// Calculate the definitive integral of the extruder over a range of moves
static double
pa_range_integrate(struct move *m, double start, double end)
{
    double res = pa_move_integrate(m, start, end);
    // Integrate over previous moves
    struct move *prev = m;
    while (unlikely(start < 0.)) {
        prev = list_prev_entry(prev, node);
        start += prev->move_t;
        res += pa_move_integrate(prev, start, prev->move_t);
    }
    // Integrate over future moves
    while (unlikely(end > m->move_t)) {
        end -= m->move_t;
        m = list_next_entry(m, node);
        res += pa_move_integrate(m, 0., end);
    }
    return res;
}

struct extruder_stepper {
    struct stepper_kinematics sk;
    double half_smooth_time, inv_smooth_time;
};

static double
extruder_calc_position(struct stepper_kinematics *sk, struct move *m
                       , double move_time)
{
    struct extruder_stepper *es = container_of(sk, struct extruder_stepper, sk);
    double hst = es->half_smooth_time;
    if (!hst)
        // Pressure advance not enabled
        return m->start_pos.x + move_get_distance(m, move_time);
    // Apply pressure advance and average over smooth_time
    double area = pa_range_integrate(m, move_time - hst, move_time + hst);
    return area * es->inv_smooth_time;
}

void __visible
extruder_set_smooth_time(struct stepper_kinematics *sk, double smooth_time)
{
    struct extruder_stepper *es = container_of(sk, struct extruder_stepper, sk);
    double hst = smooth_time * .5;
    es->half_smooth_time = hst;
    es->sk.gen_steps_pre_active = es->sk.gen_steps_post_active = hst;
    if (! hst)
        return;
    es->inv_smooth_time = 1. / smooth_time;
}

struct stepper_kinematics * __visible
extruder_stepper_alloc(void)
{
    struct extruder_stepper *es = malloc(sizeof(*es));
    memset(es, 0, sizeof(*es));
    es->sk.calc_position_cb = extruder_calc_position;
    es->sk.active_flags = AF_X;
    return &es->sk;
}
