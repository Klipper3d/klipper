// Stepper pulse schedule compression
//
// Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2022-2023  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// The goal of this code is to take a series of scheduled stepper
// pulse times and compress them into a handful of commands that can
// be efficiently transmitted and executed on a microcontroller (mcu).
// The mcu accepts step pulse commands that take interval, count, and
// add parameters such that 'count' pulses occur, with each step event
// calculating the next step event time using:
//  next_wake_time = last_wake_time + interval; interval += add
// This code is written in C (instead of python) for processing
// efficiency - the repetitive integer math is vastly faster in C.

#include <math.h> // sqrt
#include <stddef.h> // offsetof
#include <stdint.h> // uint32_t
#include <stdio.h> // fprintf
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // DIV_ROUND_UP
#include "pyhelper.h" // errorf
#include "serialqueue.h" // struct queue_message
#include "stepcompress.h" // stepcompress_alloc

// Maximum error between compressed and actual step,
// in the form of (step_i - step_{i-1}) >> MAX_ERR_2P
#define MAX_ERR_2P 6
// Minimum tolerable step error in clock ticks. Step errors
// below this value are ignored as they are getting close
// to the integer precision of a single clock tick.
#define MIN_STEP_ERR 3
// Limits on step_move values for the least squares method
#define MAX_COUNT_LSM 1024
// Maximum 'count' value for binary search, must be smaller
// than the MAX_COUNT_LSM value
#define MAX_COUNT_BISECT 512
// Limits below are optimized for "message blocks" encoding
// and underlying storage types and MCU-side implementation
#define MAX_INTRVL 0x3FFFFFF
#define MAX_ADD 0x7FFF
#define MAX_ADD2 0xFFF
#define MAX_SHIFT 16
#define MIN_SHIFT -8
#define MAX_INT32 0x7FFFFFFF

struct step_move_hp {
    uint32_t interval;
    uint16_t count;
    int16_t add;
    int16_t add2;
    int8_t shift;
    uint64_t first_step, last_step;
    uint32_t next_step_interval;
};

/****************************************************************
 * Step compression
 ****************************************************************/

// A bias of the first step in the generated step sequence step_move towards
// the first required step in the least squares method. The zero value means
// no bias: the first step error is minimized as all other step errors, and
// values larger than zero make the method push the first generated step
// towards stepcompress.next_expected_interval. This helps reducing the jitter
// between subsequent step_move(s) and pushing back the maximum errors towards
// the end of the generated step_move sequence.
#define FIRST_STEP_BIAS 1.0
// Extra bias for the first step towards the previously expected step to try
// if the step_move generated using the regular parameters fail early.
#define EXTRA_FIRST_STEP_BIAS 19.0

#define A2_REGULARIZATION 0.01

struct matrix_3x3 {
    double a00, a10, a11, a20, a21, a22;
};

struct rhs_3 {
    double b0, b1, b2;
};

struct matrix_3x3 least_squares_ldl[MAX_COUNT_LSM] = {0};
struct matrix_3x3 least_squares_efsb_ldl[MAX_COUNT_LSM] = {0};

static void
fill_least_squares_matrix_3x3(uint16_t count, struct matrix_3x3 *m)
{
    int32_t i;
    memset(m, 0, sizeof(*m));
    for (i = 0; i < count; ++i) {
        int32_t c0 = i+1;
        int32_t c1 = c0 * i / 2;
        int32_t c2 = c1 * (i-1) / 3;

        m->a00 += (double)c0 * c0;
        m->a10 += (double)c1 * c0;
        m->a11 += (double)c1 * c1;
        m->a20 += (double)c2 * c0;
        m->a21 += (double)c2 * c1;
        m->a22 += (double)c2 * c2;
    }
    m->a00 += FIRST_STEP_BIAS;
    m->a22 += A2_REGULARIZATION;
    if (i < 2) m->a11 = 1.;
    if (i < 3) m->a22 = 1.;
}

