#ifndef INTEGRATE_H
#define INTEGRATE_H

typedef struct {
    double it0, it1, it2;
} smoother_antiderivatives;

struct smoother {
    double c0[10], c1[10], c2[10];
    double hst, t_offs;
    smoother_antiderivatives m_hst, p_hst, pm_diff;
    int n, symm;
};

struct move;

int init_smoother(int n, const double a[], double t_sm, struct smoother* sm);

double integrate_move(const struct move* m, int axis, double base, double t0
                      , const smoother_antiderivatives* s
                      , double* smooth_velocity);

smoother_antiderivatives
calc_antiderivatives(const struct smoother* sm, double t);
smoother_antiderivatives
diff_antiderivatives(const smoother_antiderivatives* ad1
                     , const smoother_antiderivatives* ad2);

#endif // integrate.h
