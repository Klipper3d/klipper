#ifndef TRAPQ_H
#define TRAPQ_H

#include "list.h" // list_node

struct coord {
    int axis_count;
    double *axis;
};

struct move {
    double print_time, move_t;
    double start_v, half_accel;
    struct coord start_pos, axes_r;

    struct list_node node;
};

struct trapq {
    struct list_head moves, history;
};

struct pull_move {
    double print_time, move_t;
    double start_v, accel;
    int axis_count;
    double *start_pos;
    double *axis_r;
};

struct move *move_alloc(int axis_count);
double move_get_distance(struct move *m, double move_time);
struct coord move_get_coord(struct move *m, double move_time);
struct coord coord_alloc(int axis_count);
void coord_free(struct coord *c);
struct trapq *trapq_alloc(int axis_count);
void trapq_free(struct trapq *tq);
void trapq_check_sentinels(struct trapq *tq);
void trapq_add_move(struct trapq *tq, struct move *m);
void trapq_append(struct trapq *tq, double print_time
                  , double accel_t, double cruise_t, double decel_t
                  , double *start_pos, double *axes_r
                  , double start_v, double cruise_v, double accel);
void trapq_finalize_moves(struct trapq *tq, double print_time
                          , double clear_history_time);
void trapq_set_position(struct trapq *tq, double print_time, double *pos);
int trapq_extract_old(struct trapq *tq, struct pull_move *p, int max
                      , double start_time, double end_time);

#endif // trapq.h
