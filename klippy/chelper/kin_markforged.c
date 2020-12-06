// Markforged kinematics stepper pulse time generation
//
// Copyright (C) 2020-2021  Fabrice Gallet <tircown@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "pyhelper.h" // errorf
#include "trapq.h" // move_get_coord

// Gantry XY
static double
mark_stepper_xy_waiting_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
	return move_get_coord(m, move_time).y;
}

static double
mark_stepper_xy_positive_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
	struct coord c = move_get_coord(m, move_time);
	return c.y + c.x;
}

static double
mark_stepper_xy_negative_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
	struct coord c = move_get_coord(m, move_time);
	return c.y - c.x;
}

// Gantry XZ
static double
mark_stepper_xz_waiting_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
	return move_get_coord(m, move_time).z;
}

static double
mark_stepper_xz_positive_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
	struct coord c = move_get_coord(m, move_time);
	return c.z + c.x;
}

static double
mark_stepper_xz_negative_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
	struct coord c = move_get_coord(m, move_time);
	return c.z - c.x;
}

struct stepper_kinematics * __visible
markforged_stepper_alloc(char second_axis, char direction)
{
    struct stepper_kinematics *sk = malloc(sizeof(*sk));
    memset(sk, 0, sizeof(*sk));
    if( second_axis == 'y' ){
        if (direction == 'w')
            sk->calc_position_cb = mark_stepper_xy_waiting_calc_position;
        else if (direction == 'p')
            sk->calc_position_cb = mark_stepper_xy_positive_calc_position;   
        else if (direction == 'n')
            sk->calc_position_cb = mark_stepper_xy_negative_calc_position;
        sk->active_flags = AF_X | AF_Y;
    } else if( second_axis == 'z' ){
        if (direction == 'w')
            sk->calc_position_cb = mark_stepper_xz_waiting_calc_position;
        else if (direction == 'p')
            sk->calc_position_cb = mark_stepper_xz_positive_calc_position;   
        else if (direction == 'n')
            sk->calc_position_cb = mark_stepper_xz_negative_calc_position;
        sk->active_flags = AF_X | AF_Z;
    }
    return sk;
}
