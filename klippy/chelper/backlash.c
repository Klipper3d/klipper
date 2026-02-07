// A code for per-axis backlash compensation
//
// Copyright (C) 2026  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // fabs
#include <stddef.h> // offsetof
#include "backlash.h" // struct backlash_compensation
#include "compiler.h" // unlikely
#include "itersolve.h" // struct stepper_kinematics
#include "list.h" // list_node
#include "trapq.h" // struct move

#define EPS 1e-9

// Compute the sign of the move direction along the given axis
inline static int
get_move_dir(struct move *m, int axis)
{
    double axis_r = m->axes_r.axis[axis - 'x'];
    if (fabs(axis_r) < EPS ||
            (m->start_v < EPS && fabs(m->half_accel) < EPS)) {
        // The axis is not moving
        return 0;
    }
    return axis_r > 0. ? 1 : -1;
}

// Compute smooth weight function w(x) that gives w(0)=1, w(1)=0, and
// has zero derivatives of w'(0)=0, w'(1)=0.
inline static double
get_weight(double x)
{
    return x < 0.5 ? 1. - 2. * x * x : 2. * (1. - x) * (1. - x);
}

inline double
calc_backlash_compensation(struct backlash_compensation *bc, struct move *m
                           , int axis, double move_time, double extra_look_back)
{
    double smooth_time = bc->smooth_time;
    double axis_lag = bc->axis_lag[axis - 'x'];
    if (!smooth_time || !axis_lag) return 0.;
    // Find time to the nearest positive and negative motion of the given axis
    int move_dir = get_move_dir(m, axis);
    double ttp = move_dir > 0 ? 0. : smooth_time;
    double ttm = move_dir < 0 ? 0. : smooth_time;
    // Search through previous moves
    struct move *prev = m;
    double move_offset = move_time;
    // Make past motion direction sticky for extra_look_back duration,
    // but at most up to the next motion in the opposite direction
    double pstart = 0., mstart = 0.;
    while (move_offset < smooth_time + extra_look_back) {
        prev = list_prev_entry(prev, node);
        int prev_dir = get_move_dir(prev, axis);
        if (prev_dir != move_dir) {
            if (move_dir > 0) pstart = move_offset;
            if (move_dir < 0) mstart = move_offset;
        }
        move_dir = prev_dir;
        if (move_dir > 0) {
            double new_ttp = move_offset - extra_look_back;
            if (new_ttp < mstart) new_ttp = mstart;
            if (ttp > new_ttp) ttp = new_ttp;
        }
        if (move_dir < 0) {
            double new_ttm = move_offset - extra_look_back;
            if (new_ttm < pstart) new_ttm = pstart;
            if (ttm > new_ttm) ttm = new_ttm;
        }
        move_offset += prev->move_t;
    }
    // Search through future moves
    move_offset = m->move_t - move_time;
    while (unlikely(move_offset < ttp || move_offset < ttm)) {
        m = list_next_entry(m, node);
        move_dir = get_move_dir(m, axis);
        if (move_dir > 0 && move_offset < ttp)
            ttp = move_offset;
        if (move_dir < 0 && move_offset < ttm)
            ttm = move_offset;
        move_offset += m->move_t;
    }
    ttp *= bc->inv_smooth_time;
    ttm *= bc->inv_smooth_time;
    double weight = get_weight(ttp) - get_weight(ttm);
    return axis_lag * weight;
}
