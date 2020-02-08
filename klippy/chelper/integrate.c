// Helpers to integrate the smoothing weight function.
//
// Copyright (C) 2019-2020  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2020  Dmitry Butyugin <dmbutyugin@google.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "integrate.h"

// Integrate (t^2-h^2)^2
static inline double
iwt0(double h, double t)
{
    double t2 = t*t;
    double h2 = h*h;
    double h4 = h2*h2;
    return ((.2 * t2 - (2./3.) * h2) * t2 + h4) * t;
}

// Integrate t * (t^2-h^2)^2
static inline double
iwt1(double h, double t)
{
    double t2 = t*t;
    double h2 = h*h;
    double r = t2-h2;
    return (1./6.) * r * r * r;
}

// Integrate t^2 * (t^2-h^2)^2
static inline double
iwt2(double h, double t)
{
    double t2 = t*t;
    double h2 = h*h;
    double h4 = h2*h2;
    return (((1./7.) * t2 - .4 * h2) * t2 + (1./3.) * h4) * t2 * t;
}

// Integrate (pos + start_v*t + half_accel*t^2) with smoothing weight function
// ((t-T)^2-h^2)^2 over the range [start; end] with T == -toff
double
integrate_weighted(double pos, double start_v, double half_accel
                   , double start, double end, double toff, double hst)
{
    // Substitute the integration variable tnew = t + toff to simplify integrals
    pos += (half_accel * toff - start_v) * toff;
    start_v -= 2. * half_accel * toff;
    start += toff; end += toff;
    double res = half_accel * (iwt2(hst, end) - iwt2(hst, start));
    res += start_v * (iwt1(hst, end) - iwt1(hst, start));
    res += pos * (iwt0(hst, end) - iwt0(hst, start));
    return res;
}

// Calculate the inverse of the norm of the weight function ((t-T)^2-h^2)^2
double
calc_inv_norm(double hst)
{
    return 15. / (16. * hst * hst * hst * hst * hst);
}
