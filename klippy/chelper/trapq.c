// Trapezoidal velocity movement queue
//
// Copyright (C) 2018-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // sqrt
#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // unlikely
#include "trapq.h" // move_get_coord

// Allocate coordinate with specified axis count
struct coord
coord_alloc(int axis_count)
{
    struct coord c;
    c.axis_count = axis_count;
    c.axis = malloc(axis_count * sizeof(double));
    memset(c.axis, 0, axis_count * sizeof(double));
    return c;
}

// Free coordinate memory
void
coord_free(struct coord *c)
{
    if (c->axis) {
        free(c->axis);
        c->axis = NULL;
    }
    c->axis_count = 0;
}

// Allocate a new 'move' object
struct move *
move_alloc(int axis_count)
{
    struct move *m = malloc(sizeof(*m));
    memset(m, 0, sizeof(*m));
    m->start_pos = coord_alloc(axis_count);
    m->axes_r = coord_alloc(axis_count);
    return m;
}

// Return the distance moved given a time in a move
inline double
move_get_distance(struct move *m, double move_time)
{
    return (m->start_v + m->half_accel * move_time) * move_time;
}

// Return the coordinates given a time in a move
inline struct coord
move_get_coord(struct move *m, double move_time)
{
    double move_dist = move_get_distance(m, move_time);
    struct coord result = coord_alloc(m->start_pos.axis_count);
    for (int i = 0; i < m->start_pos.axis_count; i++) {
        result.axis[i] = m->start_pos.axis[i] + m->axes_r.axis[i] * move_dist;
    }
    return result;
}

#define NEVER_TIME 9999999999999999.9

// Allocate a new 'trapq' object
struct trapq * __visible
trapq_alloc(int axis_count)
{
    struct trapq *tq = malloc(sizeof(*tq));
    memset(tq, 0, sizeof(*tq));
    list_init(&tq->moves);
    list_init(&tq->history);
    struct move *head_sentinel = move_alloc(axis_count), *tail_sentinel = move_alloc(axis_count);
    head_sentinel->print_time = -1.0;
    tail_sentinel->print_time = tail_sentinel->move_t = NEVER_TIME;
    list_add_head(&head_sentinel->node, &tq->moves);
    list_add_tail(&tail_sentinel->node, &tq->moves);
    return tq;
}

// Free memory associated with a 'trapq' object
void __visible
trapq_free(struct trapq *tq)
{
    while (!list_empty(&tq->moves)) {
        struct move *m = list_first_entry(&tq->moves, struct move, node);
        list_del(&m->node);
        coord_free(&m->start_pos);
        coord_free(&m->axes_r);
        free(m);
    }
    while (!list_empty(&tq->history)) {
        struct move *m = list_first_entry(&tq->history, struct move, node);
        list_del(&m->node);
        coord_free(&m->start_pos);
        coord_free(&m->axes_r);
        free(m);
    }
    free(tq);
}

// Update the list sentinels
void
trapq_check_sentinels(struct trapq *tq)
{
    struct move *tail_sentinel = list_last_entry(&tq->moves, struct move, node);
    if (tail_sentinel->print_time)
        // Already up to date
        return;
    struct move *m = list_prev_entry(tail_sentinel, node);
    struct move *head_sentinel = list_first_entry(&tq->moves, struct move,node);
    if (m == head_sentinel) {
        // No moves at all on this list
        tail_sentinel->print_time = NEVER_TIME;
        return;
    }
    tail_sentinel->print_time = m->print_time + m->move_t;
    tail_sentinel->start_pos = move_get_coord(m, m->move_t);
}

#define MAX_NULL_MOVE 1.0

// Add a move to the trapezoid velocity queue
void
trapq_add_move(struct trapq *tq, struct move *m)
{
    struct move *tail_sentinel = list_last_entry(&tq->moves, struct move, node);
    struct move *prev = list_prev_entry(tail_sentinel, node);
    if (prev->print_time + prev->move_t < m->print_time) {
        // Add a null move to fill time gap
        struct move *null_move = move_alloc(m->start_pos.axis_count);
        // Copy position data
        for (int i = 0; i < m->start_pos.axis_count; i++) {
            null_move->start_pos.axis[i] = m->start_pos.axis[i];
        }
        if (!prev->print_time && m->print_time > MAX_NULL_MOVE)
            // Limit the first null move to improve numerical stability
            null_move->print_time = m->print_time - MAX_NULL_MOVE;
        else
            null_move->print_time = prev->print_time + prev->move_t;
        null_move->move_t = m->print_time - null_move->print_time;
        list_add_before(&null_move->node, &tail_sentinel->node);
    }
    list_add_before(&m->node, &tail_sentinel->node);
    tail_sentinel->print_time = 0.;
}

