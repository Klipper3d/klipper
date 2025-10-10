// Cable winch stepper kinematics with flex compensation
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2024       Contributors
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <math.h>   // sqrt, fabs, fmax, fmin
#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "itersolve.h" // struct stepper_kinematics
#include "trapq.h" // move_get_coord

#define WINCH_MAX_ANCHORS 26
#define EPSILON 1e-9
#define G_ACCEL 9.81

struct winch_flex {
    int num_anchors;
    int flex_enabled;
    int guy_wires_explicit;
    struct coord anchors[WINCH_MAX_ANCHORS];
    double mover_weight;
    double spring_constant;
    double target_force;
    double min_force[WINCH_MAX_ANCHORS];
    double max_force[WINCH_MAX_ANCHORS];
    double guy_wires[WINCH_MAX_ANCHORS];
    double distances_origin[WINCH_MAX_ANCHORS];
    double relaxed_origin[WINCH_MAX_ANCHORS];
    double cache_distances[WINCH_MAX_ANCHORS];
    double cache_flex[WINCH_MAX_ANCHORS];
    struct move *cache_move;
    double cache_time;
    int cache_valid;
};

struct winch_stepper {
    struct stepper_kinematics sk;
    struct winch_flex *wf;
    int index;
};

static inline double
clampd(double v, double min_v, double max_v)
{
    if (max_v > min_v) {
        if (v < min_v)
            return min_v;
        if (v > max_v)
            return max_v;
    }
    return v;
}

static inline double
safe_ratio(double num, double den)
{
    double abs_den = fabs(den);
    if (abs_den < EPSILON)
        return (num >= 0.) ? 1e9 : -1e9;
    return num / den;
}

static inline double
hypot3(double dx, double dy, double dz)
{
    return sqrt(dx*dx + dy*dy + dz*dz);
}

static int
gauss_jordan_3x5(double M[3][5])
{
    for (int col = 0; col < 3; ++col) {
        int pivot = col;
        double max_val = fabs(M[pivot][col]);
        for (int row = col + 1; row < 3; ++row) {
            double val = fabs(M[row][col]);
            if (val > max_val) {
                max_val = val;
                pivot = row;
            }
        }
        if (max_val < EPSILON)
            return 0;
        if (pivot != col) {
            for (int c = col; c < 5; ++c) {
                double tmp = M[col][c];
                M[col][c] = M[pivot][c];
                M[pivot][c] = tmp;
            }
        }
        double inv = 1.0 / M[col][col];
        for (int c = col; c < 5; ++c)
            M[col][c] *= inv;
        for (int row = 0; row < 3; ++row) {
            if (row == col)
                continue;
            double factor = M[row][col];
            if (fabs(factor) < EPSILON)
                continue;
            for (int c = col; c < 5; ++c)
                M[row][c] -= factor * M[col][c];
        }
    }
    return 1;
}

