#ifndef INTEGRATE_H
#define INTEGRATE_H

struct smoother {
    double c[12];
    double hst, t_offs;
    int n;
};

int init_smoother(int n, const double a[], double t_sm, struct smoother* sm);
double integrate_weighted(const struct smoother *sm,
                          double pos, double start_v, double half_accel,
                          double start, double end, double t0);

#endif // integrate.h
