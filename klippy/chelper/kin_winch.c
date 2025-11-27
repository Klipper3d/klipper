// Cable winch stepper kinematics with flex compensation
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2024       Contributors
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h>   // sqrt, fabs, fmax, fmin
#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <stdio.h> // malloc
#include <string.h> // memset
#include <float.h> // DBL_MAX
#include "compiler.h" // __visible
#include "pyhelper.h" // errorf
#include "itersolve.h" // struct stepper_kinematics
#include "trapq.h" // move_get_coord

#define WINCH_MAX_ANCHORS 26
#define EPSILON 1e-9
#define G_ACCEL 9.81
// LAMBDA is Tikhonov regularization weight in both the Tikhonov and the QP solver.
#define LAMBDA 1e-3

enum winch_force_algorithm {
    WINCH_FORCE_ALGO_TIKHONOV = 0,
    WINCH_FORCE_ALGO_QP = 1,
};

struct winch_flex {
    struct coord anchors[WINCH_MAX_ANCHORS];
    int num_anchors;
    int enabled;
    int flex_compensation_algorithm;
    double mover_weight;
    double spring_constant;
    double min_force[WINCH_MAX_ANCHORS];
    double max_force[WINCH_MAX_ANCHORS];
    double guy_wires[WINCH_MAX_ANCHORS];
    int ignore_gravity;
    int ignore_pretension;
    double distances_origin[WINCH_MAX_ANCHORS];
    double relaxed_origin[WINCH_MAX_ANCHORS];
};

struct winch_stepper {
    struct stepper_kinematics sk;
    struct winch_flex *wf;
    int index;
    struct coord anchor;
};

static inline double
hypot3(double dx, double dy, double dz)
{
    return sqrt(dx*dx + dy*dy + dz*dz);
}

static int
invert3x3(const double M[3][3], double Minv[3][3])
{
    double a = M[0][0], b = M[0][1], c = M[0][2];
    double d = M[1][0], e = M[1][1], f = M[1][2];
    double g = M[2][0], h = M[2][1], i = M[2][2];

    double A =  (e * i - f * h);
    double B = -(d * i - f * g);
    double C =  (d * h - e * g);
    double D = -(b * i - c * h);
    double E =  (a * i - c * g);
    double F = -(a * h - b * g);
    double G =  (b * f - c * e);
    double H = -(a * f - c * d);
    double I =  (a * e - b * d);

    double det = a * A + b * B + c * C;
    if (fabs(det) < EPSILON)
        return 0;
    double invdet = 1. / det;

    Minv[0][0] = A * invdet;
    Minv[0][1] = D * invdet;
    Minv[0][2] = G * invdet;
    Minv[1][0] = B * invdet;
    Minv[1][1] = E * invdet;
    Minv[1][2] = H * invdet;
    Minv[2][0] = C * invdet;
    Minv[2][1] = F * invdet;
    Minv[2][2] = I * invdet;
    return 1;
}

static int
build_direction_matrix(struct winch_flex *wf, const struct coord *pos,
                       double *A)
{
    int N = wf->num_anchors;
    int valid = 0;
    for (int j = 0; j < N; ++j) {
        double dx = wf->anchors[j].x - pos->x;
        double dy = wf->anchors[j].y - pos->y;
        double dz = wf->anchors[j].z - pos->z;
        double norm = hypot3(dx, dy, dz);
        if (norm < EPSILON) {
            A[0 * N + j] = 0.;
            A[1 * N + j] = 0.;
            A[2 * N + j] = 0.;
            continue;
        }
        double inv = 1.0 / norm;
        A[0 * N + j] = dx * inv;
        A[1 * N + j] = dy * inv;
        A[2 * N + j] = dz * inv;
        valid++;
    }
    return valid >= 3;
}

