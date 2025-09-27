#ifndef ITERSOLVE_H
#define ITERSOLVE_H

#include <stdint.h> // int32_t

#define AF_AXIS(n) (1 << (n))
#define AF_X AF_AXIS(0)
#define AF_Y AF_AXIS(1)
#define AF_Z AF_AXIS(2)

struct stepper_kinematics;
struct move;
typedef double (*sk_calc_callback)(struct stepper_kinematics *sk, struct move *m
                                   , double move_time);
typedef void (*sk_post_callback)(struct stepper_kinematics *sk);
struct stepper_kinematics {
    double step_dist, commanded_pos;
    struct stepcompress *sc;

    double last_flush_time, last_move_time;
    struct trapq *tq;
    int active_flags;
    double gen_steps_pre_active, gen_steps_post_active;

    sk_calc_callback calc_position_cb;
    sk_post_callback post_cb;
};

int32_t itersolve_generate_steps(struct stepper_kinematics *sk
                                 , struct stepcompress *sc, double flush_time);
double itersolve_check_active(struct stepper_kinematics *sk, double flush_time);
int32_t itersolve_is_active_axis(struct stepper_kinematics *sk, char axis);
void itersolve_set_trapq(struct stepper_kinematics *sk, struct trapq *tq
                         , double step_dist);
struct trapq *itersolve_get_trapq(struct stepper_kinematics *sk);
double itersolve_calc_position_from_coord(struct stepper_kinematics *sk
                                          , double *coord);
void itersolve_set_position(struct stepper_kinematics *sk, double *coord);
double itersolve_get_commanded_pos(struct stepper_kinematics *sk);
double itersolve_get_gen_steps_pre_active(struct stepper_kinematics *sk);
double itersolve_get_gen_steps_post_active(struct stepper_kinematics *sk);

#endif // itersolve.h
