// Hybrid CoreXZ kinematics stepper pulse time generation
//
// Copyright (C) 2021  Fabrice Gallet <tircown@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// The hybrid-corexz kinematic is also known as Markforged kinematics

#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "pyhelper.h" // errorf
#include "trapq.h" // move_get_coord

static double
hybrid_corexz_stepper_a_wait_calc_position(
    struct stepper_kinematics *sk, struct move *m, double move_time)
{
    return -move_get_coord(m, move_time).z;
}

struct stepper_kinematics * __visible
hybrid_corexz_stepper_alloc(char status)
{
    struct stepper_kinematics *sk = malloc(sizeof(*sk));
    memset(sk, 0, sizeof(*sk));

    if (status == 'w') {
        sk->calc_position_cb = hybrid_corexz_stepper_a_wait_calc_position;
    }
    sk->active_flags = AF_X | AF_Z;
    return sk;
}