static void
solve_min_norm_T(const double *A, int N, const double Fext[3], double lambda,
                 double *T)
{
    double S[3][3] = {
        {lambda, 0., 0.},
        {0., lambda, 0.},
        {0., 0., lambda}
    };
    for (int j = 0; j < N; ++j) {
        double ax = A[0 * N + j], ay = A[1 * N + j], az = A[2 * N + j];
        S[0][0] += ax * ax;
        S[0][1] += ax * ay;
        S[0][2] += ax * az;
        S[1][0] += ay * ax;
        S[1][1] += ay * ay;
        S[1][2] += ay * az;
        S[2][0] += az * ax;
        S[2][1] += az * ay;
        S[2][2] += az * az;
    }
    double Sinv[3][3];
    if (!invert3x3(S, Sinv)) {
        S[0][0] += 1e-6;
        S[1][1] += 1e-6;
        S[2][2] += 1e-6;
        invert3x3(S, Sinv);
    }

    double y0 = Sinv[0][0] * Fext[0] + Sinv[0][1] * Fext[1] + Sinv[0][2] * Fext[2];
    double y1 = Sinv[1][0] * Fext[0] + Sinv[1][1] * Fext[1] + Sinv[1][2] * Fext[2];
    double y2 = Sinv[2][0] * Fext[0] + Sinv[2][1] * Fext[1] + Sinv[2][2] * Fext[2];

    for (int j = 0; j < N; ++j) {
        double ax = A[0 * N + j], ay = A[1 * N + j], az = A[2 * N + j];
        T[j] = ax * y0 + ay * y1 + az * y2;
    }
}

static void
build_null_projector(const double *A, int N, double lambda, double *P)
{
    double S[3][3] = {
        {lambda, 0., 0.},
        {0., lambda, 0.},
        {0., 0., lambda}
    };
    for (int j = 0; j < N; ++j) {
        double ax = A[0 * N + j], ay = A[1 * N + j], az = A[2 * N + j];
        S[0][0] += ax * ax;
        S[0][1] += ax * ay;
        S[0][2] += ax * az;
        S[1][0] += ay * ax;
        S[1][1] += ay * ay;
        S[1][2] += ay * az;
        S[2][0] += az * ax;
        S[2][1] += az * ay;
        S[2][2] += az * az;
    }
    double Sinv[3][3];
    if (!invert3x3(S, Sinv)) {
        S[0][0] += 1e-6;
        S[1][1] += 1e-6;
        S[2][2] += 1e-6;
        invert3x3(S, Sinv);
    }
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            double ax = A[0 * N + c], ay = A[1 * N + c], az = A[2 * N + c];
            double B0 = Sinv[0][0] * ax + Sinv[0][1] * ay + Sinv[0][2] * az;
            double B1 = Sinv[1][0] * ax + Sinv[1][1] * ay + Sinv[1][2] * az;
            double B2 = Sinv[2][0] * ax + Sinv[2][1] * ay + Sinv[2][2] * az;
            double arx = A[0 * N + r], ary = A[1 * N + r], arz = A[2 * N + r];
            double Mrc = arx * B0 + ary * B1 + arz * B2;
            P[r * N + c] = (r == c ? 1.0 : 0.0) - Mrc;
        }
    }
}

static void
project_nullspace(const double *P, int N, const double *v, double *out)
{
    for (int r = 0; r < N; ++r) {
        double acc = 0.;
        for (int c = 0; c < N; ++c)
            acc += P[r * N + c] * v[c];
        out[r] = acc;
    }
}

