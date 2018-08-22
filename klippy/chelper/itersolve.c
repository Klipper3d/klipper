// Iterative solver for kinematic moves
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // sqrt
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct coord
#include "pyhelper.h" // errorf
#include "stepcompress.h" // queue_append_start


/****************************************************************
 * Kinematic moves
 ****************************************************************/

struct move * __visible
move_alloc(void)
{
    struct move *m = malloc(sizeof(*m));
    memset(m, 0, sizeof(*m));
    return m;
}

// Populate a 'struct move' with a velocity trapezoid
void __visible
move_fill(struct move *m, double print_time
          , double accel_t, double cruise_t, double decel_t
          , double start_pos_x, double start_pos_y, double start_pos_z
          , double axes_d_x, double axes_d_y, double axes_d_z
          , double start_v, double cruise_v, double accel)
{
    // Setup velocity trapezoid
    m->print_time = print_time;
    m->move_t = accel_t + cruise_t + decel_t;
    m->accel_t = accel_t;
    m->cruise_t = cruise_t;
    m->cruise_start_d = accel_t * .5 * (cruise_v + start_v);
    m->decel_start_d = m->cruise_start_d + cruise_t * cruise_v;

    // Setup for accel/cruise/decel phases
    m->cruise_v = cruise_v;
    m->accel.c1 = start_v;
    m->accel.c2 = .5 * accel;
    m->decel.c1 = cruise_v;
    m->decel.c2 = -m->accel.c2;

    // Setup for move_get_coord()
    m->start_pos.x = start_pos_x;
    m->start_pos.y = start_pos_y;
    m->start_pos.z = start_pos_z;
    double inv_move_d = 1. / sqrt(axes_d_x*axes_d_x + axes_d_y*axes_d_y
                                  + axes_d_z*axes_d_z);
    m->axes_r.x = axes_d_x * inv_move_d;
    m->axes_r.y = axes_d_y * inv_move_d;
    m->axes_r.z = axes_d_z * inv_move_d;
}

// Find the distance travel during acceleration/deceleration
static inline double
move_eval_accel(struct move_accel *ma, double move_time)
{
    return (ma->c1 + ma->c2 * move_time) * move_time;
}

// Return the distance moved given a time in a move
inline double
move_get_distance(struct move *m, double move_time)
{
    if (unlikely(move_time < m->accel_t))
        // Acceleration phase of move
        return move_eval_accel(&m->accel, move_time);
    move_time -= m->accel_t;
    if (likely(move_time <= m->cruise_t))
        // Cruising phase
        return m->cruise_start_d + m->cruise_v * move_time;
    // Deceleration phase
    move_time -= m->cruise_t;
    return m->decel_start_d + move_eval_accel(&m->decel, move_time);
}

// Return the XYZ coordinates given a time in a move
inline struct coord
move_get_coord(struct move *m, double move_time)
{
    double move_dist = move_get_distance(m, move_time);
    return (struct coord) {
        .x = m->start_pos.x + m->axes_r.x * move_dist,
        .y = m->start_pos.y + m->axes_r.y * move_dist,
        .z = m->start_pos.z + m->axes_r.z * move_dist };
}


/****************************************************************
 * Iterative solver
 ****************************************************************/

struct timepos {
    double time, position;
};

// Find step using "false position" method
static struct timepos
itersolve_find_step(struct stepper_kinematics *sk, struct move *m
                    , struct timepos low, struct timepos high
                    , double target)
{
    sk_callback calc_position = sk->calc_position;
    struct timepos best_guess = high;
    low.position -= target;
    high.position -= target;
    if (!high.position)
        // The high range was a perfect guess for the next step
        return best_guess;
    int high_sign = signbit(high.position);
    if (high_sign == signbit(low.position))
        // The target is not in the low/high range - return low range
        return (struct timepos){ low.time, target };
    for (;;) {
        double guess_time = ((low.time*high.position - high.time*low.position)
                             / (high.position - low.position));
        if (fabs(guess_time - best_guess.time) <= .000000001)
            break;
        best_guess.time = guess_time;
        best_guess.position = calc_position(sk, m, guess_time);
        double guess_position = best_guess.position - target;
        int guess_sign = signbit(guess_position);
        if (guess_sign == high_sign) {
            high.time = guess_time;
            high.position = guess_position;
        } else {
            low.time = guess_time;
            low.position = guess_position;
        }
    }
    return best_guess;
}

// Generate step times for a stepper during a move
int32_t __visible
itersolve_gen_steps(struct stepper_kinematics *sk, struct move *m)
{
    struct stepcompress *sc = sk->sc;
    sk_callback calc_position = sk->calc_position;
    double half_step = .5 * sk->step_dist;
    double mcu_freq = stepcompress_get_mcu_freq(sc);
    struct timepos last = { 0., sk->commanded_pos }, low = last, high = last;
    double seek_time_delta = 0.000100;
    int sdir = stepcompress_get_step_dir(sc);
    struct queue_append qa = queue_append_start(sc, m->print_time, .5);
    for (;;) {
        // Determine if next step is in forward or reverse direction
        double dist = high.position - last.position;
        if (fabs(dist) < half_step) {
        seek_new_high_range:
            if (high.time >= m->move_t)
                // At end of move
                break;
            // Need to increase next step search range
            low = high;
            high.time = last.time + seek_time_delta;
            seek_time_delta += seek_time_delta;
            if (high.time > m->move_t)
                high.time = m->move_t;
            high.position = calc_position(sk, m, high.time);
            continue;
        }
        int next_sdir = dist > 0.;
        if (unlikely(next_sdir != sdir)) {
            // Direction change
            if (fabs(dist) < half_step + .000000001)
                // Only change direction if going past midway point
                goto seek_new_high_range;
            if (last.time >= low.time && high.time > last.time) {
                // Must seek new low range to avoid re-finding previous time
                high.time = (last.time + high.time) * .5;
                high.position = calc_position(sk, m, high.time);
                continue;
            }
            int ret = queue_append_set_next_step_dir(&qa, next_sdir);
            if (ret)
                return ret;
            sdir = next_sdir;
        }
        // Find step
        double target = last.position + (sdir ? half_step : -half_step);
        struct timepos next = itersolve_find_step(sk, m, low, high, target);
        // Add step at given time
        int ret = queue_append(&qa, next.time * mcu_freq);
        if (ret)
            return ret;
        seek_time_delta = next.time - last.time;
        if (seek_time_delta < .000000001)
            seek_time_delta = .000000001;
        last.position = target + (sdir ? half_step : -half_step);
        last.time = next.time;
        low = next;
        if (last.time >= high.time)
            // The high range is no longer valid - recalculate it
            goto seek_new_high_range;
    }
    queue_append_finish(qa);
    sk->commanded_pos = last.position;
    return 0;
}

void __visible
itersolve_set_stepcompress(struct stepper_kinematics *sk
                           , struct stepcompress *sc, double step_dist)
{
    sk->sc = sc;
    sk->step_dist = step_dist;
}

double __visible
itersolve_calc_position_from_coord(struct stepper_kinematics *sk
                                   , double x, double y, double z)
{
    struct move m;
    memset(&m, 0, sizeof(m));
    move_fill(&m, 0., 0., 1., 0., x, y, z, 0., 1., 0., 0., 1., 0.);
    return sk->calc_position(sk, &m, 0.);
}

void __visible
itersolve_set_commanded_pos(struct stepper_kinematics *sk, double pos)
{
    sk->commanded_pos = pos;
}

double __visible
itersolve_get_commanded_pos(struct stepper_kinematics *sk)
{
    return sk->commanded_pos;
}
