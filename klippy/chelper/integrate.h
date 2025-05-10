#ifndef INTEGRATE_H
#define INTEGRATE_H

struct move;

double calc_smoothed_velocity(struct move* m, int axis, double move_time
                              , double hst);

double move_integrate_weighted(double base, double start_v, double half_accel
                               , double start, double end, double time_offset);

#endif // integrate.h