static int
static_forces_tikhonov(struct winch_flex *wf, const struct coord *pos,
                       double *forces)
{
    int N = wf->num_anchors;
    double A[3 * WINCH_MAX_ANCHORS];
    if (!build_direction_matrix(wf, pos, A))
        return 0;

    double Fext[3] = {0., 0., 0.};
    double T[WINCH_MAX_ANCHORS];
    memset(T, 0, sizeof(T));
    if (!wf->ignore_gravity) {
        Fext[2] = wf->mover_weight * G_ACCEL;
        // base min-norm solution (regularized pseudoinverse)
        // Magic number LAMBDA (1e-3) is regulatization strength,
        // damping the least squares problem.
        // It gives Thikonov a ~milliforce
        // leeway to make nicer (no blow up) but less exact solutions
        solve_min_norm_T(A, N, Fext, LAMBDA, T);
    }

    if (!wf->ignore_pretension) {
        double P[WINCH_MAX_ANCHORS * WINCH_MAX_ANCHORS];
        build_null_projector(A, N, LAMBDA, P);
        const double tol = 1e-3;
        // Magic step_damp number that makes gradient descent typically converge within tol fast
        // Anything within [0.1, 1.5] works
        const double step_damp = 0.75;
        const int max_iters = 100;
        for (int it = 0; it < max_iters; ++it) {
            double gradient[WINCH_MAX_ANCHORS];
            for (int i = 0; i < N; ++i) {
                // Gradient from the target objective: 0.5 * (T - T_target)^2
                double target_grad = 0.;
                // If tension is beyond max, pull strongly back into acceptable range
                if (T[i] > wf->max_force[i])
                    target_grad += T[i] - wf->max_force[i];
                // If tenson is below min, pull strongly back into acceptable range
                if (T[i] < wf->min_force[i])
                    target_grad += T[i] - wf->min_force[i];
                // Always pull weakly towards the minvalue
                target_grad += 0.1 * (T[i] - wf->min_force[i]);
                gradient[i] = target_grad;
            }
            double d[WINCH_MAX_ANCHORS];
            project_nullspace(P, N, gradient, d);
            double norm_sq = 0.;
            for (int i = 0; i < N; ++i)
                norm_sq += d[i] * d[i];
            if (norm_sq < tol * tol) {
                break;
            }
            for (int i = 0; i < N; ++i)
                T[i] -= step_damp * d[i];
        }

        // Hard cap on Tmax and 0 if we don't ignore pretension
        // Allow negative forces if the user asked for it
        for (int i=0;i<N;++i){
            if (wf->min_force[i] >= 0. && T[i] < 0.) T[i] = 0.;
            if (T[i] > wf->max_force[i]) T[i] = wf->max_force[i];
        }
    }

    for (int i = 0; i < N; ++i) {
        forces[i] = T[i];
    }
    for (int i = N; i < WINCH_MAX_ANCHORS; ++i)
        forces[i] = 0.;
    return 1;
}


// ---- Cholesky solver for small SPD systems (k x k), row-major in/out -------
static int
chol_decompose_spd(double *G, int k)
{
    const double eps = 1e-12;
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j <= i; ++j) {
            double s = G[i * k + j];
            for (int p = 0; p < j; ++p)
                s -= G[i * k + p] * G[j * k + p];
            if (i == j) {
                if (s <= eps)
                    s = eps;
                G[i * k + j] = sqrt(s);
            } else {
                G[i * k + j] = s / G[j * k + j];
            }
        }
        for (int j = i + 1; j < k; ++j)
            G[i * k + j] = 0.;
    }
    return 1;
}

static void
chol_solve_spd(const double *L, int k, const double *b, double *x)
{
    double y[WINCH_MAX_ANCHORS] = { 0.0 };
    // Solve L y = b
    for (int i = 0; i < k; ++i) {
        double s = b[i];
        for (int p = 0; p < i; ++p)
            s -= L[i * k + p] * y[p];
        y[i] = s / L[i * k + i];
    }
    for (int i = 0; i < k; ++i)
        x[i] = 0.;
    // Solve L^T x = y
    for (int i = k - 1; i >= 0; --i) {
        double s = y[i];
        for (int p = i + 1; p < k; ++p)
            s -= L[p * k + i] * x[p];
        x[i] = s / L[i * k + i];
    }
}

