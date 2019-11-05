// Extruder stepper pulse time generation
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "pyhelper.h" // errorf
#include "trapq.h" // move_get_distance

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
    sk->calc_position_cb = extruder_calc_position;
    sk->active_flags = AF_X;
    return sk;
}

// Populate a 'struct move' with an extruder velocity trapezoid
void __visible
extruder_add_move(struct trapq *tq, double print_time
                  , double accel_t, double cruise_t, double decel_t
                  , double start_e_pos
                  , double start_v, double cruise_v, double accel
                  , double extra_accel_v, double extra_decel_v)
{
    struct coord start_pos, axes_r;
    start_pos.x = start_e_pos;
    axes_r.x = 1.;
    start_pos.y = start_pos.z = axes_r.y = axes_r.z = 0.;

    if (accel_t) {
        struct move *m = move_alloc();
        m->print_time = print_time;
        m->move_t = accel_t;
        m->start_v = start_v + extra_accel_v;
        m->half_accel = .5 * accel;
        m->start_pos = start_pos;
        m->axes_r = axes_r;
        trapq_add_move(tq, m);

        print_time += accel_t;
        start_pos.x += move_get_distance(m, accel_t);
    }
    if (cruise_t) {
        struct move *m = move_alloc();
        m->print_time = print_time;
        m->move_t = cruise_t;
        m->start_v = cruise_v;
        m->half_accel = 0.;
        m->start_pos = start_pos;
        m->axes_r = axes_r;
        trapq_add_move(tq, m);

        print_time += cruise_t;
        start_pos.x += move_get_distance(m, cruise_t);
    }
    if (decel_t) {
        struct move *m = move_alloc();
        m->print_time = print_time;
        m->move_t = decel_t;
        m->start_v = cruise_v + extra_decel_v;
        m->half_accel = -.5 * accel;
        m->start_pos = start_pos;
        m->axes_r = axes_r;
        trapq_add_move(tq, m);
    }
}
