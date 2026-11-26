// Shared by the development solvers, RepRapFirmware and Klipper.
// A swivel pulley's fork turns about a fixed axis through the point where the
// line enters it. The line arrives travelling along the axis, wraps the wheel
// and leaves from a tangent point towards the mover. Zero radius is a pinhole.
#ifndef HANGPRINTER_SWIVEL_PULLEY_H
#define HANGPRINTER_SWIVEL_PULLEY_H
#include <math.h>

// entry: where the line enters the pulley, axis: unit direction of travel of
// the incoming line, p: where the line ends. Returns the line length from entry
// to p (wrap plus free span) and writes the point where the line leaves the wheel.
static inline double hp_swivel_length(const double entry[3], const double axis[3], double radius,
                                      const double p[3], double exit[3]) {
    double v[3]={p[0]-entry[0],p[1]-entry[1],p[2]-entry[2]};
    double z=v[0]*axis[0]+v[1]*axis[1]+v[2]*axis[2];
    double e[3]={v[0]-z*axis[0],v[1]-z*axis[1],v[2]-z*axis[2]};
    double rho=sqrt(e[0]*e[0]+e[1]*e[1]+e[2]*e[2]);
    // A line along the swivel axis has no wrap plane; treat it as a pinhole.
    if (!(radius>0.) || rho<1e-9) {
        for (int j=0;j<3;++j) exit[j]=entry[j];
        return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    }
    for (int j=0;j<3;++j) e[j]/=rho;
    // In the wheel plane, with the wheel centre at entry + radius*e
    double c=rho-radius, d=sqrt(c*c+z*z);
    double ratio=d>radius ? radius/d : 1.;
    double theta=atan2(z,c)+acos(ratio);
    double span=d>radius ? sqrt(d*d-radius*radius) : 0.;
    for (int j=0;j<3;++j) exit[j]=entry[j]+radius*(e[j]*(1.+cos(theta))+axis[j]*sin(theta));
    return span+radius*(3.14159265358979323846-theta);
}

#endif
