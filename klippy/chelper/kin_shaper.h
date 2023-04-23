#ifndef __KIN_SHAPER_H
#define __KIN_SHAPER_H

struct shaper_pulses {
    int num_pulses;
    struct {
        double t, a;
    } pulses[5];
};

struct move;

int init_shaper(int n, double a[], double t[], struct shaper_pulses *sp);
double shaper_calc_position(const struct move *m, int axis, double move_time
                            , const struct shaper_pulses *sp);

#endif  // kin_shaper.h
