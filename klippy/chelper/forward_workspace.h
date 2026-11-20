// Shared by the development solver, RepRapFirmware and Klipper.
// Coordinates use a fixed orientation and gravity points along negative Z.
#ifndef HANGPRINTER_FORWARD_WORKSPACE_H
#define HANGPRINTER_FORWARD_WORKSPACE_H
#include <math.h>
#include <stddef.h>

enum { HP_ANCHORS_NONE, HP_ANCHORS_LAST_TOP, HP_ANCHORS_ALL_TOP, HP_ANCHORS_CUBE_CORNER };

static inline double hp_dot(const double *a, const double *b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}
static inline void hp_cross(const double *a, const double *b, double *out) {
    out[0]=a[1]*b[2]-a[2]*b[1]; out[1]=a[2]*b[0]-a[0]*b[2]; out[2]=a[0]*b[1]-a[1]*b[0];
}
static inline void hp_difference(const double *a, const double *b, double *out) {
    for (int j=0;j<3;++j) out[j]=a[j]-b[j];
}
// Closed half-spaces include boundary points. Degenerate faces are invalid.
static inline int hp_same_side(const double *a, const double *b, const double *c,
                               const double *inside, const double *p) {
    double u[3],v[3],n[3],ref[3],q[3];
    hp_difference(b,a,u); hp_difference(c,a,v); hp_cross(u,v,n);
    hp_difference(inside,a,ref); hp_difference(p,a,q);
    double length=sqrt(hp_dot(n,n)), side=hp_dot(n,ref);
    if (length < 1e-12 || fabs(side) < 1e-10*length) return 0;
    return hp_dot(n,q)*(side>0 ? 1 : -1) >= -1e-6*length;
}
static inline int hp_in_prism(const double a[][3], size_t count, const double *p) {
    double center[3]={0,0,0};
    if (count<3) return 0;
    for (size_t i=0;i<count;++i) for (int j=0;j<3;++j) center[j]+=a[i][j]/count;
    for (size_t i=0;i<count;++i) {
        double down[3]={a[i][0],a[i][1],a[i][2]-1};
        if (!hp_same_side(a[i],a[(i+1)%count],down,center,p)) return 0;
    }
    return 1;
}
static inline int hp_reachable(const double a[][3], size_t count, int mode, const double *p) {
    if (mode==HP_ANCHORS_NONE) return 1;
    if (mode==HP_ANCHORS_ALL_TOP) return hp_in_prism(a,count,p);
    if (mode==HP_ANCHORS_LAST_TOP) {
        if (count<4 || !hp_in_prism(a,count-1,p)) return 0;
        // A flattened apex still permits the base's downward prism.
        double u[3],v[3],n[3],q[3],extent=0,height=0;
        hp_difference(a[1],a[0],u);hp_difference(a[2],a[0],v);hp_cross(u,v,n);
        double length=sqrt(hp_dot(n,n));
        for (size_t i=1;i<count;++i) {
            hp_difference(a[i],a[0],q);
            extent=fmax(extent,sqrt(hp_dot(q,q)));height=fmax(height,fabs(hp_dot(q,n)));
        }
        if (length>1e-12 && fabs(n[2])>length*1e-6 && height<=length*extent*1e-7) {
            hp_difference(p,a[0],q);
            return hp_dot(q,n)*(n[2]>0 ? 1 : -1)<=1e-6*length;
        }
        double center[3]={0,0,0};
        for (size_t i=0;i<count-1;++i) for (int j=0;j<3;++j) center[j]+=a[i][j]/(count-1);
        for (size_t i=0;i<count-1;++i)
            if (!hp_same_side(a[i],a[(i+1)%(count-1)],a[count-1],center,p)) return 0;
        return 1;
    }
    if (mode!=HP_ANCHORS_CUBE_CORNER || count!=8) return 0;
    // First four upper, last four lower, corresponding cyclic perimeter order.
    double center[3]={0,0,0};
    for (size_t i=0;i<8;++i) for (int j=0;j<3;++j) center[j]+=a[i][j]/8;
    int inside=hp_same_side(a[0],a[1],a[2],center,p)
            && hp_same_side(a[4],a[5],a[6],center,p);
    for (size_t i=0;i<4 && inside;++i)
        inside=hp_same_side(a[i],a[(i+1)%4],a[i+4],center,p);
    if (inside) return 1;
    if (!hp_in_prism(a+4,4,p)) return 0;
    double below[3]={a[4][0],a[4][1],a[4][2]-1};
    return hp_same_side(a[4],a[5],a[6],below,p);
}