static void
compute_ldl_3x3(struct matrix_3x3 *m)
{
    double d0 = m->a00;
    m->a00 = 1. / d0;
    m->a10 *= m->a00;
    m->a20 *= m->a00;

    double d1 = m->a11 - d0 * m->a10 * m->a10;
    m->a11 = 1. / d1;
    m->a21 -= m->a20 * m->a10 * d0;
    m->a21 *= m->a11;

    double d2 = m->a22 - d0 * m->a20 * m->a20 - d1 * m->a21 * m->a21;
    m->a22 = 1. / d2;
}

static void
compute_rhs_3(struct stepcompress *sc, uint16_t count, struct rhs_3 *f
              , struct rhs_3 *prev_f)
{
    uint32_t lsc = sc->last_step_clock;
    if (unlikely(count <= 1)) {
        memset(f, 0, sizeof(*f));
        f->b0 += FIRST_STEP_BIAS * sc->next_expected_interval;
    } else {
        *f = *prev_f;
    }
    double d = sc->queue_pos[count-1] - lsc;
    int32_t c = count;
    f->b0 += d * c;
    c = c * (count-1) / 2;
    f->b1 += d * c;
    c = c * (count-2) / 3;
    f->b2 += d * c;
}

static void
solve_3x3(struct matrix_3x3 *m, struct rhs_3 *f)
{
    f->b1 -= f->b0 * m->a10;
    f->b2 -= f->b0 * m->a20 + f->b1 * m->a21;

    f->b0 *= m->a00;
    f->b1 *= m->a11;
    f->b2 *= m->a22;

    f->b1 -= f->b2 * m->a21;
    f->b0 -= f->b1 * m->a10 + f->b2 * m->a20;
}

static struct step_move_hp
step_move_encode(uint16_t count, struct rhs_3* f)
{
    struct step_move_hp res;
    memset(&res, 0, sizeof(res));
    double interval = f->b0, add = f->b1, add2 = f->b2;
    if (interval < 0. || count > 0x7FFF)
        return res;
    if (count <= 1) {
        res.count = count;
        res.interval = round(interval);
        return res;
    }
    double end_add = add + add2 * count;
    double max_int_inc = count * (fabs(add) > fabs(end_add) ?
                                  fabs(add) : fabs(end_add));
    double max_end_int = interval + max_int_inc;
    if (fabs(add) > MAX_ADD || fabs(end_add) > MAX_ADD ||
            fabs(add2) > MAX_ADD2 || max_end_int > MAX_INTRVL) {
        while (res.shift >= MIN_SHIFT && (
                    fabs(add) > MAX_ADD || fabs(end_add) > MAX_ADD ||
                    fabs(add2) > MAX_ADD2 || max_end_int > MAX_INTRVL)) {
            interval *= 0.5;
            add *= 0.5;
            add2 *= 0.5;
            end_add *= 0.5;
            max_end_int *= 0.5;
            --res.shift;
        }
        if (res.shift < MIN_SHIFT)
            // Cannot encode the current rhs_3, the values are too large
            return res;
    } else if (max_int_inc >= 0.5 ||
               count * fabs(interval-round(interval)) >= 0.5) {
        while (res.shift < MAX_SHIFT) {
            double next_interval = interval * 2.;
            double next_add = add * 2.;
            double next_add2 = add2 * 2.;
            double next_end_add = end_add * 2.;
            double next_max_end_int = max_end_int * 2.;
            int8_t next_shift = res.shift + 1;
            uint_fast8_t extra_shift =
                next_shift > 8 ? 16-next_shift : 8-next_shift;
            if (fabs(next_add) > MAX_ADD || fabs(next_end_add) > MAX_ADD ||
                fabs(next_add2) > MAX_ADD2 || next_max_end_int > MAX_INTRVL ||
                next_interval * (1 << extra_shift) > MAX_INT32 ||
                fabs(next_add * (1 << extra_shift)) > MAX_INT32 ||
                fabs(next_add2 * (1 << extra_shift)) > MAX_INT32)
                break;
            interval = next_interval;
            add = next_add;
            add2 = next_add2;
            end_add = next_end_add;
            max_end_int = next_max_end_int;
            ++res.shift;
        }
    }
    res.count = count;
    res.interval = round(interval);
    res.add = round(add);
    res.add2 = round(add2);
    return res;
}

