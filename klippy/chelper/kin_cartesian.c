// N-Axis Cartesian kinematics stepper pulse time generation
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2025  Modified for n-axis support
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "pyhelper.h" // errorf
#include "trapq.h" // move_get_coord

struct cartesian_stepper {
    struct stepper_kinematics sk;
    int axis_index;
};

static double
cart_stepper_calc_position(struct stepper_kinematics *sk, struct move *m, double move_time)
{
    struct cartesian_stepper *cs = container_of(sk, struct cartesian_stepper, sk);
    struct coord c = move_get_coord(m, move_time);
    double result = (cs->axis_index < c.axis_count) ? c.axis[cs->axis_index] : 0.0;
    coord_free(&c);
    return result;
}

struct stepper_kinematics * __visible
cartesian_stepper_alloc(int axis_index)
{
    struct cartesian_stepper *cs = malloc(sizeof(*cs));
    memset(cs, 0, sizeof(*cs));
    cs->axis_index = axis_index;
    cs->sk.calc_position_cb = cart_stepper_calc_position;
    cs->sk.active_flags = AF_AXIS(axis_index);
    return &cs->sk;
}

// Legacy compatibility function for single-character axis specification
struct stepper_kinematics * __visible
cartesian_stepper_alloc_legacy(char axis)
{
    int axis_index;
    if (axis == 'x') {
        axis_index = 0;
    } else if (axis == 'y') {
        axis_index = 1;
    } else if (axis == 'z') {
        axis_index = 2;
    } else {
        return NULL;
    }
    return cartesian_stepper_alloc(axis_index);
}
