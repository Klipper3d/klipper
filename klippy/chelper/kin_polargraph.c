// Polargraph kinematics stepper pulse time generation
//
// Copyright (C) 2022  Tsogtgerel Amar <ts.r.madrid@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // sqrt
#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "pyhelper.h" // errorf
#include "trapq.h" // move_get_coord

struct polargraph_stepper {
    struct stepper_kinematics sk;
    double width;
};
static double
polargraph_stepper_left_calc_position(struct stepper_kinematics *sk
                             , struct move *m
                             , double move_time)
{
    struct polargraph_stepper *ps = container_of(sk, struct polargraph_stepper
                             , sk);
    struct coord c = move_get_coord(m, move_time);
    return (c.x * c.x - c.y * c.y + ps->width * ps->width) / (ps->width * 2);
}

static double
polargraph_stepper_right_calc_position(struct stepper_kinematics *sk
                             , struct move *m
                             , double move_time)
{
    struct polargraph_stepper *ps = container_of(sk, struct polargraph_stepper
                             , sk);
    struct coord c = move_get_coord(m, move_time);
    double x = (c.x * c.x - c.y * c.y + ps->width * ps->width) /
        (ps->width * 2);
    return sqrt(c.x * c.x - x * x);
}

struct stepper_kinematics * __visible
polargraph_stepper_alloc(double width, char side)
{
    struct polargraph_stepper *ps = malloc(sizeof(*ps));
    memset(ps, 0, sizeof(*ps));
    if (side == 'l') {
        ps->sk.calc_position_cb = polargraph_stepper_left_calc_position;
    } else if (side == 'r') {
        ps->sk.calc_position_cb = polargraph_stepper_right_calc_position;
    }
    ps->sk.active_flags = AF_X | AF_Y;
    ps->width = width;
    return &ps->sk;
}
