// Extruder stepper pulse time generation
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "pyhelper.h" // errorf

static double
extruder_calc_position(struct stepper_kinematics *sk, struct move *m
                       , double move_time)
{
    return m->start_pos.x + move_get_distance(m, move_time);
}

struct stepper_kinematics * __visible
extruder_stepper_alloc(void)
{
    struct stepper_kinematics *sk = malloc(sizeof(*sk));
    memset(sk, 0, sizeof(*sk));
    sk->calc_position = extruder_calc_position;
    return sk;
}

// Determine the coefficients for a 4th order bezier position function
static void
extruder_bezier4(struct move_accel *ma, double start_v, double extra_v
                      , double accel, double accel_t)
{
    if (!accel_t) {
        ma->c4 = ma->c3 = ma->c2 = ma->c1 = 0.;
        return;
    }
    double inv_accel_t = 1. / accel_t;
    double accel_div_accel_t = accel * inv_accel_t;
    double accel_div_accel_t2 = accel_div_accel_t * inv_accel_t;
    double extra_v_div_accel_t = extra_v * inv_accel_t;
    double extra_v_div_accel_t2 = extra_v_div_accel_t * inv_accel_t;
    ma->c4 = -.5 * accel_div_accel_t2;
    ma->c3 = accel_div_accel_t + (-2. * extra_v_div_accel_t2);
    ma->c2 = 3. * extra_v_div_accel_t;
    ma->c1 = start_v;
}

// Determine the coefficients for a 6th order bezier position function
static void
extruder_bezier6(struct move_accel *ma, double start_v, double extra_v
                      , double accel, double accel_t)
{
    if (!accel_t) {
        ma->c6 = ma->c5 = ma->c4 = ma->c3 = ma->c1 = 0.;
        return;
    }
    double inv_accel_t = 1. / accel_t;
    double accel_div_accel_t2 = accel * inv_accel_t * inv_accel_t;
    double accel_div_accel_t3 = accel_div_accel_t2 * inv_accel_t;
    double accel_div_accel_t4 = accel_div_accel_t3 * inv_accel_t;
    double extra_v_div_accel_t = extra_v * inv_accel_t;
    double extra_v_div_accel_t2 = extra_v_div_accel_t * inv_accel_t;
    double extra_v_div_accel_t3 = extra_v_div_accel_t2 * inv_accel_t;
    double extra_v_div_accel_t4 = extra_v_div_accel_t3 * inv_accel_t;
    ma->c6 = accel_div_accel_t4;
    ma->c5 = -3. * accel_div_accel_t3 + (6. * extra_v_div_accel_t4);
    ma->c4 = 2.5 * accel_div_accel_t2 + (-15. * extra_v_div_accel_t3);
    ma->c3 = 10. * extra_v_div_accel_t2;
    ma->c1 = start_v;
}

// Populate a 'struct move' with an extruder velocity trapezoid
void __visible
extruder_move_fill(struct move *m, double print_time
                   , double accel_t, double cruise_t, double decel_t
                   , double start_pos
                   , double start_v, double cruise_v, double accel
                   , double extra_accel_v, double extra_decel_v)
{
    // Setup velocity trapezoid
    m->print_time = print_time;
    m->move_t = accel_t + cruise_t + decel_t;
    m->accel_t = accel_t;
    m->cruise_t = cruise_t;
    m->cruise_start_d = accel_t * (.5 * (cruise_v + start_v) + extra_accel_v);
    m->decel_start_d = m->cruise_start_d + cruise_t * cruise_v;

    // Setup for accel/cruise/decel phases
    m->cruise_v = cruise_v;
    if (m->accel_order == 4) {
        extruder_bezier4(&m->accel, start_v, extra_accel_v, accel, accel_t);
        extruder_bezier4(&m->decel, cruise_v, extra_decel_v, -accel, decel_t);
    } else if (m->accel_order == 6) {
        extruder_bezier6(&m->accel, start_v, extra_accel_v, accel, accel_t);
        extruder_bezier6(&m->decel, cruise_v, extra_decel_v, -accel, decel_t);
    } else {
        m->accel.c1 = start_v + extra_accel_v;
        m->accel.c2 = .5 * accel;
        m->decel.c1 = cruise_v + extra_decel_v;
        m->decel.c2 = -m->accel.c2;
    }

    // Setup start distance
    m->start_pos.x = start_pos;
}
