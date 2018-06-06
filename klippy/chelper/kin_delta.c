// Delta kinematics stepper pulse time generation
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // sqrt
#include "compiler.h" // __visible
#include "pyhelper.h" // errorf
#include "stepcompress.h" // queue_append

// Schedule steps using delta kinematics
static int32_t
_stepcompress_push_delta(
    struct stepcompress *sc, int sdir
    , double print_time, double move_sd, double start_sv, double accel
    , double height, double startxy_sd, double arm_sd, double movez_r)
{
    // Calculate number of steps to take
    double movexy_r = movez_r ? sqrt(1. - movez_r*movez_r) : 1.;
    double arm_sd2 = arm_sd * arm_sd;
    double endxy_sd = startxy_sd - movexy_r*move_sd;
    double end_height = safe_sqrt(arm_sd2 - endxy_sd*endxy_sd);
    int count = (end_height + movez_r*move_sd - height) * (sdir ? 1. : -1.) + .5;
    if (count <= 0 || count > 10000000) {
        if (count) {
            errorf("push_delta invalid count %d %d %f %f %f %f %f %f %f %f"
                   , stepcompress_get_oid(sc), count, print_time, move_sd
                   , start_sv, accel, height, startxy_sd, arm_sd, movez_r);
            return ERROR_RET;
        }
        return 0;
    }
    int ret = set_next_step_dir(sc, sdir);
    if (ret)
        return ret;
    int res = sdir ? count : -count;

    // Calculate each step time
    height += (sdir ? .5 : -.5);
    if (!accel) {
        // Move at constant velocity (zero acceleration)
        struct queue_append qa = queue_append_start(sc, print_time, .5);
        double inv_cruise_sv = stepcompress_get_mcu_freq(sc) / start_sv;
        if (!movez_r) {
            // Optimized case for common XY only moves (no Z movement)
            while (count--) {
                double v = safe_sqrt(arm_sd2 - height*height);
                double pos = startxy_sd + (sdir ? -v : v);
                int ret = queue_append(&qa, pos * inv_cruise_sv);
                if (ret)
                    return ret;
                height += (sdir ? 1. : -1.);
            }
        } else if (!movexy_r) {
            // Optimized case for Z only moves
            double pos = ((sdir ? height-end_height : end_height-height)
                          * inv_cruise_sv);
            while (count--) {
                int ret = queue_append(&qa, pos);
                if (ret)
                    return ret;
                pos += inv_cruise_sv;
            }
        } else {
            // General case (handles XY+Z moves)
            double start_pos = movexy_r*startxy_sd, zoffset = movez_r*startxy_sd;
            while (count--) {
                double relheight = movexy_r*height - zoffset;
                double v = safe_sqrt(arm_sd2 - relheight*relheight);
                double pos = start_pos + movez_r*height + (sdir ? -v : v);
                int ret = queue_append(&qa, pos * inv_cruise_sv);
                if (ret)
                    return ret;
                height += (sdir ? 1. : -1.);
            }
        }
        queue_append_finish(qa);
    } else {
        // Move with constant acceleration
        double start_pos = movexy_r*startxy_sd, zoffset = movez_r*startxy_sd;
        double mcu_freq = stepcompress_get_mcu_freq(sc);
        double inv_accel = 1. / accel;
        start_pos += 0.5 * start_sv*start_sv * inv_accel;
        struct queue_append qa = queue_append_start(
            sc, print_time, 0.5 - start_sv * inv_accel * mcu_freq);
        double accel_multiplier = 2. * inv_accel * mcu_freq * mcu_freq;
        while (count--) {
            double relheight = movexy_r*height - zoffset;
            double v = safe_sqrt(arm_sd2 - relheight*relheight);
            double pos = start_pos + movez_r*height + (sdir ? -v : v);
            v = safe_sqrt(pos * accel_multiplier);
            int ret = queue_append(&qa, accel_multiplier >= 0. ? v : -v);
            if (ret)
                return ret;
            height += (sdir ? 1. : -1.);
        }
        queue_append_finish(qa);
    }
    return res;
}

int32_t __visible
stepcompress_push_delta(
    struct stepcompress *sc, double print_time, double move_sd
    , double start_sv, double accel
    , double height, double startxy_sd, double arm_sd, double movez_r)
{
    double reversexy_sd = startxy_sd + arm_sd*movez_r;
    if (reversexy_sd <= 0.)
        // All steps are in down direction
        return _stepcompress_push_delta(
            sc, 0, print_time, move_sd, start_sv, accel
            , height, startxy_sd, arm_sd, movez_r);
    double movexy_r = movez_r ? sqrt(1. - movez_r*movez_r) : 1.;
    if (reversexy_sd >= move_sd * movexy_r)
        // All steps are in up direction
        return _stepcompress_push_delta(
            sc, 1, print_time, move_sd, start_sv, accel
            , height, startxy_sd, arm_sd, movez_r);
    // Steps in both up and down direction
    int res1 = _stepcompress_push_delta(
        sc, 1, print_time, reversexy_sd / movexy_r, start_sv, accel
        , height, startxy_sd, arm_sd, movez_r);
    if (res1 == ERROR_RET)
        return res1;
    int res2 = _stepcompress_push_delta(
        sc, 0, print_time, move_sd, start_sv, accel
        , height + res1, startxy_sd, arm_sd, movez_r);
    if (res2 == ERROR_RET)
        return res2;
    return res1 + res2;
}
