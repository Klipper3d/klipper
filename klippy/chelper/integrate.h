#ifndef INTEGRATE_H
#define INTEGRATE_H

double integrate_weighted(double pos, double start_v, double half_accel,
                          double start, double end,
                          double toff, double hst);
double calc_inv_norm(double hst);
double integrate_time_weight(double toff, double hst);

#endif // integrate.h