static struct step_move_hp
single_step_move_encode(uint64_t interval)
{
    struct step_move_hp res;
    memset(&res, 0, sizeof(res));
    res.interval = interval;
    res.count = 1;
    res.first_step = res.last_step = interval;
    return res;
}

struct points {
    int32_t minp, maxp;
};

// Given a requested step time, return the minimum and maximum
// acceptable times
static inline struct points
minmax_point(struct stepcompress *sc, uint32_t *pos)
{
    uint32_t lsc = sc->last_step_clock, point = *pos - lsc;

    uint32_t max_bck_error = pos > sc->queue ? *pos - *(pos-1) : point;
    max_bck_error = (max_bck_error + (1 << (MAX_ERR_2P - 1))) >> MAX_ERR_2P;
    if (max_bck_error < MIN_STEP_ERR) max_bck_error = MIN_STEP_ERR;
    if (max_bck_error > sc->max_error) max_bck_error = sc->max_error;

    uint32_t max_frw_error = pos + 1 < sc->queue_next ? *(pos+1) - *pos : (
            sc->next_step_clock ? (uint32_t)sc->next_step_clock - *pos : 0);
    max_frw_error = (max_frw_error + (1 << (MAX_ERR_2P - 1))) >> MAX_ERR_2P;
    if (max_frw_error) {
        if (max_frw_error < MIN_STEP_ERR) max_frw_error = MIN_STEP_ERR;
        if (max_bck_error > max_frw_error) max_bck_error = max_frw_error;
        if (max_frw_error > max_bck_error) max_frw_error = max_bck_error;
    } else max_frw_error = MIN_STEP_ERR;

    return (struct points){ point - max_bck_error, point + max_frw_error };
}

static inline struct points
get_cached_minmax_point(struct stepcompress *sc, uint16_t ind)
{
    if (ind < MAX_COUNT_LSM && ind < sc->cached_count)
        return sc->errb_cache[ind];
    return minmax_point(sc, sc->queue_pos + ind);
}

struct stepper_moves {
    uint32_t interval;
    int32_t add;
    int32_t add2;
    uint_fast8_t shift;
    uint16_t int_low_acc;
    uint16_t count;
};

static inline void
add_interval(uint32_t* time, struct stepper_moves *s)
{
    if (s->shift == 16) {
        uint32_t interval = s->interval + s->int_low_acc;
        *time += interval >> 16;
        s->int_low_acc = interval & 0xFFFF;
    } else if (s->shift == 8) {
        uint32_t interval = s->interval + s->int_low_acc;
        *time += interval >> 8;
        s->int_low_acc = interval & 0xFF;
    } else {
        *time += s->interval;
    }
}

static inline void
inc_interval(struct stepper_moves *s)
{
    s->interval += s->add;
    s->add += s->add2;
}

static void
fill_stepper_moves(struct step_move_hp *m, struct stepper_moves *s)
{
    s->count = m->count;
    uint32_t interval = m->interval;
    int32_t add = m->add;
    int32_t add2 = m->add2;
    int8_t shift = m->shift;

    if (shift <= 0) {
        interval <<= -shift;
        // Left shift of a negative int is an undefined behavior in C
        add = add >= 0 ? add << -shift : -(-add << -shift);
        add2 = add2 >= 0 ? add2 << -shift : -(-add2 << -shift);
        s->interval = interval;
        s->add = add;
        s->add2 = add2;
        s->int_low_acc = 0;
        s->shift = 0;
    } else {
        uint_fast8_t extra_shift = shift > 8 ? 16-shift : 8-shift;
        s->shift = shift > 8 ? 16 : 8;
        interval <<= extra_shift;
        add = add >= 0 ? add << extra_shift : -(-add << extra_shift);
        add2 = add2 >= 0 ? add2 << extra_shift : -(-add2 << extra_shift);
        s->interval = interval;
        s->add = add;
        s->add2 = add2;
        if (s->shift == 16) {
            s->int_low_acc = 1 << 15;
        } else {  // s->shift == 8
            s->int_low_acc = 1 << 7;
        }
    }
}

