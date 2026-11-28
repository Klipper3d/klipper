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
    if (!(radius>0.)) {
        for (int j=0;j<3;++j) exit[j]=entry[j];
        return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    }
    if (rho<1e-9) {
        // On the axis every fork angle gives the same length; pick any wheel plane.
        int k=fabs(axis[0])<.6 ? 0 : (fabs(axis[1])<.6 ? 1 : 2);
        for (int j=0;j<3;++j) e[j]=(j==k)-axis[k]*axis[j];
        double n=sqrt(e[0]*e[0]+e[1]*e[1]+e[2]*e[2]);
        for (int j=0;j<3;++j) e[j]/=n;
    } else {
        for (int j=0;j<3;++j) e[j]/=rho;
    }
    // In the wheel plane, with the wheel centre at entry + radius*e
    double c=rho-radius, d=sqrt(c*c+z*z);
    double ratio=d>radius ? radius/d : 1.;
    double theta=atan2(z,c)+acos(ratio);
    // Inside the wheel (d <= radius) there is no tangent; the span collapses to zero.
    double span=d>radius ? sqrt(d*d-radius*radius) : 0.;
    for (int j=0;j<3;++j) exit[j]=entry[j]+radius*(e[j]*(1.+cos(theta))+axis[j]*sin(theta));
    return span+radius*(3.14159265358979323846-theta);
}

// The exit point turns with the fork about the axis, so across the wheel plane
// the length bends less than a fixed exit would make it: its Hessian is
// (I - u u^T - k n n^T) / span, with u the free span direction and n the wheel
// plane normal written here. Returns k, the exit's distance from the axis over
// p's distance (0 for pinholes).
static inline double hp_swivel_turn(const double entry[3], const double axis[3], double radius,
                                    const double p[3], const double exit[3], double normal[3]) {
    double v[3]={p[0]-entry[0],p[1]-entry[1],p[2]-entry[2]};
    double w[3]={exit[0]-entry[0],exit[1]-entry[1],exit[2]-entry[2]};
    double zv=v[0]*axis[0]+v[1]*axis[1]+v[2]*axis[2], zw=w[0]*axis[0]+w[1]*axis[1]+w[2]*axis[2];
    for (int j=0;j<3;++j) { v[j]-=zv*axis[j]; w[j]-=zw*axis[j]; }
    double rv=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]), rw=sqrt(w[0]*w[0]+w[1]*w[1]+w[2]*w[2]);
    for (int j=0;j<3;++j) normal[j]=0.;
    if (!(radius>0.) || rv<1e-9) return 0.;
    normal[0]=(axis[1]*v[2]-axis[2]*v[1])/rv;
    normal[1]=(axis[2]*v[0]-axis[0]*v[2])/rv;
    normal[2]=(axis[0]*v[1]-axis[1]*v[0])/rv;
    return rw/rv;
}

#endif
