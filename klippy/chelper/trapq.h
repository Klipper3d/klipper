#ifndef TRAPQ_H
#define TRAPQ_H

struct coord {
    double x, y, z;
};

struct move_accel {
    double c1, c2;
};

struct move {
    double print_time, move_t;
    double accel_t, cruise_t;
    double cruise_start_d, decel_start_d;
    double cruise_v;
    struct move_accel accel, decel;
    struct coord start_pos, axes_r;
};

struct move *move_alloc(void);
void move_fill(struct move *m, double print_time
               , double accel_t, double cruise_t, double decel_t
               , double start_pos_x, double start_pos_y, double start_pos_z
               , double axes_d_x, double axes_d_y, double axes_d_z
               , double start_v, double cruise_v, double accel);
double move_get_distance(struct move *m, double move_time);
struct coord move_get_coord(struct move *m, double move_time);

#endif // trapq.h