static double
projected_gradient_norm(const double *H, const double *t, const double *f,
                        const double *Lbounds, const double *Ubounds,
                        int N)
{
    double s2 = 0.;
    for (int i = 0; i < N; ++i) {
        double gi = 0.;
        for (int j = 0; j < N; ++j)
            gi += H[i * N + j] * t[j];
        gi -= f[i];
        double li = Lbounds ? Lbounds[i] : 0.;
        double ui = Ubounds ? Ubounds[i] : DBL_MAX;
        if (ui < li)
            ui = li;
        int atL = t[i] <= li + 1e-12;
        int atU = t[i] >= ui - 1e-12;
        double pgi = gi;
        if (atL && gi > 0.)
            pgi = 0.;
        if (atU && gi < 0.)
            pgi = 0.;
        s2 += pgi * pgi;
    }
    return sqrt(s2);
}


// -------- Bound-constrained ridge LS: H = A^T A + lambda I, f = A^T F --------
// minimize 0.5 T^T H T - f^T T, s.t. L <= T <= U
static int
solve_box_ridge_ls(const double *A, int N, const double Fext[3], double lambda,
                   const double *Lbounds, const double *Ubounds,
                   int max_iters, double tol, double *T_out)
{
    double H[WINCH_MAX_ANCHORS * WINCH_MAX_ANCHORS];
    double f[WINCH_MAX_ANCHORS] = { 0.0 };
    for (int i = 0; i < N; ++i) {
        double aix = A[0 * N + i];
        double aiy = A[1 * N + i];
        double aiz = A[2 * N + i];
        f[i] = aix * Fext[0] + aiy * Fext[1] + aiz * Fext[2];
        for (int j = 0; j <= i; ++j) {
            double ajx = A[0 * N + j];
            double ajy = A[1 * N + j];
            double ajz = A[2 * N + j];
            double dot = aix * ajx + aiy * ajy + aiz * ajz;
            double v = dot + (i == j ? lambda : 0.);
            H[i * N + j] = v;
            H[j * N + i] = v;
        }
    }

    double Lfact[WINCH_MAX_ANCHORS * WINCH_MAX_ANCHORS];
    memcpy(Lfact, H, sizeof(double) * N * N);
    if (!chol_decompose_spd(Lfact, N))
        return 0;
    double t[WINCH_MAX_ANCHORS];
    chol_solve_spd(Lfact, N, f, t);
    for (int i = 0; i < N; ++i) {
        double li = Lbounds ? Lbounds[i] : 0.;
        double ui = Ubounds ? Ubounds[i] : DBL_MAX;
        if (ui < li)
            ui = li;
        if (t[i] < li)
            t[i] = li;
        if (t[i] > ui)
            t[i] = ui;
    }

    double g[WINCH_MAX_ANCHORS];
    int free_idx[WINCH_MAX_ANCHORS];

    for (int it = 0; it < max_iters; ++it) {
        for (int i = 0; i < N; ++i) {
            double gi = 0.;
            for (int j = 0; j < N; ++j)
                gi += H[i * N + j] * t[j];
            g[i] = gi - f[i];
        }

        int k = 0;
        for (int i = 0; i < N; ++i) {
            double li = Lbounds ? Lbounds[i] : 0.;
            double ui = Ubounds ? Ubounds[i] : DBL_MAX;
            if (ui < li)
                ui = li;
            int atL = t[i] <= li + 1e-12;
            int atU = t[i] >= ui - 1e-12;
            int violateL = atL && (g[i] < -tol);
            int violateU = atU && (g[i] > tol);
            if ((!atL && !atU) || violateL || violateU)
                free_idx[k++] = i;
        }

        double pgn = projected_gradient_norm(H, t, f, Lbounds, Ubounds, N);
        if (pgn <= tol)
            break;

        if (!k) {
            int best_idx = -1;
            double best = 0.;
            for (int i = 0; i < N; ++i) {
                double li = Lbounds ? Lbounds[i] : 0.;
                double ui = Ubounds ? Ubounds[i] : DBL_MAX;
                if (ui < li)
                    ui = li;
                int atL = t[i] <= li + 1e-12;
                int atU = t[i] >= ui - 1e-12;
                double viol = 0.;
                if (atL)
                    viol = fmax(0., -g[i]);
                else if (atU)
                    viol = fmax(0., g[i]);
                if (viol > best) {
                    best = viol;
                    best_idx = i;
                }
            }
            if (best_idx < 0)
                break;
            free_idx[k++] = best_idx;
        }

        int ksize = k;
        double Hff[WINCH_MAX_ANCHORS * WINCH_MAX_ANCHORS];
        double gf[WINCH_MAX_ANCHORS];
        double pf[WINCH_MAX_ANCHORS];
        for (int p = 0; p < ksize; ++p) {
            int ip = free_idx[p];
            gf[p] = g[ip];
            for (int q = 0; q < ksize; ++q) {
                int iq = free_idx[q];
                Hff[p * ksize + q] = H[ip * N + iq];
            }
        }
        if (!chol_decompose_spd(Hff, ksize))
            return 0;
        for (int i = 0; i < ksize; ++i)
            gf[i] = -gf[i];
        chol_solve_spd(Hff, ksize, gf, pf);

        double alpha = 1.0;
        for (int idx = 0; idx < ksize; ++idx) {
            int i = free_idx[idx];
            double pi = pf[idx];
            if (fabs(pi) < 1e-16)
                continue;
            double li = Lbounds ? Lbounds[i] : 0.;
            double ui = Ubounds ? Ubounds[i] : DBL_MAX;
            if (ui < li)
                ui = li;
            if (pi > 0.) {
                double amax = (ui - t[i]) / pi;
                if (amax < alpha)
                    alpha = amax > 0. ? amax : 0.;
            } else if (pi < 0.) {
                double amax = (li - t[i]) / pi;
                if (amax < alpha)
                    alpha = amax > 0. ? amax : 0.;
            }
        }
        if (alpha < 0.)
            alpha = 0.;

        for (int idx = 0; idx < ksize; ++idx) {
            int i = free_idx[idx];
            t[i] += alpha * pf[idx];
        }
        for (int i = 0; i < N; ++i) {
            double li = Lbounds ? Lbounds[i] : 0.;
            double ui = Ubounds ? Ubounds[i] : DBL_MAX;
            if (ui < li)
                ui = li;
            if (t[i] < li)
                t[i] = li;
            if (t[i] > ui)
                t[i] = ui;
        }
    }

    for (int i = 0; i < N; ++i)
        T_out[i] = t[i];
    return 1;
}

