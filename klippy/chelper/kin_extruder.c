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

// Helper code for integrating acceleration
static double
integrate_accel(struct move *m, double start, double end)
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
    double integral = integrate_accel(m, start, end);
    return base + integral * m->axes_r.axis[axis - 'x'];
}

// Calculate the definitive integral for a cartesian axis
static double
trapq_integrate(struct move *m, int axis, double start, double end)
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

// Find a move associated with a given time
static struct move *
trapq_find_move(struct move *m, double *ptime)
{
    double move_time = *ptime;
    for (;;) {
        if (unlikely(move_time < 0.)) {
            // Check previous move in list
            m = list_prev_entry(m, node);
            move_time += m->move_t;
        } else if (unlikely(move_time > m->move_t)) {
            // Check next move in list
            move_time -= m->move_t;
            m = list_next_entry(m, node);
        } else {
            *ptime = move_time;
            return m;
        }
    }
}

struct extruder_stepper {
    struct stepper_kinematics sk;
    double pressure_advance_factor, half_smooth_time, inv_smooth_time;
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
    // Calculate average position over smooth_time
    double area = trapq_integrate(m, 'x', move_time - hst, move_time + hst);
    double base_pos = area * es->inv_smooth_time;
    // Calculate position 'half_smooth_time' in the past
    double start_time = move_time - hst;
    struct move *sm = trapq_find_move(m, &start_time);
    double start_dist = move_get_distance(sm, start_time);
    double pa_start_pos = sm->start_pos.y + (sm->axes_r.y ? start_dist : 0.);
    // Calculate position 'half_smooth_time' in the future
    double end_time = move_time + hst;
    struct move *em = trapq_find_move(m, &end_time);
    double end_dist = move_get_distance(em, end_time);
    double pa_end_pos = em->start_pos.y + (em->axes_r.y ? end_dist : 0.);
    // Calculate position with pressure advance
    return base_pos + (pa_end_pos - pa_start_pos) * es->pressure_advance_factor;
}

void __visible
extruder_set_pressure(struct stepper_kinematics *sk
                      , double pressure_advance, double half_smooth_time)
{
    struct extruder_stepper *es = container_of(sk, struct extruder_stepper, sk);
    if (! half_smooth_time) {
        es->pressure_advance_factor = es->half_smooth_time = 0.;
        return;
    }
    es->sk.scan_past = es->sk.scan_future = half_smooth_time;
    es->half_smooth_time = half_smooth_time;
    es->inv_smooth_time = .5 / half_smooth_time;
    es->pressure_advance_factor = pressure_advance * es->inv_smooth_time;
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
