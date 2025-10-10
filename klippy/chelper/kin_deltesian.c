// Deltesian kinematics stepper pulse time generation
//
// Copyright (C) 2022  Fabrice Gallet <tircown@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // sqrt
#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "trapq.h" // move_get_coord

struct deltesian_stepper {
    struct stepper_kinematics sk;
    double arm2, arm_x;
};

static double
deltesian_stepper_calc_position(struct stepper_kinematics *sk, struct move *m
                            , double move_time)
{
    struct deltesian_stepper *ds = container_of(
                sk, struct deltesian_stepper, sk);
    struct coord c = move_get_coord(m, move_time);
    double dx = c.x - ds->arm_x;
    return sqrt(ds->arm2 - dx*dx) + c.z;
}

struct stepper_kinematics * __visible
deltesian_stepper_alloc(double arm2, double arm_x)
{
    struct deltesian_stepper *ds = malloc(sizeof(*ds));
    memset(ds, 0, sizeof(*ds));
    ds->arm2 = arm2;
    ds->arm_x = arm_x;
    ds->sk.calc_position_cb = deltesian_stepper_calc_position;
    ds->sk.active_flags = AF_X | AF_Z;
    return &ds->sk;
}
