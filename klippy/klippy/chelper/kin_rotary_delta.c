// Rotary delta kinematics stepper pulse time generation
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // sqrt
#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "trapq.h" // move_get_coord

// The arm angle calculation is based on the following two formulas:
//   elbow_x**2 + elbow_y**2 = upper_arm**2
//   (effector_x - elbow_x)**2 + (effector_y - elbow_y)**2 = lower_arm**2

// Calculate upper arm angle given xy position of effector joint
// (relative to shoulder joint), upper arm length, and lower arm length.
static inline double
rotary_two_arm_calc(double dx, double dy, double upper_arm2, double lower_arm2)
{
    // Determine constants such that: elbow_y = c1 - c2*elbow_x
    double inv_dy = 1. / dy;
    double c1 = .5 * inv_dy * (dx*dx + dy*dy + upper_arm2 - lower_arm2);
    double c2 = dx * inv_dy;
    // Calculate scaled elbow coordinates via quadratic equation.
    double scale = c2*c2 + 1.0;
    double scaled_elbow_x = c1*c2 + sqrt(scale*upper_arm2 - c1*c1);
    double scaled_elbow_y = c1*scale - c2*scaled_elbow_x;
    // Calculate angle in radians
    return atan2(scaled_elbow_y, scaled_elbow_x);
}

struct rotary_stepper {
    struct stepper_kinematics sk;
    double cos, sin, shoulder_radius, shoulder_height;
    double upper_arm2, lower_arm2;
};

static double
rotary_stepper_calc_position(struct stepper_kinematics *sk, struct move *m
                             , double move_time)
{
    struct rotary_stepper *rs = container_of(sk, struct rotary_stepper, sk);
    struct coord c = move_get_coord(m, move_time);
    // Rotate and shift axes to an origin at shoulder joint with upper
    // arm constrained to xy plane and x aligned to shoulder platform.
    double sjz = c.y * rs->cos - c.x * rs->sin;
    double sjx = c.x * rs->cos + c.y * rs->sin - rs->shoulder_radius;
    double sjy = c.z - rs->shoulder_height;
    // Calculate angle in radians
    return rotary_two_arm_calc(sjx, sjy, rs->upper_arm2
                               , rs->lower_arm2 - sjz*sjz);
}

struct stepper_kinematics * __visible
rotary_delta_stepper_alloc(double shoulder_radius, double shoulder_height
                           , double angle, double upper_arm, double lower_arm)
{
    struct rotary_stepper *rs = malloc(sizeof(*rs));
    memset(rs, 0, sizeof(*rs));
    rs->cos = cos(angle);
    rs->sin = sin(angle);
    rs->shoulder_radius = shoulder_radius;
    rs->shoulder_height = shoulder_height;
    rs->upper_arm2 = upper_arm * upper_arm;
    rs->lower_arm2 = lower_arm * lower_arm;
    rs->sk.calc_position_cb = rotary_stepper_calc_position;
    rs->sk.active_flags = AF_X | AF_Y | AF_Z;
    return &rs->sk;
}
