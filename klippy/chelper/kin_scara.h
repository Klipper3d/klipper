#ifndef KIN_SCARA_H
#define KIN_SCARA_H

#include <stdlib.h> // malloc
#include <string.h> // memset
#include <math.h>   // M_PI, cos, sin, atan2, sqrt
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "pyhelper.h" // errorf
#include "trapq.h" // move_get_coord
#include "kin_scara.h"

struct scara_params {
    double arm1;
    double arm2;
    double j1;
    double j2;
    double g1;
    double g2;
};


static void scara_forward_kinematics(double theta, double phi, double *x, double *y,
                              const struct scara_params *params);


static void scara_inverse_kinematics(double x, double y, double *theta, double *phi,
                              const struct scara_params *params);


static double scara_stepper_theta_calc_position(struct stepper_kinematics *sk, struct move *m, double move_time);


static double scara_stepper_phi_calc_position(struct stepper_kinematics *sk, struct move *m, double move_time);


static double scara_stepper_z_calc_position(struct stepper_kinematics *sk, struct move *m, double move_time);


struct stepper_kinematics *scara_stepper_alloc(char axis, double arm1, double arm2, double j1, double j2, double g1, double g2);

#endif // KIN_SCARA_H