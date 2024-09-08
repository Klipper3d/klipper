// Extruder stepper pulse time generation
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "list.h" // list_node
#include "pyhelper.h" // errorf
#include "trapq.h" // move_get_distance

struct pa_params {
    double pressure_advance, active_print_time;
    struct list_node node;
};

// Without pressure advance, the extruder stepper position is:
//     extruder_position(t) = nominal_position(t)
// When pressure advance is enabled, additional filament is pushed
// into the extruder during acceleration (and retracted during
// deceleration). The formula is:
//     pa_position(t) = (nominal_position(t)
//                       + pressure_advance * nominal_velocity(t))
// Which is then "smoothed" using a weighted average:
//     smooth_position(t) = (
//         definitive_integral(pa_position(x) * (smooth_time/2 - abs(t-x)) * dx,
//                             from=t-smooth_time/2, to=t+smooth_time/2)
//         / ((smooth_time/2)**2))

// Calculate the definitive integral of the motion formula:
//   position(t) = base + t * (start_v + t * half_accel)
static double
extruder_integrate(double base, double start_v, double half_accel
                   , double start, double end)
{
    double half_v = .5 * start_v, sixth_a = (1. / 3.) * half_accel;
    double si = start * (base + start * (half_v + start * sixth_a));
    double ei = end * (base + end * (half_v + end * sixth_a));
    return ei - si;
}

// Calculate the definitive integral of time weighted position:
//   weighted_position(t) = t * (base + t * (start_v + t * half_accel))
static double
extruder_integrate_time(double base, double start_v, double half_accel
                        , double start, double end)
{
    double half_b = .5 * base, third_v = (1. / 3.) * start_v;
    double eighth_a = .25 * half_accel;
    double si = start * start * (half_b + start * (third_v + start * eighth_a));
    double ei = end * end * (half_b + end * (third_v + end * eighth_a));
    return ei - si;
}

// Calculate the definitive integral of extruder for a given move
static double
pa_move_integrate(struct move *m, struct list_head *pa_list
                  , double base, double start, double end, double time_offset)
{
    if (start < 0.)
        start = 0.;
    if (end > m->move_t)
        end = m->move_t;
    // Determine pressure_advance value
    int can_pressure_advance = m->axes_r.y != 0.;
    double pressure_advance = 0.;
    if (can_pressure_advance) {
        struct pa_params *pa = list_last_entry(pa_list, struct pa_params, node);
        while (unlikely(pa->active_print_time > m->print_time) &&
                !list_is_first(&pa->node, pa_list)) {
            pa = list_prev_entry(pa, node);
        }
        pressure_advance = pa->pressure_advance;
    }
    // Calculate base position and velocity with pressure advance
    base += pressure_advance * m->start_v;
    double start_v = m->start_v + pressure_advance * 2. * m->half_accel;
    // Calculate definitive integral
    double ha = m->half_accel;
    double iext = extruder_integrate(base, start_v, ha, start, end);
    double wgt_ext = extruder_integrate_time(base, start_v, ha, start, end);
    return wgt_ext - time_offset * iext;
}

// Calculate the definitive integral of the extruder over a range of moves
static double
pa_range_integrate(struct move *m, double move_time
                   , struct list_head *pa_list, double hst)
{
    // Calculate integral for the current move
    double res = 0., start = move_time - hst, end = move_time + hst;
    double start_base = m->start_pos.x;
    res += pa_move_integrate(m, pa_list, 0., start, move_time, start);
    res -= pa_move_integrate(m, pa_list, 0., move_time, end, end);
    // Integrate over previous moves
    struct move *prev = m;
    while (unlikely(start < 0.)) {
        prev = list_prev_entry(prev, node);
        start += prev->move_t;
        double base = prev->start_pos.x - start_base;
        res += pa_move_integrate(prev, pa_list, base, start
                                 , prev->move_t, start);
    }
    // Integrate over future moves
    while (unlikely(end > m->move_t)) {
        end -= m->move_t;
        m = list_next_entry(m, node);
        double base = m->start_pos.x - start_base;
        res -= pa_move_integrate(m, pa_list, base, 0., end, end);
    }
    return res;
}

struct extruder_stepper {
    struct stepper_kinematics sk;
    struct list_head pa_list;
    double half_smooth_time, inv_half_smooth_time2;
};

static double
extruder_calc_position(struct stepper_kinematics *sk, struct move *m
                       , double move_time)
{
    struct extruder_stepper *es = container_of(sk, struct extruder_stepper, sk);
    double hst = es->half_smooth_time;
    if (!hst)
        // Pressure advance not enabled
        return m->start_pos.x + move_get_distance(m, move_time);
    // Apply pressure advance and average over smooth_time
    double area = pa_range_integrate(m, move_time, &es->pa_list, hst);
    return m->start_pos.x + area * es->inv_half_smooth_time2;
}

void __visible
extruder_set_pressure_advance(struct stepper_kinematics *sk, double print_time
                              , double pressure_advance, double smooth_time)
{
    struct extruder_stepper *es = container_of(sk, struct extruder_stepper, sk);
    double hst = smooth_time * .5, old_hst = es->half_smooth_time;
    es->half_smooth_time = hst;
    es->sk.gen_steps_pre_active = es->sk.gen_steps_post_active = hst;

    // Cleanup old pressure advance parameters
    double cleanup_time = sk->last_flush_time - (old_hst > hst ? old_hst : hst);
    struct pa_params *first_pa = list_first_entry(
            &es->pa_list, struct pa_params, node);
    while (!list_is_last(&first_pa->node, &es->pa_list)) {
        struct pa_params *next_pa = list_next_entry(first_pa, node);
        if (next_pa->active_print_time >= cleanup_time) break;
        list_del(&first_pa->node);
        first_pa = next_pa;
    }

    if (! hst)
        return;
    es->inv_half_smooth_time2 = 1. / (hst * hst);

    if (list_last_entry(&es->pa_list, struct pa_params, node)->pressure_advance
            == pressure_advance) {
        // Retain old pa_params
        return;
    }
    // Add new pressure advance parameters
    struct pa_params *pa = malloc(sizeof(*pa));
    memset(pa, 0, sizeof(*pa));
    pa->pressure_advance = pressure_advance;
    pa->active_print_time = print_time;
    list_add_tail(&pa->node, &es->pa_list);
}

struct stepper_kinematics * __visible
extruder_stepper_alloc(void)
{
    struct extruder_stepper *es = malloc(sizeof(*es));
    memset(es, 0, sizeof(*es));
    es->sk.calc_position_cb = extruder_calc_position;
    es->sk.active_flags = AF_X;
    list_init(&es->pa_list);
    struct pa_params *pa = malloc(sizeof(*pa));
    memset(pa, 0, sizeof(*pa));
    list_add_tail(&pa->node, &es->pa_list);
    return &es->sk;
}

void __visible
extruder_stepper_free(struct stepper_kinematics *sk)
{
    struct extruder_stepper *es = container_of(sk, struct extruder_stepper, sk);
    while (!list_empty(&es->pa_list)) {
        struct pa_params *pa = list_first_entry(
                &es->pa_list, struct pa_params, node);
        list_del(&pa->node);
        free(pa);
    }
    free(sk);
}