static int
static_forces_qp(struct winch_flex *wf, const struct coord *pos,
                 double *forces)
{
    int N = wf->num_anchors;

    double A[3 * WINCH_MAX_ANCHORS];
    if (!build_direction_matrix(wf, pos, A))
        return 0;

    double Fext[3] = {0., 0., 0.};
    if (!wf->ignore_gravity)
        Fext[2] = wf->mover_weight * G_ACCEL;

    double Lbounds[WINCH_MAX_ANCHORS] = { 0. };
    double Ubounds[WINCH_MAX_ANCHORS] = { 0. };
    for (int i = 0; i < N; ++i) {
        double li = wf->ignore_pretension ? 0. : wf->min_force[i];
        double ui = wf->max_force[i];
        if (ui < li)
            ui = li;
        Lbounds[i] = li;
        Ubounds[i] = ui;
    }

    // Solve convex QP
    const int max_iters = 100;
    const double tol = 1e-3;
    double T[WINCH_MAX_ANCHORS];
    if (!solve_box_ridge_ls(A, N, Fext, LAMBDA, Lbounds, Ubounds,
                            max_iters, tol, T))
        return 0;

    for (int i = 0; i < N; ++i)
        forces[i] = T[i];
    for (int i = N; i < WINCH_MAX_ANCHORS; ++i)
        forces[i] = 0.;
    return 1;
}

