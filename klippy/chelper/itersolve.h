#ifndef ITERSOLVE_H
#define ITERSOLVE_H

#include <stdint.h> // int32_t

struct stepper_kinematics;
struct move;
typedef double (*sk_calc_callback)(struct stepper_kinematics *sk, struct move *m
                                   , double move_time);
typedef void (*sk_post_callback)(struct stepper_kinematics *sk);
struct stepper_kinematics {
    double step_dist, commanded_pos;
    struct stepcompress *sc;
    sk_calc_callback calc_position_cb;
    sk_post_callback post_cb;
};

int32_t itersolve_gen_steps(struct stepper_kinematics *sk, struct move *m);
void itersolve_set_stepcompress(struct stepper_kinematics *sk
                                , struct stepcompress *sc, double step_dist);
double itersolve_calc_position_from_coord(struct stepper_kinematics *sk
                                          , double x, double y, double z);
void itersolve_set_commanded_pos(struct stepper_kinematics *sk, double pos);
double itersolve_get_commanded_pos(struct stepper_kinematics *sk);

#endif // itersolve.h