static void
static_forces_tetrahedron(struct winch_flex *wf, const struct coord *pos,
                          double *F)
{
    if (!wf->flex_enabled || wf->num_anchors != 4)
        return;
    double norm[4] = {0.};
    double dirs[4][3] = {{0.}};
    for (int i = 0; i < 3; ++i) {
        double dx = wf->anchors[i].x - pos->x;
        double dy = wf->anchors[i].y - pos->y;
        double dz = wf->anchors[i].z - pos->z;
        norm[i] = hypot3(dx, dy, dz);
        if (norm[i] < EPSILON)
            continue;
        dirs[i][0] = dx / norm[i];
        dirs[i][1] = dy / norm[i];
        dirs[i][2] = dz / norm[i];
    }
    double dx = wf->anchors[3].x - pos->x;
    double dy = wf->anchors[3].y - pos->y;
    double dz = wf->anchors[3].z - pos->z;
    double normD = hypot3(dx, dy, dz);
    double dirD[3] = {0., 0., 0.};
    if (normD >= EPSILON) {
        dirD[0] = dx / normD;
        dirD[1] = dy / normD;
        dirD[2] = dz / normD;
    }

    double mg = wf->mover_weight * G_ACCEL;
    if (mg < EPSILON)
        return;

    double D_mg = 0., D_pre = 0.;
    if (wf->anchors[3].z > pos->z && fabs(dirD[2]) >= EPSILON) {
        D_mg = mg / dirD[2];
        D_pre = wf->target_force;
    }

    double M[3][5] = {{0.}};
    for (int axis = 0; axis < 3; ++axis) {
        M[axis][0] = dirs[0][axis];
        M[axis][1] = dirs[1][axis];
        M[axis][2] = dirs[2][axis];
        M[axis][3] = -D_mg * dirD[axis];
        M[axis][4] = -D_pre * dirD[axis];
    }
    M[2][3] += mg;

    if (!gauss_jordan_3x5(M))
        return;

    double A_mg = M[0][3], B_mg = M[1][3], C_mg = M[2][3];
    double A_pre = M[0][4], B_pre = M[1][4], C_pre = M[2][4];

    double lower = 0.;
    lower = fmax(lower, fabs(safe_ratio(wf->target_force - A_mg, A_pre)));
    lower = fmax(lower, fabs(safe_ratio(wf->target_force - B_mg, B_pre)));
    lower = fmax(lower, fabs(safe_ratio(wf->target_force - C_mg, C_pre)));

    double upper = fabs(safe_ratio(wf->max_force[3] - D_mg, D_pre));
    upper = fmin(upper, fabs(safe_ratio(wf->max_force[0] - A_mg, A_pre)));
    upper = fmin(upper, fabs(safe_ratio(wf->max_force[1] - B_mg, B_pre)));
    upper = fmin(upper, fabs(safe_ratio(wf->max_force[2] - C_mg, C_pre)));

    double preFac = fmin(lower, upper);
    if (!isfinite(preFac))
        preFac = 0.;

    double total[4];
    total[0] = A_mg + preFac * A_pre;
    total[1] = B_mg + preFac * B_pre;
    total[2] = C_mg + preFac * C_pre;
    total[3] = D_mg + preFac * D_pre;

    for (int i = 0; i < 4; ++i)
        F[i] = clampd(total[i], wf->min_force[i], wf->max_force[i]);
}

