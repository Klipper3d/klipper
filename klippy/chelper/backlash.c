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

inline static double
backlash_integrate(double value, double start, double end)
{
    return value * (end - start);
}

inline static double
backlash_integrate_time(double value, double start, double end)
{
    return value * (end - start) * (end + start) * 0.5;
}

inline static double
backlash_move_integrate(struct move *m, int axis, double start, double end
                        , double time_offset)
{
    if (start < 0.)
        start = 0.;
    if (end > m->move_t)
        end = m->move_t;
    double axis_r = m->axes_r.axis[axis - 'x'];
    if (fabs(axis_r) < EPS ||
            (m->start_v < EPS && fabs(m->half_accel) < EPS)) {
        // The axis is not moving
        return 0.;
    }
    double value = axis_r > 0. ? 1. : -1.;
    double iext = backlash_integrate(value, start, end);
    double wgt_ext = backlash_integrate_time(value, start, end);
    return wgt_ext - time_offset * iext;
}

// Calculate the definitive integral of the velocity sign over a range of moves
inline static double
backlash_range_integrate(struct move *m, int axis, double move_time, double hst)
{
    // Calculate integral for the current move
    double res = 0., start = move_time - hst, end = move_time + hst;
    res += backlash_move_integrate(m, axis, start, move_time, start);
    res -= backlash_move_integrate(m, axis, move_time, end, end);
    // Integrate over previous moves
    struct move *prev = m;
    while (unlikely(start < 0.)) {
        prev = list_prev_entry(prev, node);
        start += prev->move_t;
        res += backlash_move_integrate(prev, axis, start, prev->move_t, start);
    }
    // Integrate over future moves
    while (unlikely(end > m->move_t)) {
        end -= m->move_t;
        m = list_next_entry(m, node);
        res -= backlash_move_integrate(m, axis, 0., end, end);
    }
    return res;
}

inline double
calc_backlash_compensation(struct backlash_compensation *bc, struct move *m
                           , int axis, double move_time)
{
    double hst = bc->half_smooth_time;
    double axis_lag = bc->axis_lag[axis - 'x'];
    if (!hst || !axis_lag) return 0.;
    double area = backlash_range_integrate(m, axis, move_time, hst);
    return axis_lag * area * bc->inv_half_smooth_time2;
}
