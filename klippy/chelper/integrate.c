// Helpers to integrate the smoothing weight function
//
// Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2020-2023  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "compiler.h" // unlikely
#include "integrate.h"
#include "trapq.h" // struct move

#include <string.h>

/****************************************************************
 * Generic smoother integration
 ****************************************************************/

static double coeffs[] = {
    1./1., 1./2., 1./3., 1./4., 1./5., 1./6., 1./7., 1./8., 1./9., 1./10.,
    1./11., 1./12., 1./13., 1./14., 1./15., 1./16., 1./17., 1./18., 1./19.,
};

inline static void
integrate_smoother(const struct smoother* sm, double start, double end
                   , double* s0, double* s1, double* s2)
{
    int n = sm->n;
    double u = start + end;
    double end_n = end;
    // First 2 iterations with i == 0 and 1
    double sum0 = sm->c0[0] + sm->c0[1] * u, sum1 = sm->c1[0] * u, sum2 = 0.;
    int i;
    for (i = 2; i < n; ++i) {
        end_n *= end;
        u = u * start + end_n;
        sum2 += sm->c2[i-2] * u;
        sum1 += sm->c1[i-1] * u;
        sum0 += sm->c0[i] * u;
    }
    // Remainging 2 iterations with i == n and n+1
    end_n *= end;
    u = u * start + end_n;
    sum2 += sm->c2[n-2] * u;
    sum1 += sm->c1[n-1] * u;

    end_n *= end;
    u = u * start + end_n;
    sum2 += sm->c2[n-1] * u;

    *s0 = sum0;
    *s1 = sum1;
    *s2 = sum2;
}

inline static void
integrate_smoother_symm(const struct smoother* sm, double start, double end
                        , double* s0, double* s1, double* s2)
{
    int n = sm->n;
    double u = start + end;
    double end_n = end;
    // First 2 iterations with i == 0 and 1
    double sum0 = sm->c0[0], sum1 = sm->c1[0] * u, sum2 = 0.;
    int i;
    for (i = 2; i < n; i += 2) {
        end_n *= end;
        u = u * start + end_n;
        sum2 += sm->c2[i-2] * u;
        sum0 += sm->c0[i] * u;
        end_n *= end;
        u = u * start + end_n;
        sum1 += sm->c1[i] * u;
    }
    // Last iteration with i == n+1
    end_n *= end;
    u = u * start + end_n;
    sum2 += sm->c2[n-1] * u;

    *s0 = sum0;
    *s1 = sum1;
    *s2 = sum2;
}

// Integrate (pos + start_v*t + half_accel*t^2) with smoothing weight function
// w(t0 - t) over the range [start; end]
__always_inline double
integrate_weighted(const struct move* m, int axis, const struct smoother *sm
                   , double base, double start, double end, double t0
                   , double* smooth_velocity)
{
    double axis_r = m->axes_r.axis[axis - 'x'];
    double start_v = m->start_v * axis_r;
    double half_accel = m->half_accel * axis_r;
    // Substitute the integration variable tnew = t0 - t to simplify integrals
    double accel = 2. * half_accel;
    base += (half_accel * t0 + start_v) * t0;
    start_v += accel * t0;
    if (start <= 0.) start = 0.;
    if (end >= m->move_t) end = m->move_t;
    double delta_t = end - start;
    double s0, s1, s2;
    if (unlikely(sm->symm))
        integrate_smoother_symm(sm, t0 - end, t0 - start, &s0, &s1, &s2);
    else
        integrate_smoother(sm, t0 - end, t0 - start, &s0, &s1, &s2);
    double smooth_pos = (base * s0 - start_v * s1 + half_accel * s2) * delta_t;
    if (smooth_velocity)
        *smooth_velocity = (start_v * s0 - accel * s1) * delta_t;
    return smooth_pos;
}

/****************************************************************
 * Smoother initialization
 ****************************************************************/

int
init_smoother(int n, const double a[], double t_sm, struct smoother* sm)
{
    if (n < 2 || n + 2 > ARRAY_SIZE(sm->c0))
        return -1;
    memset(sm, 0, sizeof(*sm));
    sm->n = n;
    sm->hst = 0.5 * t_sm;
    if (!t_sm) return 0;
    double inv_t_sm = 1. / t_sm;
    double inv_t_sm_n = inv_t_sm;
    int i, symm = n & 1;
    for (i = 0; i < n; ++i) {
        if ((i & 1) && a[i]) symm = 0;
        double c = a[i] * inv_t_sm_n;
        sm->c0[i] = c * coeffs[i];
        sm->c1[i] = c * coeffs[i+1];
        sm->c2[i] = c * coeffs[i+2];
        inv_t_sm_n *= inv_t_sm;
    }
    sm->symm = symm;
    double norm, t_int, t2_int;
    integrate_smoother(sm, -sm->hst, sm->hst, &norm, &t_int, &t2_int);
    double inv_norm = inv_t_sm / norm;
    for (i = 0; i < n; ++i) {
        sm->c0[i] *= inv_norm;
        sm->c1[i] *= inv_norm;
        sm->c2[i] *= inv_norm;
    }
    integrate_smoother(sm, -sm->hst, sm->hst, &norm, &t_int, &t2_int);
    sm->t_offs = t_int * t_sm;
    return 0;
}
