// Extruder stepper pulse time generation
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // tanh
#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "kin_shaper.h" // struct shaper_pulses
#include "pyhelper.h" // errorf
#include "trapq.h" // move_get_distance

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

// Calculate the definitive integral of time weighted position:
//   weighted_position(t) = t * (base + t * (start_v + t * half_accel))
//              - time_offset * (base + t * (start_v + t * half_accel))
inline static double
extruder_integrate_weighted(double base, double start_v, double half_accel
                            , double start, double end, double time_offset)
{
    double delta_t = end - start;
    double end2 = end * end;
    double start2 = start * start;
    double c1 = .5 * (end + start);
    double c2 = 1./3. * (end2 + end * start + start2);
    double c3 = .5 * c1 * (end2 + start2);
    double avg_val = base * (c1 - time_offset)
        + start_v * (c2 - c1 * time_offset)
        + half_accel * (c3 - c2 * time_offset);
    return delta_t * avg_val;
}

// Calculate the definitive integral of extruder for a given move
static void
pa_move_integrate(struct move *m, int axis
                  , double base, double start, double end, double time_offset
                  , double *pos_integral, double *pa_velocity_integral)
{
    if (start < 0.)
        start = 0.;
    if (end > m->move_t)
        end = m->move_t;
    // Calculate base position and velocity with pressure advance
    int can_pressure_advance = m->axes_r.x > 0. || m->axes_r.y > 0.;
    double axis_r = m->axes_r.axis[axis - 'x'];
    double start_v = m->start_v * axis_r;
    double ha = m->half_accel * axis_r;
    // Calculate definitive integral
    *pos_integral = extruder_integrate_weighted(
            base, start_v, ha, start, end, time_offset);
    if (!can_pressure_advance) {
        *pa_velocity_integral = 0.;
    } else {
        *pa_velocity_integral = extruder_integrate_weighted(
                start_v, 2. * ha, 0., start, end, time_offset);
    }
}

// Calculate the definitive integral of the extruder over a range of moves
static void
pa_range_integrate(struct move *m, int axis, double move_time, double hst
                   , double *pos_integral, double *pa_velocity_integral)
{
    while (unlikely(move_time < 0.)) {
        m = list_prev_entry(m, node);
        move_time += m->move_t;
    }
    while (unlikely(move_time > m->move_t)) {
        move_time -= m->move_t;
        m = list_next_entry(m, node);
    }
    // Calculate integral for the current move
    *pos_integral = *pa_velocity_integral = 0.;
    double m_pos_int, m_pa_vel_int;
    double start = move_time - hst, end = move_time + hst;
    double start_base = m->start_pos.axis[axis - 'x'];
    pa_move_integrate(m, axis, 0., start, move_time, start,
                      &m_pos_int, &m_pa_vel_int);
    *pos_integral += m_pos_int;
    *pa_velocity_integral += m_pa_vel_int;
    pa_move_integrate(m, axis, 0., move_time, end, end,
                      &m_pos_int, &m_pa_vel_int);
    *pos_integral -= m_pos_int;
    *pa_velocity_integral -= m_pa_vel_int;
    // Integrate over previous moves
    struct move *prev = m;
    while (unlikely(start < 0.)) {
        prev = list_prev_entry(prev, node);
        start += prev->move_t;
        double base = prev->start_pos.axis[axis - 'x'] - start_base;
        pa_move_integrate(prev, axis, base, start, prev->move_t, start,
                          &m_pos_int, &m_pa_vel_int);
        *pos_integral += m_pos_int;
        *pa_velocity_integral += m_pa_vel_int;
    }
    // Integrate over future moves
    while (unlikely(end > m->move_t)) {
        end -= m->move_t;
        m = list_next_entry(m, node);
        double base = m->start_pos.axis[axis - 'x'] - start_base;
        pa_move_integrate(m, axis, base, 0., end, end,
                          &m_pos_int, &m_pa_vel_int);
        *pos_integral -= m_pos_int;
        *pa_velocity_integral -= m_pa_vel_int;
    }
    *pos_integral += start_base * hst * hst;
}

static void
shaper_pa_range_integrate(struct move *m, int axis, double move_time
                          , double hst, struct shaper_pulses *sp
                          , double *pos_integral, double *pa_velocity_integral)
{
    *pos_integral = *pa_velocity_integral = 0.;
    int num_pulses = sp->num_pulses, i;
    for (i = 0; i < num_pulses; ++i) {
        double t = sp->pulses[i].t, a = sp->pulses[i].a;
        double p_pos_int, p_pa_vel_int;
        pa_range_integrate(m, axis, move_time + t, hst,
                           &p_pos_int, &p_pa_vel_int);
        *pos_integral += a * p_pos_int;
        *pa_velocity_integral += a * p_pa_vel_int;
    }
}

struct pressure_advance_params {
    union {
        struct {
            double pressure_advance;
        };
        struct {
            double linear_advance, linear_offset, linearization_velocity;
        };
        double params[3];
    };
};

typedef double (*pressure_advance_func)(
        double, double, struct pressure_advance_params *pa_params);

struct extruder_stepper {
    struct stepper_kinematics sk;
    struct shaper_pulses sp[3];
    struct pressure_advance_params pa_params;
    pressure_advance_func pa_func;
    double time_offset, half_smooth_time, inv_half_smooth_time2;
};

double __visible
pressure_advance_linear_model_func(double position, double pa_velocity
                                   , struct pressure_advance_params *pa_params)
{
    return position + pa_velocity * pa_params->pressure_advance;
}

