#ifndef STEPCORR_H
#define STEPCORR_H

struct stepper_corrections {
    double rad_per_mm;
    double stealthchop_lag_const;
    double spreadcycle_lag_const;
    double hst;
};

struct move;
struct stepper_kinematics;

double stepcorr_calc_position(struct stepper_kinematics *sk, struct move *m
                              , double move_time);
int stepcorr_set_lag_correction(struct stepper_kinematics *sk, double rad_per_mm
                                , double stealthchop_lag_const
                                , double spreadcycle_lag_const
                                , double velocity_smooth_time);
void stepcorr_update_gen_steps_window(struct stepper_kinematics *sk);

#endif // stepcorr.h