static int
test_step_move(struct stepcompress *sc, struct step_move_hp *m
               , int report_errors, int trunc_move)
{
    if (!m->count || (!m->interval && !m->add && !m->add2 && m->count > 1)
        || m->interval >= 0x80000000
        || m->count > 0x7FFF
        || m->add < -MAX_ADD || m->add > MAX_ADD
        || m->add2 < -MAX_ADD2 || m->add2 > MAX_ADD2
        || m->shift > MAX_SHIFT || m->shift < MIN_SHIFT) {
        if (report_errors)
            errorf("stepcompress o=%d i=%d c=%d a=%d, a2=%d, s=%d:"
                   " Invalid sequence"
                   , sc->oid, m->interval, m->count, m->add, m->add2, m->shift);
        m->count = 0;
        return ERROR_RET;
    }
    struct stepper_moves s;
    fill_stepper_moves(m, &s);
    m->next_step_interval = 0;
    uint32_t lsc = sc->last_step_clock;
    uint16_t i, trunc_pos = 0;
    uint32_t cur_step = 0, prev_step = 0, trunc_last_step = 0;
    uint32_t trunc_err = MAX_INT32;
    for (i = 0; i < m->count; ++i) {
        add_interval(&cur_step, &s);
        struct points point = get_cached_minmax_point(sc, i);
        if ((int32_t)cur_step < point.minp || cur_step > point.maxp) {
            if (report_errors)
                errorf("stepcompress o=%d i=%d c=%d a=%d, a2=%d, s=%d:"
                       " Point %u: %d not in %d:%d"
                       , sc->oid, m->interval, m->count, m->add, m->add2
                       , m->shift, i+1, cur_step, point.minp, point.maxp);
            // The least squares method does not minimize the maximum error
            // in the generated step sequence, but rather the total error.
            // However, we can still use the longest good generated prefix.
            m->count = i;
            return ERROR_RET;
        }
        if (trunc_move && m->count > 3 && m->count - i <= (m->count + 9) / 10) {
            int32_t err = cur_step - (sc->queue_pos[i] - lsc);
            if (err < 0) err = -err;
            if (err <= trunc_err || err <= 1) {
                trunc_pos = i;
                trunc_err = err;
                m->next_step_interval = cur_step - prev_step;
                trunc_last_step = prev_step;
            }
        }
        inc_interval(&s);
        if (s.interval >= 0x80000000) {
            if (report_errors)
                errorf("stepcompress o=%d i=%d c=%d a=%d, a2=%d, s=%d:"
                       " Point %d: interval overflow %d"
                       , sc->oid, m->interval, m->count, m->add, m->add2
                       , m->shift, i+1, s.interval);
            m->count = i;
            return ERROR_RET;
        }
        m->last_step = cur_step;
        if (!m->first_step)
            m->first_step = cur_step;
        prev_step = cur_step;
    }
    if (trunc_move && trunc_pos) {
        m->count = trunc_pos;
        m->last_step = trunc_last_step;
    }
    return 0;
}

