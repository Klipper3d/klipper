// Move integration utilities
//
// Copyright (C) 2018-2025  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2025  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stddef.h> // offsetof
#include "compiler.h" // unlikely
#include "integrate.h" // struct stepper_kinematics
#include "list.h" // list_node
#include "trapq.h" // move_get_velocity

// Calculate the definitive integral of the motion formula:
//   position(t) = base + t * (start_v + t * half_accel)
static inline double
move_integrate(double base, double start_v, double half_accel
               , double start, double end)
{
    double half_v = .5 * start_v, sixth_a = (1. / 3.) * half_accel;
    double si = start * (base + start * (half_v + start * sixth_a));
    double ei = end * (base + end * (half_v + end * sixth_a));
    return ei - si;
}

// Calculate the definitive integral of time weighted position:
//   weighted_position(t) = t * (base + t * (start_v + t * half_accel))
static inline double
move_integrate_time(double base, double start_v, double half_accel
                    , double start, double end)
{
    double half_b = .5 * base, third_v = (1. / 3.) * start_v;
    double eighth_a = .25 * half_accel;
    double si = start * start * (half_b + start * (third_v + start * eighth_a));
    double ei = end * end * (half_b + end * (third_v + end * eighth_a));
    return ei - si;
}

inline double
move_integrate_weighted(double base, double start_v, double half_accel
                        , double start, double end, double time_offset)
{
    double iext = move_integrate(base, start_v, half_accel, start, end);
    double wgt_ext = move_integrate_time(base, start_v, half_accel, start, end);
    return wgt_ext - time_offset * iext;
}

static inline double
move_velocity_integrate(struct move *m, int axis, double start, double end
                        , double time_offset)
{
    if (start < 0.)
        start = 0.;
    if (end > m->move_t)
        end = m->move_t;
    // Calculate definitive integral
    double axis_r = m->axes_r.axis[axis - 'x'];
    return axis_r * move_integrate_weighted(
            m->start_v, 2. * m->half_accel, 0., start, end, time_offset);
}

inline double
calc_smoothed_velocity(struct move* m, int axis, double move_time, double hst)
{
    if (!hst)
        return m->axes_r.axis[axis - 'x'] * move_get_velocity(m, move_time);
    double inv_half_smooth_time2 = 1. / (hst * hst);
    // Calculate integral for the current move
    double res = 0., start = move_time - hst, end = move_time + hst;
    res += move_velocity_integrate(m, axis, start, move_time, start);
    res -= move_velocity_integrate(m, axis, move_time, end, end);
    // Integrate over previous moves
    struct move *prev = m;
    while (unlikely(start < 0.)) {
        prev = list_prev_entry(prev, node);
        start += prev->move_t;
        res += move_velocity_integrate(prev, axis, start, prev->move_t, start);
    }
    // Integrate over future moves
    while (unlikely(end > m->move_t)) {
        end -= m->move_t;
        m = list_next_entry(m, node);
        res -= move_velocity_integrate(m, axis, 0., end, end);
    }
    return res * inv_half_smooth_time2;
}
