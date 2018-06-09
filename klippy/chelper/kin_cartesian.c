// Cartesian kinematics stepper pulse time generation
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // sqrt
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // likely
#include "itersolve.h" // move_get_coord
#include "pyhelper.h" // errorf
#include "stepcompress.h" // queue_append


/****************************************************************
 * Direct step generation
 ****************************************************************/

// Common suffixes: _sd is step distance (a unit length the same
// distance the stepper moves on each step), _sv is step velocity (in
// units of step distance per time), _sd2 is step distance squared, _r
// is ratio (scalar usually between 0.0 and 1.0).  Times are in
// seconds and acceleration is in units of step distance per second
// squared.

// Wrapper around sqrt() to handle small negative numbers
static double
_safe_sqrt(double v)
{
    // Due to floating point truncation, it's possible to get a small
    // negative number - treat it as zero.
    if (v < -0.001)
        errorf("safe_sqrt of %.9f", v);
    return 0.;
}
inline double safe_sqrt(double v) {
    return likely(v >= 0.) ? sqrt(v) : _safe_sqrt(v);
}

// Schedule a step event at the specified step_clock time
int32_t __visible
stepcompress_push(struct stepcompress *sc, double print_time, int32_t sdir)
{
    int ret = set_next_step_dir(sc, !!sdir);
    if (ret)
        return ret;
    struct queue_append qa = queue_append_start(sc, print_time, 0.5);
    ret = queue_append(&qa, 0.);
    if (ret)
        return ret;
    queue_append_finish(qa);
    return sdir ? 1 : -1;
}

// Schedule 'steps' number of steps at constant acceleration. If
// acceleration is zero (ie, constant velocity) it uses the formula:
//  step_time = print_time + step_num/start_sv
// Otherwise it uses the formula:
//  step_time = (print_time + sqrt(2*step_num/accel + (start_sv/accel)**2)
//               - start_sv/accel)
int32_t __visible
stepcompress_push_const(
    struct stepcompress *sc, double print_time
    , double step_offset, double steps, double start_sv, double accel)
{
    // Calculate number of steps to take
    int sdir = 1;
    if (steps < 0) {
        sdir = 0;
        steps = -steps;
        step_offset = -step_offset;
    }
    int count = steps + .5 - step_offset;
    if (count <= 0 || count > 10000000) {
        if (count && steps) {
            errorf("push_const invalid count %d %f %f %f %f %f"
                   , stepcompress_get_oid(sc), print_time, step_offset, steps
                   , start_sv, accel);
            return ERROR_RET;
        }
        return 0;
    }
    int ret = set_next_step_dir(sc, sdir);
    if (ret)
        return ret;
    int res = sdir ? count : -count;

    // Calculate each step time
    if (!accel) {
        // Move at constant velocity (zero acceleration)
        struct queue_append qa = queue_append_start(sc, print_time, .5);
        double inv_cruise_sv = stepcompress_get_mcu_freq(sc) / start_sv;
        double pos = (step_offset + .5) * inv_cruise_sv;
        while (count--) {
            ret = queue_append(&qa, pos);
            if (ret)
                return ret;
            pos += inv_cruise_sv;
        }
        queue_append_finish(qa);
    } else {
        // Move with constant acceleration
        double inv_accel = 1. / accel;
        double mcu_freq = stepcompress_get_mcu_freq(sc);
        double accel_time = start_sv * inv_accel * mcu_freq;
        struct queue_append qa = queue_append_start(
            sc, print_time, 0.5 - accel_time);
        double accel_multiplier = 2. * inv_accel * mcu_freq * mcu_freq;
        double pos = (step_offset + .5)*accel_multiplier + accel_time*accel_time;
        while (count--) {
            double v = safe_sqrt(pos);
            int ret = queue_append(&qa, accel_multiplier >= 0. ? v : -v);
            if (ret)
                return ret;
            pos += accel_multiplier;
        }
        queue_append_finish(qa);
    }
    return res;
}


/****************************************************************
 * Iterative solver
 ****************************************************************/

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

struct stepper_kinematics * __visible
cartesian_stepper_alloc(char axis)
{
    struct stepper_kinematics *sk = malloc(sizeof(*sk));
    memset(sk, 0, sizeof(*sk));
    if (axis == 'x')
        sk->calc_position = cart_stepper_x_calc_position;
    else if (axis == 'y')
        sk->calc_position = cart_stepper_y_calc_position;
    else if (axis == 'z')
        sk->calc_position = cart_stepper_z_calc_position;
    return sk;
}
