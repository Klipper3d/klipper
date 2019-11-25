// Iterative solver for kinematic moves
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // fabs
#include <stddef.h> // offsetof
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // itersolve_generate_steps
#include "pyhelper.h" // errorf
#include "stepcompress.h" // queue_append_start
#include "trapq.h" // struct move

struct timepos {
    double time, position;
};

// Find step using "false position" method
static struct timepos
itersolve_find_step(struct stepper_kinematics *sk, struct move *m
                    , struct timepos low, struct timepos high
                    , double target)
{
    sk_calc_callback calc_position_cb = sk->calc_position_cb;
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
        best_guess.position = calc_position_cb(sk, m, guess_time);
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

// Generate step times for a portion of a move
static int32_t
itersolve_gen_steps_range(struct stepper_kinematics *sk, struct move *m
                          , double move_start, double move_end)
{
    struct stepcompress *sc = sk->sc;
    sk_calc_callback calc_position_cb = sk->calc_position_cb;
    double half_step = .5 * sk->step_dist;
    double mcu_freq = stepcompress_get_mcu_freq(sc);
    double start = move_start - m->print_time, end = move_end - m->print_time;
    struct timepos last = { start, sk->commanded_pos }, low = last, high = last;
    double seek_time_delta = 0.000100;
    int sdir = stepcompress_get_step_dir(sc);
    struct queue_append qa = queue_append_start(sc, m->print_time, .5);
    for (;;) {
        // Determine if next step is in forward or reverse direction
        double dist = high.position - last.position;
        if (fabs(dist) < half_step) {
        seek_new_high_range:
            if (high.time >= end)
                // At end of move
                break;
            // Need to increase next step search range
            low = high;
            high.time = last.time + seek_time_delta;
            seek_time_delta += seek_time_delta;
            if (high.time > end)
                high.time = end;
            high.position = calc_position_cb(sk, m, high.time);
            continue;
        }
        int next_sdir = dist > 0.;
        if (unlikely(next_sdir != sdir)) {
            // Direction change
            if (fabs(dist) < half_step + .000000001)
                // Only change direction if going past midway point
                goto seek_new_high_range;
            if (last.time >= low.time) {
                // Must seek new low range to avoid re-finding previous time
                if (high.time < last.time + .000000001)
                    goto seek_new_high_range;
                high.time = (last.time + high.time) * .5;
                high.position = calc_position_cb(sk, m, high.time);
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
    if (sk->post_cb)
        sk->post_cb(sk);
    return 0;
}

// Check if a move is likely to cause movement on a stepper
static inline int
check_active(struct stepper_kinematics *sk, struct move *m)
{
    int af = sk->active_flags;
    return ((af & AF_X && m->axes_r.x != 0.)
            || (af & AF_Y && m->axes_r.y != 0.)
            || (af & AF_Z && m->axes_r.z != 0.));
}

// Generate step times for a range of moves on the trapq
int32_t __visible
itersolve_generate_steps(struct stepper_kinematics *sk, double flush_time)
{
    double last_flush_time = sk->last_flush_time;
    sk->last_flush_time = flush_time;
    if (!sk->tq)
        return 0;
    trapq_check_sentinels(sk->tq);
    struct move *m = list_first_entry(&sk->tq->moves, struct move, node);
    while (last_flush_time >= m->print_time + m->move_t)
        m = list_next_entry(m, node);
    double force_steps_time = sk->last_move_time + sk->scan_past;
    for (;;) {
        if (last_flush_time >= flush_time)
            return 0;
        double start = m->print_time, end = start + m->move_t;
        if (start < last_flush_time)
            start = last_flush_time;
        if (end > flush_time)
            end = flush_time;
        if (check_active(sk, m)) {
            if (sk->scan_future && start > last_flush_time) {
                // Must generate steps leading up to stepper activity
                force_steps_time = start;
                if (last_flush_time < start - sk->scan_future)
                    last_flush_time = start - sk->scan_future;
                while (m->print_time > last_flush_time)
                    m = list_prev_entry(m, node);
                continue;
            }
            // Generate steps for this move
            int32_t ret = itersolve_gen_steps_range(sk, m, start, end);
            if (ret)
                return ret;
            sk->last_move_time = last_flush_time = end;
            force_steps_time = end + sk->scan_past;
        } else if (start < force_steps_time) {
            // Must generates steps just past stepper activity
            if (end > force_steps_time)
                end = force_steps_time;
            int32_t ret = itersolve_gen_steps_range(sk, m, start, end);
            if (ret)
                return ret;
            last_flush_time = end;
        }
        if (flush_time + sk->scan_future <= m->print_time + m->move_t)
            return 0;
        m = list_next_entry(m, node);
    }
}

// Check if the given stepper is likely to be active in the given time range
double __visible
itersolve_check_active(struct stepper_kinematics *sk, double flush_time)
{
    if (!sk->tq)
        return 0.;
    trapq_check_sentinels(sk->tq);
    struct move *m = list_first_entry(&sk->tq->moves, struct move, node);
    while (sk->last_flush_time >= m->print_time + m->move_t)
        m = list_next_entry(m, node);
    for (;;) {
        if (check_active(sk, m))
            return m->print_time;
        if (flush_time <= m->print_time + m->move_t)
            return 0.;
        m = list_next_entry(m, node);
    }
}

void __visible
itersolve_set_trapq(struct stepper_kinematics *sk, struct trapq *tq)
{
    sk->tq = tq;
}

void __visible
itersolve_set_stepcompress(struct stepper_kinematics *sk
                           , struct stepcompress *sc, double step_dist)
{
    sk->sc = sc;
    sk->step_dist = step_dist;
}

static double
itersolve_calc_position_from_coord(struct stepper_kinematics *sk
                                   , double x, double y, double z)
{
    struct move m;
    memset(&m, 0, sizeof(m));
    m.start_pos.x = x;
    m.start_pos.y = y;
    m.start_pos.z = z;
    m.move_t = 1000.;
    return sk->calc_position_cb(sk, &m, 500.);
}

void __visible
itersolve_set_position(struct stepper_kinematics *sk
                       , double x, double y, double z)
{
    sk->commanded_pos = itersolve_calc_position_from_coord(sk, x, y, z);
}

double __visible
itersolve_get_commanded_pos(struct stepper_kinematics *sk)
{
    return sk->commanded_pos;
}
