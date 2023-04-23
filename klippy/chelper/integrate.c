// Helpers to integrate the smoothing weight function
//
// Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2020-2023  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "compiler.h"
#include "integrate.h"

#include <string.h>

/****************************************************************
 * Generic smoother integration
 ****************************************************************/

static double coeffs[] = {
    1./1., 1./2., 1./3., 1./4., 1./5., 1./6., 1./7., 1./8., 1./9., 1./10.,
    1./11., 1./12., 1./13., 1./14., 1./15., 1./16., 1./17., 1./18., 1./19.,
};

inline static double
integrate_nth_order(int n, const double c[], double start, double end
                    , double a0, double a1, double a2)
{
    double u = start + end;
    double v = u * .5;
    double end_n = end;
    // First 2 iterations with i == 0 and 1
    double sum0 = c[0] + c[1] * v, sum1 = c[0] * v, sum2 = 0.;
    int i;
    for (i = 2; i < n; ++i) {
        end_n *= end;
        u = u * start + end_n;
        v = u * coeffs[i];
        sum2 += c[i-2] * v;
        sum1 += c[i-1] * v;
        sum0 += c[i] * v;
    }
    // Remainging 2 iterations with i == n and n+1
    end_n *= end;
    u = u * start + end_n;
    v = u * coeffs[n];
    sum2 += c[n-2] * v;
    sum1 += c[n-1] * v;

    end_n *= end;
    u = u * start + end_n;
    v = u * coeffs[n+1];
    sum2 += c[n-1] * v;

    double delta_t = end - start;
    double avg_val = a0 * sum0 + a1 * sum1 + a2 * sum2;
    return avg_val * delta_t;
}

// Integrate (pos + start_v*t + half_accel*t^2) with smoothing weight function
// w(t0 - t) over the range [start; end]
inline double
integrate_weighted(const struct smoother *sm, double pos
                   , double start_v, double half_accel
                   , double start, double end, double t0)
{
    // Substitute the integration variable tnew = t0 - t to simplify integrals
    pos += (half_accel * t0 + start_v) * t0;
    start_v = -(start_v + 2. * half_accel * t0);
    return integrate_nth_order(sm->n, sm->c, t0 - end, t0 - start
                               , pos, start_v, half_accel);
}

/****************************************************************
 * Smoother initialization
 ****************************************************************/

int
init_smoother(int n, const double a[], double t_sm, struct smoother* sm)
{
    if (n < 0 || n > ARRAY_SIZE(sm->c))
        return -1;
    memset(sm->c, 0, sizeof(sm->c));
    sm->n = n;
    sm->hst = 0.5 * t_sm;
    if (!t_sm) return 0;
    double inv_t_sm = 1. / t_sm;
    double inv_t_sm_n = inv_t_sm;
    int i;
    for (i = 0; i < n; ++i) {
        sm->c[i] = a[i] * inv_t_sm_n;
        inv_t_sm_n *= inv_t_sm;
    }
    double inv_norm = 1. / integrate_nth_order(n, sm->c, -sm->hst, sm->hst
                                               , 1., 0., 0.);
    for (i = 0; i < n; ++i)
        sm->c[i] *= inv_norm;
    sm->t_offs = integrate_nth_order(n, sm->c, -sm->hst, sm->hst, 0., 1., 0.);
    return 0;
}
