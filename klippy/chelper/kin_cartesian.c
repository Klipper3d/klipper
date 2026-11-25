// Generic cartesian kinematics stepper position calculation
//
// Copyright (C) 2018-2026  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2024  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "trapq.h" // move_get_coord

struct cartesian_stepper {
    struct stepper_kinematics sk;
    struct coord a;
};

// Optimized calc_position() calls for simple cartesian axes
static double
cart_stepper_x_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
    return move_get_coord(m, move_time).x;
}

static double
cart_stepper_y_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
    return move_get_coord(m, move_time).y;
}

static double
cart_stepper_z_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
    return move_get_coord(m, move_time).z;
}

// Optimized calc_position() calls for common corexy axes
static double
corexy_stepper_plus_calc_position(struct stepper_kinematics *sk, struct move *m
                                  , double move_time)
{
    struct coord c = move_get_coord(m, move_time);
    return c.x + c.y;
}

static double
corexy_stepper_minus_calc_position(struct stepper_kinematics *sk, struct move *m
                                   , double move_time)
{
    struct coord c = move_get_coord(m, move_time);
    return c.x - c.y;
}

// Generic cartesian calc_position()
static double
cartesian_stepper_calc_position(struct stepper_kinematics *sk
                                , struct move *m, double move_time)
{
    struct cartesian_stepper *cs = container_of(
            sk, struct cartesian_stepper, sk);
    struct coord c = move_get_coord(m, move_time);
    return cs->a.x * c.x + cs->a.y * c.y + cs->a.z * c.z;
}

static sk_calc_callback
lookup_calc_position_cb(struct coord a)
{
    if (a.x == 1. && a.y == 0. && a.z == 0.)
        return cart_stepper_x_calc_position;
    if (a.x == 0. && a.y == 1. && a.z == 0.)
        return cart_stepper_y_calc_position;
    if (a.x == 0. && a.y == 0. && a.z == 1.)
        return cart_stepper_z_calc_position;
    if (a.x == 1. && a.y == 1. && a.z == 0.)
        return corexy_stepper_plus_calc_position;
    if (a.x == 1. && a.y == -1. && a.z == 0.)
        return corexy_stepper_minus_calc_position;
    return cartesian_stepper_calc_position;
}

void __visible
cartesian_stepper_set_coeffs(struct stepper_kinematics *sk
                             , double a_x, double a_y, double a_z)
{
    struct cartesian_stepper *cs = container_of(
            sk, struct cartesian_stepper, sk);
    cs->a.x = a_x;
    cs->a.y = a_y;
    cs->a.z = a_z;
    cs->sk.active_flags= (a_x ? AF_X : 0) | (a_y ? AF_Y : 0) | (a_z ? AF_Z : 0);

    cs->sk.calc_position_cb = lookup_calc_position_cb(cs->a);
}

struct stepper_kinematics * __visible
cartesian_stepper_alloc(double a_x, double a_y, double a_z)
{
    struct cartesian_stepper *cs = malloc(sizeof(*cs));
    memset(cs, 0, sizeof(*cs));
    cartesian_stepper_set_coeffs(&cs->sk, a_x, a_y, a_z);
    return &cs->sk;
}