static int
compute_static_forces(struct winch_flex *wf, const struct coord *pos,
                      double *forces, int algo)
{
    int ok = 0;
    if (algo == WINCH_FORCE_ALGO_QP) {
        ok = static_forces_qp(wf, pos, forces);
    } else if (algo == WINCH_FORCE_ALGO_TIKHONOV) {
        ok = static_forces_tikhonov(wf, pos, forces);
    }
    if (ok) {
        return 1;
    }
    memset(forces, 0, sizeof(double) * wf->num_anchors);
    return 0;
}

static void
compute_flex(struct winch_flex *wf, double x, double y, double z,
             double *distances, double *flex)
{
    int num = wf->num_anchors;
    struct coord pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    for (int i = 0; i < num; ++i) {
        double dx = wf->anchors[i].x - x;
        double dy = wf->anchors[i].y - y;
        double dz = wf->anchors[i].z - z;
        double dist = hypot3(dx, dy, dz);
        distances[i] = dist;
    }
    if (!wf->enabled) {
        for (int i = 0; i < num; ++i)
            flex[i] = 0.;
        return;
    }
    double forces[WINCH_MAX_ANCHORS];
    memset(forces, 0, sizeof(forces));
    compute_static_forces(wf, &pos, forces, wf->flex_compensation_algorithm);
    for (int i = 0; i < num; ++i) {
        double spring_length = distances[i] + wf->guy_wires[i];
        if (spring_length < EPSILON)
            spring_length = EPSILON;
        double spring_k = wf->spring_constant / spring_length;
        flex[i] = forces[i] / spring_k;
    }
}
static double
calc_position_common(struct winch_stepper *ws, struct move *m, double move_time)
{
    struct coord pos = move_get_coord(m, move_time);
    double dx = ws->anchor.x - pos.x;
    double dy = ws->anchor.y - pos.y;
    double dz = ws->anchor.z - pos.z;
    double dist = hypot3(dx, dy, dz);
    struct winch_flex *wf = ws->wf;
    if (!wf || ws->index >= wf->num_anchors || !wf->enabled)
        return dist;
    double distances[WINCH_MAX_ANCHORS];
    double flex[WINCH_MAX_ANCHORS];
    compute_flex(wf, pos.x, pos.y, pos.z, distances, flex);
    return dist - flex[ws->index];
}

static double
winch_stepper_calc_position(struct stepper_kinematics *sk, struct move *m,
                            double move_time)
{
    struct winch_stepper *ws = container_of(sk, struct winch_stepper, sk);
    return calc_position_common(ws, m, move_time);
}

struct winch_flex * __visible
winch_flex_alloc(void)
{
    struct winch_flex *wf = malloc(sizeof(*wf));
    if (!wf)
        return NULL;
    memset(wf, 0, sizeof(*wf));
    return wf;
}

void __visible
winch_flex_free(struct winch_flex *wf)
{
    free(wf);
}

static void
recalc_origin(struct winch_flex *wf)
{
    int num = wf->num_anchors;
    if (num <= 0)
        return;

    for (int i = 0; i < num; ++i) {
        double dx = wf->anchors[i].x;
        double dy = wf->anchors[i].y;
        double dz = wf->anchors[i].z;
        wf->distances_origin[i] = hypot3(dx, dy, dz);
    }

    if (!wf->enabled) {
        for (int i = 0; i < num; ++i)
            wf->relaxed_origin[i] = wf->distances_origin[i];
        return;
    }

    struct coord origin;
    origin.x = 0.;
    origin.y = 0.;
    origin.z = 0.;
    double forces[WINCH_MAX_ANCHORS] = {0.};
    compute_static_forces(wf, &origin, forces, wf->flex_compensation_algorithm);

    double pretension[WINCH_MAX_ANCHORS] = {0.};
    for (int i = 0; i < num; ++i) {
        double spring_length = wf->distances_origin[i] + wf->guy_wires[i];
        if (spring_length < EPSILON)
            spring_length = EPSILON;
        double spring_k = wf->spring_constant / spring_length;
        double tension = forces[i] / spring_k;
        pretension[i] = tension;
        double relaxed = wf->distances_origin[i] - tension;
        wf->relaxed_origin[i] = relaxed;
    }
    if (wf->enabled && num > 0) {
        //char msg[256];
        //int len = snprintf(msg, sizeof(msg), "winch pretension at origin:");
        //for (int i = 0; i < num && len > 0 && len < (int)sizeof(msg); ++i) {
        //    len += snprintf(msg + len, sizeof(msg) - len,
        //                    " %d:%.6f", i, pretension[i]);
        //    if (len >= (int)sizeof(msg))
        //        break;
        //}
        //errorf("%s", msg);
    }
}