static void
static_forces_quadrilateral(struct winch_flex *wf, const struct coord *pos,
                            double *F)
{
    if (!wf->flex_enabled || wf->num_anchors != 5)
        return;

    double mg = wf->mover_weight * G_ACCEL;
    if (mg < EPSILON)
        return;

    double norm[5] = {0.};
    double dirs[5][3] = {{0.}};
    for (int i = 0; i < 5; ++i) {
        double dx = wf->anchors[i].x - pos->x;
        double dy = wf->anchors[i].y - pos->y;
        double dz = wf->anchors[i].z - pos->z;
        norm[i] = hypot3(dx, dy, dz);
        if (norm[i] < EPSILON)
            continue;
        dirs[i][0] = dx / norm[i];
        dirs[i][1] = dy / norm[i];
        dirs[i][2] = dz / norm[i];
    }

    double top_mg = 0.;
    double top_pre = wf->target_force;
    if (wf->anchors[4].z > pos->z && fabs(dirs[4][2]) >= EPSILON)
        top_mg = mg / dirs[4][2];
    else
        top_pre = 0.;

    double matrices[4][3][5];
    memset(matrices, 0, sizeof(matrices));
    for (int i = 0; i < 4; ++i) {
        for (int axis = 0; axis < 3; ++axis) {
            for (int skip = 0; skip < 4; ++skip) {
                if (skip == i)
                    continue;
                int col = (i > skip) ? i - 1 : i;
                matrices[skip][axis][col] = dirs[i][axis];
            }
        }
    }
    for (int axis = 0; axis < 3; ++axis) {
        double top_dir = dirs[4][axis];
        for (int k = 0; k < 4; ++k) {
            matrices[k][axis][3] = -top_mg * top_dir;
            matrices[k][axis][4] = -top_pre * top_dir;
        }
    }
    for (int k = 0; k < 4; ++k)
        matrices[k][2][3] += mg;

    for (int k = 0; k < 4; ++k) {
        if (!gauss_jordan_3x5(matrices[k]))
            return;
    }

    double norm_ABCD[4];
    for (int k = 0; k < 4; ++k) {
        double sum = 0.;
        for (int axis = 0; axis < 3; ++axis) {
            double v = matrices[k][axis][4];
            sum += v * v;
        }
        norm_ABCD[k] = sqrt(sum);
        if (norm_ABCD[k] < EPSILON)
            norm_ABCD[k] = 1.;
    }

    double p[4] = {0., 0., 0., 0.};
    double m_vec[4] = {0., 0., 0., 0.};
    for (int axis = 0; axis < 3; ++axis) {
        for (int j = 0; j < 4; ++j) {
            double pt_weight = (wf->target_force >= EPSILON)
                ? wf->target_force / norm_ABCD[j] : 0.;
            double mg_weight = 0.25;
            int s = (j <= axis) ? axis + 1 : axis;
            p[s] += matrices[j][axis][4] * pt_weight;
            m_vec[s] += matrices[j][axis][3] * mg_weight;
        }
    }

    double lower = 0.;
    for (int i = 0; i < 4; ++i)
        lower = fmax(lower, fabs(safe_ratio(wf->target_force - m_vec[i], p[i])));

    double upper = fabs(safe_ratio(wf->max_force[4] - top_mg, top_pre));
    for (int i = 0; i < 4; ++i)
        upper = fmin(upper, fabs(safe_ratio(wf->max_force[i] - m_vec[i], p[i])));

    double preFac = fmin(lower, upper);
    if (!isfinite(preFac))
        preFac = 0.;

    double total[5];
    for (int i = 0; i < 4; ++i)
        total[i] = m_vec[i] + preFac * p[i];
    total[4] = top_mg + preFac * top_pre;

    for (int i = 0; i < 5; ++i)
        F[i] = clampd(total[i], wf->min_force[i], wf->max_force[i]);
}

static void
static_forces(struct winch_flex *wf, const struct coord *pos, double *F)
{
    memset(F, 0, sizeof(double) * wf->num_anchors);
    if (!wf->flex_enabled)
        return;
    if (wf->num_anchors == 4)
        static_forces_tetrahedron(wf, pos, F);
    else if (wf->num_anchors == 5)
        static_forces_quadrilateral(wf, pos, F);
}

static void
compute_flex(struct winch_flex *wf, double x, double y, double z,
             double *distances, double *flex)
{
    int num = wf->num_anchors;
    struct coord pos = { x, y, z };
    for (int i = 0; i < num; ++i) {
        double dx = wf->anchors[i].x - x;
        double dy = wf->anchors[i].y - y;
        double dz = wf->anchors[i].z - z;
        distances[i] = hypot3(dx, dy, dz);
    }
    if (!wf->flex_enabled) {
        for (int i = 0; i < num; ++i)
            flex[i] = 0.;
        return;
    }
    double forces[WINCH_MAX_ANCHORS];
    static_forces(wf, &pos, forces);
    for (int i = 0; i < num; ++i) {
        double spring_length = distances[i] + wf->guy_wires[i];
        if (spring_length < EPSILON)
            spring_length = EPSILON;
        double spring_k = wf->spring_constant / spring_length;
        double relaxed = distances[i] - forces[i] / spring_k;
        double line_pos = relaxed - wf->relaxed_origin[i];
        double delta = distances[i] - wf->distances_origin[i];
        flex[i] = line_pos - delta;
    }
}

static void
update_cache(struct winch_flex *wf, struct move *m, double move_time)
{
    if (!wf || wf->num_anchors <= 0)
        return;
    if (wf->cache_valid && wf->cache_move == m && wf->cache_time == move_time)
        return;
    struct coord pos = move_get_coord(m, move_time);
    compute_flex(wf, pos.x, pos.y, pos.z, wf->cache_distances, wf->cache_flex);
    wf->cache_move = m;
    wf->cache_time = move_time;
    wf->cache_valid = 1;
}