static struct step_move_hp
test_step_count(struct stepcompress *sc, uint16_t count)
{
    if (count > MAX_COUNT_LSM || sc->cached_count < count) {
        struct step_move_hp res;
        memset(&res, 0, sizeof(res));
        return res;
    }
    struct matrix_3x3 *m = &least_squares_ldl[count-1];
    struct rhs_3 f = sc->rhs_cache[count-1];
    solve_3x3(m, &f);
    struct step_move_hp res = step_move_encode(count, &f);
    test_step_move(sc, &res, /*report_errors=*/0, /*trunc_move=*/0);
    if (count > 20 && res.count < count / 4) {
        m = &least_squares_efsb_ldl[count-1];
        f = sc->rhs_cache[count-1];
        f.b0 += EXTRA_FIRST_STEP_BIAS * sc->next_expected_interval;
        solve_3x3(m, &f);
        struct step_move_hp efsb_res = step_move_encode(count, &f);
        test_step_move(sc, &efsb_res, /*report_errors=*/0, /*trunc_move=*/0);
        if (efsb_res.count > res.count) return efsb_res;
    }
    return res;
}

static struct step_move_hp
gen_avg_interval(struct stepcompress *sc, uint16_t count)
{
    uint32_t lsc = sc->last_step_clock;
    double d = sc->queue_pos[count-1] - lsc;
    d += FIRST_STEP_BIAS * sc->next_expected_interval;
    struct rhs_3 f;
    memset(&f, 0, sizeof(f));
    f.b0 = d / (count + FIRST_STEP_BIAS);
    return step_move_encode(count, &f);
}

inline static void
update_caches_to_count(struct stepcompress *sc, uint16_t count)
{
    if (!sc->cached_count) {
        compute_rhs_3(sc, /*count=*/1, &sc->rhs_cache[0], NULL);
        sc->errb_cache[0] = minmax_point(sc, sc->queue_pos);
        sc->cached_count = 1;
    }
    uint16_t i;
    for (i = sc->cached_count + 1; i <= count && i <= MAX_COUNT_LSM; ++i) {
        compute_rhs_3(sc, i, &sc->rhs_cache[i-1], &sc->rhs_cache[i-2]);
        sc->errb_cache[i-1] = minmax_point(sc, sc->queue_pos+i-1);
    }
    sc->cached_count = i - 1;
}

// Find a 'step_move_hp' that covers a series of step times
static struct step_move_hp
compress_bisect_count(struct stepcompress *sc)
{
    sc->cached_count = 0;
    struct step_move_hp cur, best;
    memset(&best, 0, sizeof(best));
    uint16_t queue_size = sc->queue_next < sc->queue_pos + 0x7FFF
                        ? sc->queue_next - sc->queue_pos : 0x7FFF;
    uint16_t left = 0, right = 8;
    for (; right <= MAX_COUNT_LSM && right <= queue_size; right <<= 1) {
        update_caches_to_count(sc, right);
        cur = test_step_count(sc, right);
        if (cur.count > best.count) {
            best = cur;
            left = cur.count;
        }
        else break;
    }
    if (right >= MAX_COUNT_BISECT) {
        for (; right <= queue_size; right <<= 1) {
            cur = gen_avg_interval(sc, right);
            test_step_move(sc, &cur, /*report_errors=*/0, /*trunc_move=*/0);
            if (cur.count > best.count) best = cur;
            else break;
        }
        return best;
    }
    if (right > queue_size) right = queue_size + 1;
    update_caches_to_count(sc, right - 1);
    while (right - left > 1) {
        uint16_t count = (left + right) / 2;
        cur = test_step_count(sc, count);
        if (cur.count > best.count) {
            best = cur;
            left = count;
        } else right = count;
    }
    if (best.count <= 1) {
        uint32_t interval = *sc->queue_pos - (uint32_t)sc->last_step_clock;
        return single_step_move_encode(interval);
    }
    return best;
}


/****************************************************************
 * Step compress interface
 ****************************************************************/

// Maximium clock delta between messages in the queue
#define CLOCK_DIFF_MAX (3<<28)

