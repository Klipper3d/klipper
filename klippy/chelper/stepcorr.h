#ifndef STEPCORR_H
#define STEPCORR_H

struct stepper_corrections {
    double rad_per_mm, motor_lag_const;
};

struct move;
struct stepper_kinematics;

double stepcorr_calc_position(struct stepper_kinematics *sk, struct move *m
                              , double move_time);
int stepcorr_set_lag_correction(struct stepper_kinematics *sk, double rad_per_mm
                                , double motor_lag_const);
void stepcorr_update_gen_steps_window(struct stepper_kinematics *sk);

#endif // stepcorr.h