static double
calc_position_common(struct winch_stepper *ws, struct move *m, double move_time)
{
    struct winch_flex *wf = ws->wf;
    if (!wf || ws->index >= wf->num_anchors)
        return 0.;
    update_cache(wf, m, move_time);
    return wf->cache_distances[ws->index] - wf->cache_flex[ws->index];
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

    if (!wf->guy_wires_explicit && num >= 2) {
        int top = num - 1;
        for (int i = 0; i < num; ++i) {
            if (i == top) {
                wf->guy_wires[i] = 0.;
                continue;
            }
            double dx = wf->anchors[i].x - wf->anchors[top].x;
            double dy = wf->anchors[i].y - wf->anchors[top].y;
            double dz = wf->anchors[i].z - wf->anchors[top].z;
            wf->guy_wires[i] = hypot3(dx, dy, dz);
        }
    }

    for (int i = 0; i < num; ++i) {
        double dx = wf->anchors[i].x;
        double dy = wf->anchors[i].y;
        double dz = wf->anchors[i].z;
        wf->distances_origin[i] = hypot3(dx, dy, dz);
    }

    if (!wf->flex_enabled) {
        for (int i = 0; i < num; ++i)
            wf->relaxed_origin[i] = wf->distances_origin[i];
        return;
    }

    struct coord origin = {0., 0., 0.};
    double forces[WINCH_MAX_ANCHORS];
    static_forces(wf, &origin, forces);

    for (int i = 0; i < num; ++i) {
        double spring_length = wf->distances_origin[i] + wf->guy_wires[i];
        if (spring_length < EPSILON)
            spring_length = EPSILON;
        double spring_k = wf->spring_constant / spring_length;
        double relaxed = wf->distances_origin[i] - forces[i] / spring_k;
        wf->relaxed_origin[i] = relaxed;
    }
}

void __visible
winch_flex_configure(struct winch_flex *wf, int num_anchors,
                     const double *anchors, double mover_weight,
                     double spring_constant, double target_force,
                     const double *min_force, const double *max_force,
                     const double *guy_wires, int guy_wires_valid)
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
    wf->target_force = target_force;
    wf->guy_wires_explicit = guy_wires_valid;

    for (int i = 0; i < num_anchors; ++i) {
        wf->anchors[i].x = anchors[i * 3];
        wf->anchors[i].y = anchors[i * 3 + 1];
        wf->anchors[i].z = anchors[i * 3 + 2];
        wf->min_force[i] = min_force ? min_force[i] : 0.;
        wf->max_force[i] = max_force ? max_force[i] : 1e6;
        if (guy_wires_valid && guy_wires)
            wf->guy_wires[i] = guy_wires[i] < 0. ? 0. : guy_wires[i];
        else
            wf->guy_wires[i] = 0.;
    }
    for (int i = num_anchors; i < WINCH_MAX_ANCHORS; ++i) {
        wf->anchors[i].x = wf->anchors[i].y = wf->anchors[i].z = 0.;
        wf->min_force[i] = 0.;
        wf->max_force[i] = 1e6;
        wf->guy_wires[i] = 0.;
    }

    wf->flex_enabled = (num_anchors >= 4
                        && mover_weight > 0.
                        && spring_constant > 0.);
    wf->cache_valid = 0;
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

struct stepper_kinematics * __visible
winch_stepper_alloc(struct winch_flex *wf, int index)
{
    struct winch_stepper *ws = malloc(sizeof(*ws));
    if (!ws)
        return NULL;
    memset(ws, 0, sizeof(*ws));
    ws->wf = wf;
    ws->index = index;
    ws->sk.calc_position_cb = winch_stepper_calc_position;
    ws->sk.active_flags = AF_X | AF_Y | AF_Z;
    return &ws->sk;
}