// Helper to create a queue_step command from a 'struct step_move_hp'
static void
add_move(struct stepcompress *sc, uint64_t first_clock
         , struct step_move_hp *move)
{
    uint64_t last_clock = sc->last_step_clock + move->last_step;

    // Create and queue a queue_step command
    uint32_t msg[7] = {
        sc->queue_step_msgtag, sc->oid, move->interval, move->count,
        move->add, move->add2, move->shift
    };
    struct queue_message *qm = message_alloc_and_encode(msg, 7);
    qm->min_clock = qm->req_clock = sc->last_step_clock;
    if (move->count == 1 && first_clock >= sc->last_step_clock + CLOCK_DIFF_MAX)
        qm->req_clock = first_clock;
    list_add_tail(&qm->node, &sc->msg_queue);
    sc->last_step_clock = last_clock;
    sc->next_expected_interval = move->next_step_interval;

    // Create and store move in history tracking
    struct history_steps *hs = malloc(sizeof(*hs));
    hs->first_clock = first_clock;
    hs->last_clock = last_clock;
    hs->start_position = sc->last_position;
    hs->interval = move->interval;
    hs->add = move->add;
    hs->add2 = move->add2;
    hs->shift = move->shift;
    hs->step_count = sc->sdir ? move->count : -move->count;
    sc->last_position += hs->step_count;
    list_add_head(&hs->node, &sc->history_list);
}

// Convert previously scheduled steps into commands for the mcu
int
queue_flush_hp(struct stepcompress *sc, uint64_t move_clock)
{
    if (sc->queue_pos >= sc->queue_next)
        return 0;
    while (sc->last_step_clock < move_clock) {
        if (!sc->next_expected_interval) {
            sc->next_expected_interval =
                *sc->queue_pos - (uint32_t)sc->last_step_clock;
        }
        struct step_move_hp move = compress_bisect_count(sc);
        // Verify that a given 'step_move_hp' matches the actual step times
        // and truncate it to improve the junction with the next step_move_hp
        // and reduce velocity jitter if appropriate
        int ret = test_step_move(sc, &move,
                                 /*report_errors=*/1, /*trunc_move=*/1);
        if (ret)
            return ret;

        add_move(sc, sc->last_step_clock + move.first_step, &move);

        if (sc->queue_pos + move.count >= sc->queue_next) {
            sc->queue_pos = sc->queue_next = sc->queue;
            break;
        }
        sc->queue_pos += move.count;
    }
    stepcompress_calc_last_step_print_time(sc);
    return 0;
}

// Generate a queue_step for a step far in the future from the last step
int
queue_flush_far_hp(struct stepcompress *sc, uint64_t abs_step_clock)
{
    uint64_t interval = abs_step_clock - sc->last_step_clock;
    struct step_move_hp move = single_step_move_encode(interval);
    add_move(sc, abs_step_clock, &move);
    stepcompress_calc_last_step_print_time(sc);
    return 0;
}

// Allocate a new 'stepcompress' object
struct stepcompress * __visible
stepcompress_hp_alloc(uint32_t oid)
{
    struct stepcompress *sc = malloc(sizeof(*sc));
    memset(sc, 0, sizeof(*sc));
    list_init(&sc->msg_queue);
    list_init(&sc->history_list);
    sc->oid = oid;
    sc->sdir = -1;
    sc->queue_flush = queue_flush_hp;
    sc->queue_flush_far = queue_flush_far_hp;
    sc->rhs_cache = malloc(sizeof(sc->rhs_cache[0]) * MAX_COUNT_LSM);
    sc->errb_cache = malloc(sizeof(sc->errb_cache[0]) * MAX_COUNT_LSM);
    if (!least_squares_ldl[0].a00)
        for (int i = 0; i < MAX_COUNT_LSM; ++i) {
            struct matrix_3x3 *m = &least_squares_ldl[i];
            fill_least_squares_matrix_3x3(i+1, m);
            compute_ldl_3x3(m);
            m = &least_squares_efsb_ldl[i];
            fill_least_squares_matrix_3x3(i+1, m);
            m->a00 += EXTRA_FIRST_STEP_BIAS;
            compute_ldl_3x3(m);
        }
    return sc;
}
