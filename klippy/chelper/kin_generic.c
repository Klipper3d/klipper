// Generic cartesian kinematics stepper position calculation
//
// Copyright (C) 2024  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "trapq.h" // move_get_coord

struct generic_cartesian_stepper {
    struct stepper_kinematics sk;
    struct coord a;
};

static double
generic_cartesian_stepper_calc_position(struct stepper_kinematics *sk
                                        , struct move *m, double move_time)
{
    struct generic_cartesian_stepper *cs = container_of(
            sk, struct generic_cartesian_stepper, sk);
    struct coord c = move_get_coord(m, move_time);
    return cs->a.x * c.x + cs->a.y * c.y + cs->a.z * c.z;
}

void __visible
generic_cartesian_stepper_set_coeffs(struct stepper_kinematics *sk
                                     , double a_x, double a_y, double a_z)
{
    struct generic_cartesian_stepper *cs = container_of(
            sk, struct generic_cartesian_stepper, sk);
    cs->a.x = a_x;
    cs->a.y = a_y;
    cs->a.z = a_z;
    cs->sk.active_flags = 0;
    if (a_x) cs->sk.active_flags |= AF_X;
    if (a_y) cs->sk.active_flags |= AF_Y;
    if (a_z) cs->sk.active_flags |= AF_Z;
}

struct stepper_kinematics * __visible
generic_cartesian_stepper_alloc(double a_x, double a_y, double a_z)
{
    struct generic_cartesian_stepper *cs = malloc(sizeof(*cs));
    memset(cs, 0, sizeof(*cs));
    cs->sk.calc_position_cb = generic_cartesian_stepper_calc_position;
    generic_cartesian_stepper_set_coeffs(&cs->sk, a_x, a_y, a_z);
    return &cs->sk;
}
