// CLI wrapper around klipper's winch_forward_solve for dataset evaluation.
// Matches the input format used by solver_quadratic: optional CFG line
// followed by rows of: N use_flex motor_deg... anchor_xyz...

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../klippy/chelper/kin_winch.c"

// Stub out move_get_coord so kin_winch.c links; not used by this tool.
struct coord
move_get_coord(struct move *m, double move_time)
{
    struct coord c = { 0.0, 0.0, 0.0 };
    return c;
}

struct cfg_state {
    size_t count;
    double spool_buildup;
    double spring_k_per_unit_length;
    double mover_weight;
    double spool_gear;
    double motor_gear;
    double steps_per_rev;
    int use_flex;
    int ignore_gravity;
    int ignore_pretension;
    double lambda_reg;
    double tol;
    int max_iters_target;
    double g;
    double spool_r[WINCH_MAX_ANCHORS];
    double mech_adv[WINCH_MAX_ANCHORS];
    double lines_per_spool[WINCH_MAX_ANCHORS];
    double min_force[WINCH_MAX_ANCHORS];
    double max_force[WINCH_MAX_ANCHORS];
    double guy_wires[WINCH_MAX_ANCHORS];
    int has_cfg;
};

static void
init_cfg(struct cfg_state *cfg)
{
    memset(cfg, 0, sizeof(*cfg));
    cfg->count = 0;
    cfg->spool_buildup = 0.043003;
    cfg->spring_k_per_unit_length = 20000.0;
    cfg->mover_weight = 2.0;
    cfg->spool_gear = 255.0;
    cfg->motor_gear = 20.0;
    cfg->steps_per_rev = 360.0;
    cfg->use_flex = 1;
    cfg->ignore_gravity = 0;
    cfg->ignore_pretension = 0;
    cfg->lambda_reg = 1e-3;
    cfg->tol = 1e-3;
    cfg->max_iters_target = 100;
    cfg->g = 9.81;
    for (int i = 0; i < WINCH_MAX_ANCHORS; ++i) {
        cfg->spool_r[i] = 75.0;
        cfg->mech_adv[i] = 2.0;
        cfg->lines_per_spool[i] = 1.0;
        cfg->min_force[i] = 3.0;
        cfg->max_force[i] = 120.0;
        cfg->guy_wires[i] = 0.0;
    }
    cfg->has_cfg = 0;
}

static void
broadcast_tail(double *arr, size_t count)
{
    if (!count || count >= WINCH_MAX_ANCHORS)
        return;
    double last = arr[count - 1];
    for (size_t i = count; i < WINCH_MAX_ANCHORS; ++i)
        arr[i] = last;
}

static int
parse_cfg_line(const char *line, struct cfg_state *cfg)
{
    init_cfg(cfg);
    char tag[8] = {0};
    double steps_per_rev = 0.;
    int use_flex = 1, ignore_grav = 0, ignore_pre = 0;
    int max_iters = 0;
    int nread = sscanf(line,
                       "%7s %zu %lf %lf %lf %lf %lf %lf %d %d %d %lf %lf %d %lf",
                       tag, &cfg->count, &cfg->spool_buildup,
                       &cfg->spring_k_per_unit_length, &cfg->mover_weight,
                       &cfg->spool_gear, &cfg->motor_gear, &steps_per_rev,
                       &use_flex, &ignore_grav, &ignore_pre, &cfg->lambda_reg,
                       &cfg->tol, &max_iters, &cfg->g);
    if (nread != 15 || strcmp(tag, "CFG") != 0)
        return 0;

    cfg->steps_per_rev = steps_per_rev;
    cfg->use_flex = use_flex;
    cfg->ignore_gravity = ignore_grav;
    cfg->ignore_pretension = ignore_pre;
    cfg->max_iters_target = max_iters;
    size_t limit = cfg->count > WINCH_MAX_ANCHORS ? WINCH_MAX_ANCHORS : cfg->count;
    const char *p = line;
    for (int i = 0; i < 15; ++i) {
        p = strchr(p, ' ');
        if (!p)
            return 0;
        while (*p == ' ')
            p++;
    }
    for (size_t i = 0; i < limit; ++i) {
        cfg->spool_r[i] = strtod(p, (char **)&p);
    }
    for (size_t i = 0; i < limit; ++i) {
        cfg->mech_adv[i] = strtod(p, (char **)&p);
    }
    for (size_t i = 0; i < limit; ++i) {
        cfg->lines_per_spool[i] = strtod(p, (char **)&p);
    }
    for (size_t i = 0; i < limit; ++i) {
        cfg->min_force[i] = strtod(p, (char **)&p);
    }
    for (size_t i = 0; i < limit; ++i) {
        cfg->max_force[i] = strtod(p, (char **)&p);
    }
    for (size_t i = 0; i < limit; ++i) {
        cfg->guy_wires[i] = strtod(p, (char **)&p);
    }
    broadcast_tail(cfg->spool_r, limit);
    broadcast_tail(cfg->mech_adv, limit);
    broadcast_tail(cfg->lines_per_spool, limit);
    broadcast_tail(cfg->min_force, limit);
    broadcast_tail(cfg->max_force, limit);
    broadcast_tail(cfg->guy_wires, limit);
    cfg->has_cfg = 1;
    return 1;
}

