#ifndef INTEGRATE_H
#define INTEGRATE_H

struct smoother {
    double c0[14], c1[14], c2[14];
    double hst, t_offs;
    int n, symm;
};

struct move;

int init_smoother(int n, const double a[], double t_sm, struct smoother* sm);
double integrate_weighted(const struct move* m, int axis
                          , const struct smoother *sm, double base
                          , double start, double end, double t0
                          , double* smooth_velocity);

#endif // integrate.h
