#ifndef ITERSOLVE_H
#define ITERSOLVE_H

#include <stdint.h> // uint32_t

struct coord {
    double x, y, z;
};

struct move *move_alloc(void);
void move_fill(struct move *m, double print_time
               , double accel_t, double cruise_t, double decel_t
               , double start_pos_x, double start_pos_y, double start_pos_z
               , double axes_d_x, double axes_d_y, double axes_d_z
               , double start_v, double cruise_v, double accel);
struct coord move_get_coord(struct move *m, double move_time);

struct stepper_kinematics;
typedef double (*sk_callback)(struct stepper_kinematics *sk, struct move *m
                              , double move_time);
struct stepper_kinematics {
    double step_dist, commanded_pos;
    struct stepcompress *sc;
    sk_callback calc_position;
};

int32_t itersolve_gen_steps(struct stepper_kinematics *sk, struct move *m);
void itersolve_set_stepcompress(struct stepper_kinematics *sk
                                , struct stepcompress *sc, double step_dist);
void itersolve_set_commanded_pos(struct stepper_kinematics *sk, double pos);

#endif // itersolve.h