double __visible
pressure_advance_tanh_model_func(double position, double pa_velocity
                                 , struct pressure_advance_params *pa_params)
{
    position += pa_params->linear_advance * pa_velocity;
    if (pa_params->linear_offset) {
        double rel_velocity = pa_velocity / pa_params->linearization_velocity;
        position += pa_params->linear_offset * tanh(rel_velocity);
    }
    return position;
}

double __visible
pressure_advance_recipr_model_func(double position, double pa_velocity
                                   , struct pressure_advance_params *pa_params)
{
    position += pa_params->linear_advance * pa_velocity;
    if (pa_params->linear_offset) {
        double rel_velocity = pa_velocity / pa_params->linearization_velocity;
        position += pa_params->linear_offset * (1. - 1. / (1. + rel_velocity));
    }
    return position;
}

static double
extruder_calc_position(struct stepper_kinematics *sk, struct move *m
                       , double move_time)
{
    struct extruder_stepper *es = container_of(sk, struct extruder_stepper, sk);
    move_time += es->time_offset;
    while (unlikely(move_time < 0.)) {
        m = list_prev_entry(m, node);
        move_time += m->move_t;
    }
    while (unlikely(move_time >= m->move_t)) {
        move_time -= m->move_t;
        m = list_next_entry(m, node);
    }
    double hst = es->half_smooth_time;
    int i;
    struct coord e_pos, pa_vel;
    double move_dist = move_get_distance(m, move_time);
    for (i = 0; i < 3; ++i) {
        int axis = 'x' + i;
        struct shaper_pulses* sp = &es->sp[i];
        int num_pulses = sp->num_pulses;
        if (!hst) {
            e_pos.axis[i] = num_pulses
                ? shaper_calc_position(m, axis, move_time, sp)
                : m->start_pos.axis[i] + m->axes_r.axis[i] * move_dist;
            pa_vel.axis[i] = 0.;
        } else {
            if (num_pulses) {
                shaper_pa_range_integrate(m, axis, move_time, hst, sp,
                                          &e_pos.axis[i], &pa_vel.axis[i]);
            } else {
                pa_range_integrate(m, axis, move_time, hst,
                                   &e_pos.axis[i], &pa_vel.axis[i]);
            }
            e_pos.axis[i] *= es->inv_half_smooth_time2;
            pa_vel.axis[i] *= es->inv_half_smooth_time2;
        }
    }
    double position = e_pos.x + e_pos.y + e_pos.z;
    if (!hst)
        return position;
    double pa_velocity = pa_vel.x + pa_vel.y + pa_vel.z;
    if (pa_velocity < 0.) pa_velocity = 0.;
    return es->pa_func(position, pa_velocity, &es->pa_params);
}

static void
extruder_note_generation_time(struct extruder_stepper *es)
{
    double pre_active = 0., post_active = 0.;
    int i;
    for (i = 0; i < 2; ++i) {
        struct shaper_pulses* sp = &es->sp[i];
        if (!es->sp[i].num_pulses) continue;
        pre_active = sp->pulses[sp->num_pulses-1].t > pre_active
            ? sp->pulses[sp->num_pulses-1].t : pre_active;
        post_active = -sp->pulses[0].t > post_active
            ? -sp->pulses[0].t : post_active;
    }
    pre_active += es->half_smooth_time + es->time_offset;
    if (pre_active < 0.) pre_active = 0.;
    post_active += es->half_smooth_time - es->time_offset;
    if (post_active < 0.) post_active = 0.;
    es->sk.gen_steps_pre_active = pre_active;
    es->sk.gen_steps_post_active = post_active;
}

void __visible
extruder_set_pressure_advance(struct stepper_kinematics *sk
                              , int n_params, double params[]
                              , double smooth_time, double time_offset)
{
    struct extruder_stepper *es = container_of(sk, struct extruder_stepper, sk);
    double hst = smooth_time * .5;
    es->half_smooth_time = hst;
    es->time_offset = time_offset;
    extruder_note_generation_time(es);
    if (! hst)
        return;
    es->inv_half_smooth_time2 = 1. / (hst * hst);
    memset(&es->pa_params, 0, sizeof(es->pa_params));
    if (n_params < 0 || n_params > ARRAY_SIZE(es->pa_params.params))
        return;
    memcpy(&es->pa_params, params, n_params * sizeof(params[0]));
}

void __visible
extruder_set_pressure_advance_model_func(struct stepper_kinematics *sk
                                         , pressure_advance_func func)
{
    struct extruder_stepper *es = container_of(sk, struct extruder_stepper, sk);
    memset(&es->pa_params, 0, sizeof(es->pa_params));
    es->pa_func = func;
}

int __visible
extruder_set_shaper_params(struct stepper_kinematics *sk, char axis
                           , int n, double a[], double t[])
{
    if (axis != 'x' && axis != 'y')
        return -1;
    struct extruder_stepper *es = container_of(sk, struct extruder_stepper, sk);
    struct shaper_pulses *sp = &es->sp[axis-'x'];
    int status = init_shaper(n, a, t, sp);
    extruder_note_generation_time(es);
    return status;
}

double __visible
extruder_get_step_gen_window(struct stepper_kinematics *sk)
{
    struct extruder_stepper *es = container_of(sk, struct extruder_stepper, sk);
    return es->sk.gen_steps_pre_active > es->sk.gen_steps_post_active
         ? es->sk.gen_steps_pre_active : es->sk.gen_steps_post_active;
}

struct stepper_kinematics * __visible
extruder_stepper_alloc(void)
{
    struct extruder_stepper *es = malloc(sizeof(*es));
    memset(es, 0, sizeof(*es));
    es->sk.calc_position_cb = extruder_calc_position;
    es->pa_func = pressure_advance_tanh_model_func;
    es->sk.active_flags = AF_X | AF_Y | AF_Z;
    return &es->sk;
}
