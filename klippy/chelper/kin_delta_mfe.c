// Delta kinematics stepper pulse time generation
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics

static double
delta_mfe_stepper_calc_position(struct stepper_kinematics *sk, struct move *m
                            , double move_time)
{
    return move_get_coord(m, move_time).z;
}

struct stepper_kinematics * __visible
delta_mfe_stepper_alloc()
{
    struct stepper_kinematics *sk = malloc(sizeof(*sk));
    memset(sk, 0, sizeof(*sk));
    sk->calc_position = delta_mfe_stepper_calc_position;
    return sk;
}
