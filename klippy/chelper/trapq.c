// Trapezoidal velocity movement queue
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h> // sqrt
#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // unlikely
#include "trapq.h" // move_get_coord

// Allocate a new 'move' object
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

// Allocate a new 'trapq' object
struct trapq * __visible
trapq_alloc(void)
{
    struct trapq *tq = malloc(sizeof(*tq));
    memset(tq, 0, sizeof(*tq));
    list_init(&tq->moves);
    return tq;
}

// Free memory associated with a 'trapq' object
void __visible
trapq_free(struct trapq *tq)
{
    while (!list_empty(&tq->moves)) {
        struct move *m = list_first_entry(&tq->moves, struct move, node);
        list_del(&m->node);
        free(m);
    }
    free(tq);
}

// Add a move to the trapezoid velocity queue
void __visible
trapq_add_move(struct trapq *tq, struct move *m)
{
    struct move *nm = move_alloc();
    memcpy(nm, m, sizeof(*nm));
    list_add_tail(&nm->node, &tq->moves);
}

// Free any moves older than `print_time` from the trapezoid velocity queue
void __visible
trapq_free_moves(struct trapq *tq, double print_time)
{
    while (!list_empty(&tq->moves)) {
        struct move *m = list_first_entry(&tq->moves, struct move, node);
        if (m->print_time + m->move_t > print_time)
            return;
        list_del(&m->node);
        free(m);
    }
}