// Fill and add a move to the trapezoid velocity queue
void __visible
trapq_append(struct trapq *tq, double print_time
             , double accel_t, double cruise_t, double decel_t
             , double *start_pos, double *axes_r
             , double start_v, double cruise_v, double accel)
{
    // Get axis count from the trapq's sentinel moves
    struct move *tail_sentinel = list_last_entry(&tq->moves, struct move, node);
    int axis_count = tail_sentinel->start_pos.axis_count;

    struct coord start_pos_coord = coord_alloc(axis_count);
    struct coord axes_r_coord = coord_alloc(axis_count);
    for (int i = 0; i < axis_count; i++) {
        start_pos_coord.axis[i] = start_pos[i];
        axes_r_coord.axis[i] = axes_r[i];
    }

    if (accel_t) {
        struct move *m = move_alloc(axis_count);
        m->print_time = print_time;
        m->move_t = accel_t;
        m->start_v = start_v;
        m->half_accel = .5 * accel;
        for (int i = 0; i < axis_count; i++) {
            m->start_pos.axis[i] = start_pos_coord.axis[i];
            m->axes_r.axis[i] = axes_r_coord.axis[i];
        }
        trapq_add_move(tq, m);

        print_time += accel_t;
        struct coord new_pos = move_get_coord(m, accel_t);
        for (int i = 0; i < axis_count; i++) {
            start_pos_coord.axis[i] = new_pos.axis[i];
        }
        coord_free(&new_pos);
    }
    if (cruise_t) {
        struct move *m = move_alloc(axis_count);
        m->print_time = print_time;
        m->move_t = cruise_t;
        m->start_v = cruise_v;
        m->half_accel = 0.;
        for (int i = 0; i < axis_count; i++) {
            m->start_pos.axis[i] = start_pos_coord.axis[i];
            m->axes_r.axis[i] = axes_r_coord.axis[i];
        }
        trapq_add_move(tq, m);

        print_time += cruise_t;
        struct coord new_pos = move_get_coord(m, cruise_t);
        for (int i = 0; i < axis_count; i++) {
            start_pos_coord.axis[i] = new_pos.axis[i];
        }
        coord_free(&new_pos);
    }
    if (decel_t) {
        struct move *m = move_alloc(axis_count);
        m->print_time = print_time;
        m->move_t = decel_t;
        m->start_v = cruise_v;
        m->half_accel = -.5 * accel;
        for (int i = 0; i < axis_count; i++) {
            m->start_pos.axis[i] = start_pos_coord.axis[i];
            m->axes_r.axis[i] = axes_r_coord.axis[i];
        }
        trapq_add_move(tq, m);
    }

    coord_free(&start_pos_coord);
    coord_free(&axes_r_coord);
}

// Expire any moves older than `print_time` from the trapezoid velocity queue
void __visible
trapq_finalize_moves(struct trapq *tq, double print_time
                     , double clear_history_time)
{
    struct move *head_sentinel = list_first_entry(&tq->moves, struct move,node);
    struct move *tail_sentinel = list_last_entry(&tq->moves, struct move, node);
    // Move expired moves from main "moves" list to "history" list
    for (;;) {
        struct move *m = list_next_entry(head_sentinel, node);
        if (m == tail_sentinel) {
            tail_sentinel->print_time = NEVER_TIME;
            break;
        }
        if (m->print_time + m->move_t > print_time)
            break;
        list_del(&m->node);
        if (m->start_v || m->half_accel)
            list_add_head(&m->node, &tq->history);
        else {
            coord_free(&m->start_pos);
            coord_free(&m->axes_r);
            free(m);
        }
    }
    // Free old moves from history list
    if (list_empty(&tq->history))
        return;
    struct move *latest = list_first_entry(&tq->history, struct move, node);
    for (;;) {
        struct move *m = list_last_entry(&tq->history, struct move, node);
        if (m == latest || m->print_time + m->move_t > clear_history_time)
            break;
        list_del(&m->node);
        coord_free(&m->start_pos);
        coord_free(&m->axes_r);
        free(m);
    }
}

// Note a position change in the trapq history
void __visible
trapq_set_position(struct trapq *tq, double print_time, double *pos)
{
    // Flush all moves from trapq
    trapq_finalize_moves(tq, NEVER_TIME, 0);

    // Get axis count from the trapq's sentinel moves
    struct move *tail_sentinel = list_last_entry(&tq->moves, struct move, node);
    int axis_count = tail_sentinel->start_pos.axis_count;

    // Prune any moves in the trapq history that were interrupted
    while (!list_empty(&tq->history)) {
        struct move *m = list_first_entry(&tq->history, struct move, node);
        if (m->print_time < print_time) {
            if (m->print_time + m->move_t > print_time)
                m->move_t = print_time - m->print_time;
            break;
        }
        list_del(&m->node);
        coord_free(&m->start_pos);
        coord_free(&m->axes_r);
        free(m);
    }

    // Add a marker to the trapq history
    struct move *m = move_alloc(axis_count);
    m->print_time = print_time;
    for (int i = 0; i < axis_count; i++) {
        m->start_pos.axis[i] = pos[i];
    }
    list_add_head(&m->node, &tq->history);
}

// Return history of movement queue
int __visible
trapq_extract_old(struct trapq *tq, struct pull_move *p, int max
                  , double start_time, double end_time)
{
    int res = 0;
    struct move *m;
    list_for_each_entry(m, &tq->history, node) {
        if (start_time >= m->print_time + m->move_t || res >= max)
            break;
        if (end_time <= m->print_time)
            continue;
        p->print_time = m->print_time;
        p->move_t = m->move_t;
        p->start_v = m->start_v;
        p->accel = 2. * m->half_accel;
        p->axis_count = m->start_pos.axis_count;

        // Allocate memory for axis data if needed
        if (!p->start_pos) {
            p->start_pos = malloc(p->axis_count * sizeof(double));
            p->axis_r = malloc(p->axis_count * sizeof(double));
        }

        // Copy axis data
        for (int i = 0; i < p->axis_count; i++) {
            p->start_pos[i] = m->start_pos.axis[i];
            p->axis_r[i] = m->axes_r.axis[i];
        }

        p++;
        res++;
    }
    return res;
}
