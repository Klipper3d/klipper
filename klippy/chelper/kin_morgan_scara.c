// Morgan SCARA kinematics stepper pulse time generation
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2019  Noah <noah@hack.se>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // sqrt, acos
#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "trapq.h" // move_get_coord

struct morgan_stepper {
    struct stepper_kinematics sk;
    double inner_arm_length, outer_arm_length;
    double inner_arm2, outer_arm2;
};

static double
morgan_calc_shoulder_joint_angle(struct stepper_kinematics *sk, struct coord *c)
{
    struct morgan_stepper *ms = container_of(sk, struct morgan_stepper, sk);

    // Calculate distance to (x, y) position
    double distance2 = c->x*c->x + c->y*c->y;
    double distance = sqrt(distance2);

    // Find angles using law of cosines
    double shoulder_joint_angle;
    shoulder_joint_angle = acos((ms->inner_arm2 + distance2 - ms->outer_arm2) / (2*ms->inner_arm_length*distance));
    //double elbow_joint_angle;
    //elbow_joint_angle = acos((ms->outer_arm2 + ms->inner_arm2 - distance2) / (2*ms->outer_arm_length*ms->inner_arm_length));

    return shoulder_joint_angle;
}

static double
left_stepper_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
    struct coord c = move_get_coord(m, move_time);
    double angle = M_PI/2 + morgan_calc_shoulder_joint_angle(sk, &c) - atan2(c.x, c.y);

    return angle;
}

static double
right_stepper_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
    struct coord c = move_get_coord(m, move_time);
    double angle = M_PI/2 - morgan_calc_shoulder_joint_angle(sk, &c) - atan2(c.x, c.y);

    return angle;
}

struct stepper_kinematics * __visible
morgan_scara_stepper_alloc(char stepper, double inner_arm_length, double outer_arm_length)
{
    struct morgan_stepper *ms = malloc(sizeof(*ms));
    memset(ms, 0, sizeof(*ms));
    ms->inner_arm_length = inner_arm_length;
    ms->outer_arm_length = outer_arm_length;
    ms->inner_arm2 = inner_arm_length*inner_arm_length;
    ms->outer_arm2 = outer_arm_length*outer_arm_length;

    /* Ensure itersolve is called for all X and Y moves */
    ms->sk.active_flags = AF_X | AF_Y;
    /* Setup itersolve callback */
    if (stepper == 'a')
        ms->sk.calc_position_cb = left_stepper_calc_position;
    else if (stepper == 'b')
        ms->sk.calc_position_cb = right_stepper_calc_position;

    return &ms->sk;
}
