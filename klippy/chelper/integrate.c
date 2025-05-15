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

inline double
move_integrate_weighted(double base, double start_v, double half_accel
                        , double start, double end, double time_offset)
{
    // Calculate the definitive integral of the motion formula:
    //   position(t) = base + t * (start_v + t * half_accel)
    double half_v = .5 * start_v, sixth_a = (1. / 3.) * half_accel;
    double start_p_end = start + end;
    double end2 = end * end;
    double start_p_end_2 = start_p_end * start + end2;
    double iext_avg = base + half_v * start_p_end + sixth_a * start_p_end_2;

    // Calculate the definitive integral of time weighted position:
    //   weighted_position(t) = t * (base + t * (start_v + t * half_accel))
    double half_b = .5 * base, third_v = (1. / 3.) * start_v;
    double eighth_a = .25 * half_accel;
    double end3 = end2 * end;
    double start_p_end_3 = start_p_end_2 * start + end3;
    double wgt_ext_avg = half_b * start_p_end + third_v * start_p_end_2
        + eighth_a * start_p_end_3;

    return (wgt_ext_avg - time_offset * iext_avg) * (end - start);
}

inline double
move_no_accel_integrate_weighted(double base, double start_v
                                 , double start, double end, double time_offset)
{
    // Calculate the definitive integral of the motion formula:
    //   position(t) = base + t * start_v
    double half_v = .5 * start_v;
    double start_p_end = start + end;
    double iext_avg = base + half_v * start_p_end;

    // Calculate the definitive integral of time weighted position:
    //   weighted_position(t) = t * (base + t * start_v)
    double half_b = .5 * base, third_v = (1. / 3.) * start_v;
    double start_p_end_2 = start_p_end * start + end * end;
    double wgt_ext_avg = half_b * start_p_end + third_v * start_p_end_2;

    return (wgt_ext_avg - time_offset * iext_avg) * (end - start);
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
    return axis_r * move_no_accel_integrate_weighted(
            m->start_v, 2. * m->half_accel, start, end, time_offset);
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