// Gaussian elimination with relative pivoting. The normal system may be rank 2.
static inline int hp_solve_range_system(double s[3][4], double *out) {
    double scale=fmax(s[0][0],fmax(s[1][1],s[2][2]));
    for (int j=0;j<3;++j) {
        int pivot=j;
        for (int r=j+1;r<3;++r) if (fabs(s[r][j])>fabs(s[pivot][j])) pivot=r;
        if (fabs(s[pivot][j])<=scale*1e-14) return 0;
        for (int c=0;c<4;++c) { double t=s[j][c];s[j][c]=s[pivot][c];s[pivot][c]=t; }
        double divisor=s[j][j];
        for (int c=j;c<4;++c) s[j][c]/=divisor;
        for (int r=0;r<3;++r) if (r!=j) {
            double factor=s[r][j];
            for (int c=j;c<4;++c) s[r][c]-=factor*s[j][c];
        }
    }
    for (int j=0;j<3;++j) { if (!isfinite(s[j][3])) return 0; out[j]=s[j][3]; }
    return 1;
}

// Produce a full-rank seed and, for flat/nearly flat layouts, both sphere mirrors.
// Exact planes carry an upward normal so the caller can select the gravity side.
static inline int hp_range_candidates(const double a[][3], const double *distance, size_t count,
                                      double candidates[3][3], double *normal, int *planar) {
    if (count<3) return 0;
    double u[3]={0,0,0},v[3]={0,0,0},largest=0,area=0;
    for (size_t i=1;i<count;++i) {
        double d[3]; hp_difference(a[i],a[0],d);
        double square=hp_dot(d,d);
        if (square>largest) { largest=square;for (int j=0;j<3;++j) u[j]=d[j]; }
    }
    if (largest<1e-12) return 0;
    for (int j=0;j<3;++j) u[j]/=sqrt(largest);
    for (size_t i=1;i<count;++i) {
        double d[3],cross[3]; hp_difference(a[i],a[0],d); hp_cross(u,d,cross);
        double square=hp_dot(cross,cross);
        if (square>area) { area=square;for (int j=0;j<3;++j) normal[j]=cross[j]; }
    }
    if (area<largest*1e-12) return 0; // Collinear anchors cannot locate a 3D mover.
    for (int j=0;j<3;++j) normal[j]/=sqrt(area);
    if (normal[2]<0) for (int j=0;j<3;++j) normal[j]=-normal[j];
    hp_cross(normal,u,v);
    double system[3][4]={{0}}, projected[2][3]={{0}}, height=0;
    for (size_t i=1;i<count;++i) {
        double d[3];hp_difference(a[i],a[0],d);
        double rhs=(distance[0]-distance[i])*(distance[0]+distance[i])+hp_dot(d,d);
        double row[3]={2*d[0],2*d[1],2*d[2]};
        double xy[2]={2*hp_dot(d,u),2*hp_dot(d,v)};
        height=fmax(height,fabs(hp_dot(d,normal)));
        for (int r=0;r<3;++r) {
            for (int c=0;c<3;++c) system[r][c]+=row[r]*row[c];
            system[r][3]+=row[r]*rhs;
        }
        for (int r=0;r<2;++r) {
            for (int c=0;c<2;++c) projected[r][c]+=xy[r]*xy[c];
            projected[r][2]+=xy[r]*rhs;
        }
    }
    *planar=height<=sqrt(largest)*1e-7; // Coordinate-precision coplanarity.
    if (*planar && normal[2]<1e-6) return 0; // Gravity cannot select a vertical-plane mirror.
    int found=0;
    if (hp_solve_range_system(system,candidates[0])) {
        for (int j=0;j<3;++j) candidates[0][j]+=a[0][j];
        found=1;
    }
    if (height>sqrt(largest)*1e-3) return found;
    double det=projected[0][0]*projected[1][1]-projected[0][1]*projected[1][0];
    if (det<=0) return found;
    double x=(projected[0][2]*projected[1][1]-projected[0][1]*projected[1][2])/det;
    double y=(projected[0][0]*projected[1][2]-projected[1][0]*projected[0][2])/det;
    double flat[3],square=0;
    for (int j=0;j<3;++j) flat[j]=a[0][j]+x*u[j]+y*v[j];
    for (size_t i=0;i<count;++i) {
        double d[3];hp_difference(flat,a[i],d);
        square+=(distance[i]*distance[i]-hp_dot(d,d))/count;
    }
    double offset=sqrt(fmax(0,square));
    for (int sign=-1;sign<=1;sign+=2) {
        for (int j=0;j<3;++j) candidates[found][j]=flat[j]+sign*offset*normal[j];
        ++found;
    }
    return found;
}
static inline int hp_forward_allowed(const double a[][3], size_t count, int mode,
                                      const double *normal, int planar, const double *p) {
    if (!isfinite(p[0]) || !isfinite(p[1]) || !isfinite(p[2])) return 0;
    if (!hp_reachable(a,count,mode,p)) return 0;
    double d[3];hp_difference(p,a[0],d);
    return !planar || hp_dot(d,normal)<=1e-6;
}
static inline void hp_reflect_above(const double *origin, const double *normal, int planar, double *p) {
    if (!planar) return;
    double d[3];hp_difference(p,origin,d);
    double height=hp_dot(d,normal);
    if (height>0) for (int j=0;j<3;++j) p[j]-=2*height*normal[j];
}
#endif
