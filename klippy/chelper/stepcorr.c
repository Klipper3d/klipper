// Stepper position correction utilities
//
// Copyright (C) 2025  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h>  // atan
#include <stddef.h> // offsetof
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "stepcorr.h" // struct stepper_corrections
#include "trapq.h" // struct move

static inline double
calc_stealthchop_time_lag(struct stepper_corrections *sc, double velocity)
{
    double omega = sc->rad_per_mm * velocity;
    double tan_phi = omega * sc->stealthchop_lag_const;
    if (fabs(tan_phi) > 0.0001) return atan(tan_phi) / omega;
    // Use Taylor expansion for small tan_phi values
    return sc->stealthchop_lag_const * (1. - (1./3.) * tan_phi * tan_phi);
}

static inline double
calc_spreadcycle_position_lag(struct stepper_corrections *sc, double velocity)
{
    double phase_lag = sc->spreadcycle_lag_const * velocity;
    if (phase_lag > 1.)
        phase_lag = 1.;
    return asin(phase_lag) / sc->rad_per_mm;
}

static inline struct move*
find_move_at_time(struct move *m, double* move_time)
{
    while (likely(*move_time < 0.)) {
        m = list_prev_entry(m, node);
        *move_time += m->move_t;
    }
    while (likely(*move_time > m->move_t)) {
        *move_time -= m->move_t;
        m = list_next_entry(m, node);
    }
    return m;
}

inline double
stepcorr_calc_position(struct stepper_kinematics *sk, struct move *m
                       , double move_time)
{
    struct stepper_corrections *sc = &sk->step_corr;
    if ((!sc->stealthchop_lag_const && !sc->spreadcycle_lag_const)
           || !sk->calc_smoothed_velocity_cb)
        return sk->calc_position_cb(sk, m, move_time);
    double velocity = sk->calc_smoothed_velocity_cb(sk, m, move_time, sc->hst);
    move_time += calc_stealthchop_time_lag(sc, velocity);
    m = find_move_at_time(m, &move_time);
    return sk->calc_position_cb(sk, m, move_time)
        + calc_spreadcycle_position_lag(sc, velocity);
}

inline void
stepcorr_update_gen_steps_window(struct stepper_kinematics *sk)
{
    struct stepper_corrections *sc = &sk->step_corr;
    sk->gen_steps_pre_active += sc->stealthchop_lag_const + sc->hst;
    sk->gen_steps_post_active += sc->hst;
}

int __visible
stepcorr_set_lag_correction(struct stepper_kinematics *sk, double rad_per_mm
                            , double stealthchop_lag_const
                            , double spreadcycle_lag_const
                            , double velocity_smooth_time)
{
    struct stepper_corrections *sc = &sk->step_corr;
    if (!sk->calc_smoothed_velocity_cb && (
                stealthchop_lag_const || spreadcycle_lag_const))
        return -1;
    if (velocity_smooth_time < 0)
        return -1;
    sk->gen_steps_pre_active -= sc->stealthchop_lag_const + sc->hst;
    sk->gen_steps_post_active -= sc->hst;
    sc->rad_per_mm = rad_per_mm;
    sc->stealthchop_lag_const = stealthchop_lag_const;
    sc->spreadcycle_lag_const = spreadcycle_lag_const;
    sc->hst = 0.5 * velocity_smooth_time;
    stepcorr_update_gen_steps_window(sk);
    return 0;
}
