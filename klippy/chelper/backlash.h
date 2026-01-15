#ifndef BACKLASH_H
#define BACKLASH_H

struct move;

struct backlash_compensation {
    double axis_lag[3];
    double smooth_time, inv_smooth_time;
};

double calc_backlash_compensation(struct backlash_compensation *bc
                                  , struct move *m, int axis, double move_time);

#endif // backlash.h
