#ifndef BACKLASH_H
#define BACKLASH_H

struct move;

struct backlash_compensation {
    double axis_lag[3];
    double half_smooth_time, inv_half_smooth_time2;
};

double calc_backlash_compensation(struct backlash_compensation *bc
                                  , struct move *m, int axis, double move_time);

#endif // backlash.h
