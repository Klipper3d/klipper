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
    double result = 0.0;

    // Calculate dot product for all axes
    int min_axes = (cs->a.axis_count < c.axis_count) ? cs->a.axis_count : c.axis_count;
    for (int i = 0; i < min_axes; i++) {
        result += cs->a.axis[i] * c.axis[i];
    }

    coord_free(&c);
    return result;
}

void __visible
generic_cartesian_stepper_set_coeffs_n(struct stepper_kinematics *sk
                                       , double *coeffs, int axis_count)
{
    struct generic_cartesian_stepper *cs = container_of(
            sk, struct generic_cartesian_stepper, sk);

    // Free existing coordinate if it exists
    coord_free(&cs->a);

    // Allocate new coordinate with specified axis count
    cs->a = coord_alloc(axis_count);

    // Copy coefficients and calculate active flags
    cs->sk.active_flags = 0;
    for (int i = 0; i < axis_count; i++) {
        cs->a.axis[i] = coeffs[i];
        if (coeffs[i] != 0.0) {
            cs->sk.active_flags |= AF_AXIS(i);
        }
    }
}

// Legacy function for backward compatibility with xyz
void __visible
generic_cartesian_stepper_set_coeffs(struct stepper_kinematics *sk
                                     , double a_x, double a_y, double a_z)
{
    double coeffs[3] = {a_x, a_y, a_z};
    generic_cartesian_stepper_set_coeffs_n(sk, coeffs, 3);
}

struct stepper_kinematics * __visible
generic_cartesian_stepper_alloc_n(double *coeffs, int axis_count)
{
    struct generic_cartesian_stepper *cs = malloc(sizeof(*cs));
    memset(cs, 0, sizeof(*cs));
    cs->sk.calc_position_cb = generic_cartesian_stepper_calc_position;
    // Initialize with zero coordinate first
    cs->a = coord_alloc(1);
    // Then set the actual coefficients
    generic_cartesian_stepper_set_coeffs_n(&cs->sk, coeffs, axis_count);
    return &cs->sk;
}

// Legacy function for backward compatibility with xyz
struct stepper_kinematics * __visible
generic_cartesian_stepper_alloc(double a_x, double a_y, double a_z)
{
    double coeffs[3] = {a_x, a_y, a_z};
    return generic_cartesian_stepper_alloc_n(coeffs, 3);
}