static double
rotation_distance_for_axis(const struct cfg_state *cfg, size_t idx)
{
    double r = cfg->spool_r[idx];
    double ma = cfg->mech_adv[idx];
    if (ma == 0.)
        ma = 1.;
    double gear = cfg->spool_gear / cfg->motor_gear;
    return (2.0 * M_PI * r) / (gear * ma);
}

static int
solve_sample(const struct cfg_state *cfg, size_t num, int use_flex,
             const double *motor_deg, const double *anchors, double *pos_out,
             int *iters_out, double *cost_out, double *runtime_ms_out)
{
    struct winch_flex *wf = winch_flex_alloc();
    if (!wf)
        return 0;

    double min_force[WINCH_MAX_ANCHORS], max_force[WINCH_MAX_ANCHORS];
    double guy[WINCH_MAX_ANCHORS];
    int mech_adv[WINCH_MAX_ANCHORS];
    for (size_t i = 0; i < num; ++i) {
        min_force[i] = cfg->min_force[i];
        max_force[i] = cfg->max_force[i];
        guy[i] = cfg->guy_wires[i];
        mech_adv[i] = (int)(cfg->mech_adv[i] + 0.5);
        if (mech_adv[i] <= 0)
            mech_adv[i] = 1;
    }
    winch_flex_configure(
        wf, (int)num, anchors, cfg->spool_buildup, cfg->mover_weight,
        cfg->spring_k_per_unit_length, min_force, max_force, guy,
        WINCH_FORCE_ALGO_QP, cfg->ignore_gravity, cfg->ignore_pretension,
        mech_adv);
    winch_flex_set_enabled(wf, use_flex ? 1 : 0);

    double motor_mm[WINCH_MAX_ANCHORS];
    for (size_t i = 0; i < num; ++i) {
        double rd = rotation_distance_for_axis(cfg, i);
        winch_flex_set_spool_params(wf, (int)i, rd, cfg->steps_per_rev);
        motor_mm[i] = motor_deg[i] / cfg->steps_per_rev * rd;
    }

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    int iters = 0;
    double cost = 0.0;
    int ok = winch_forward_solve(
        wf, motor_mm, NULL, 1e-3, 1e-3, 3, 30, pos_out, &cost, &iters);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double dt_ms = (t1.tv_sec - t0.tv_sec) * 1000.0
        + (t1.tv_nsec - t0.tv_nsec) / 1e6;

    winch_flex_free(wf);
    if (iters_out)
        *iters_out = iters;
    if (cost_out)
        *cost_out = cost;
    if (runtime_ms_out)
        *runtime_ms_out = dt_ms;
    return ok;
}

int
main(void)
{
    char buf[4096];
    struct cfg_state cfg;
    init_cfg(&cfg);

    while (fgets(buf, sizeof(buf), stdin)) {
        if (buf[0] == '\0' || buf[0] == '\n')
            continue;
        if (strncmp(buf, "CFG", 3) == 0) {
            parse_cfg_line(buf, &cfg);
            continue;
        }

        size_t num = 0;
        int use_flex = 1;
        const char *p = buf;
        if (sscanf(p, "%zu %d", &num, &use_flex) != 2 || num == 0
            || num > WINCH_MAX_ANCHORS) {
            printf("fail 0 0 0 0 0 0\n");
            continue;
        }

        // advance past num/use_flex
        for (int i = 0; i < 2; ++i) {
            p = strchr(p, ' ');
            if (!p)
                break;
            while (*p == ' ')
                p++;
        }
        double motor_deg[WINCH_MAX_ANCHORS];
        for (size_t i = 0; i < num; ++i) {
            motor_deg[i] = strtod(p, (char **)&p);
        }
        double anchors[WINCH_MAX_ANCHORS * 3];
        for (size_t i = 0; i < num * 3; ++i) {
            anchors[i] = strtod(p, (char **)&p);
        }

        double pos[3] = {0., 0., 0.};
        int iters = 0;
        double cost = 0., ms = 0.;
        int ok = solve_sample(&cfg, num, use_flex, motor_deg, anchors, pos,
                              &iters, &cost, &ms);
        printf("%s %.9g %.9g %.9g %d %.9g %.9g\n",
               ok ? "ok" : "fail", pos[0], pos[1], pos[2], iters, cost, ms);
    }
    return 0;
}