void __visible
winch_flex_configure(struct winch_flex *wf,
                     int num_anchors,
                     const double *anchors,
                     double mover_weight,
                     double spring_constant,
                     const double *min_force,
                     const double *max_force,
                     const double *guy_wires,
                     int flex_compensation_algorithm,
                     int ignore_gravity,
                     int ignore_pretension)
{
    if (!wf)
        return;
    if (num_anchors < 0)
        num_anchors = 0;
    if (num_anchors > WINCH_MAX_ANCHORS)
        num_anchors = WINCH_MAX_ANCHORS;
    wf->num_anchors = num_anchors;
    wf->mover_weight = mover_weight;
    wf->spring_constant = spring_constant;
    wf->flex_compensation_algorithm = flex_compensation_algorithm;
    wf->ignore_gravity = ignore_gravity;
    wf->ignore_pretension = ignore_pretension;

    for (int i = 0; i < num_anchors; ++i) {
        wf->anchors[i].x = anchors[i * 3];
        wf->anchors[i].y = anchors[i * 3 + 1];
        wf->anchors[i].z = anchors[i * 3 + 2];
        wf->min_force[i] = min_force ? min_force[i] : 0.;
        wf->max_force[i] = max_force ? max_force[i] : 120.0;
        if (guy_wires)
            wf->guy_wires[i] = guy_wires[i];
        else
            wf->guy_wires[i] = 0.;
    }
    for (int i = num_anchors; i < WINCH_MAX_ANCHORS; ++i) {
        wf->anchors[i].x = wf->anchors[i].y = wf->anchors[i].z = 0.;
        wf->min_force[i] = 0.;
        wf->max_force[i] = 120.0;
        wf->guy_wires[i] = 0.;
    }
    if (wf->flex_compensation_algorithm < WINCH_FORCE_ALGO_TIKHONOV
        || wf->flex_compensation_algorithm > WINCH_FORCE_ALGO_QP) {
        wf->flex_compensation_algorithm = WINCH_FORCE_ALGO_QP;
    }
    recalc_origin(wf);
}

void __visible
winch_flex_calc_arrays(struct winch_flex *wf, double x, double y, double z,
                       double *distances_out, double *flex_out)
{
    if (!wf || wf->num_anchors <= 0)
        return;
    compute_flex(wf, x, y, z, distances_out, flex_out);
}

void __visible
winch_flex_set_enabled(struct winch_flex *wf, int enabled)
{
    if (!wf)
        return;
    wf->enabled = enabled ? 1 : 0;
    recalc_origin(wf);
}

struct stepper_kinematics * __visible
winch_stepper_alloc(struct winch_flex *wf, int index)
{
    struct winch_stepper *ws = malloc(sizeof(*ws));
    if (!ws)
        return NULL;
    memset(ws, 0, sizeof(*ws));
    ws->wf = wf;
    ws->index = index;
    if (wf && index >= 0 && index < wf->num_anchors)
        ws->anchor = wf->anchors[index];
    ws->sk.calc_position_cb = winch_stepper_calc_position;
    ws->sk.active_flags = AF_X | AF_Y | AF_Z;
    return &ws->sk;
}
