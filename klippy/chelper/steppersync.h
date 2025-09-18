#ifndef STEPPERSYNC_H
#define STEPPERSYNC_H

#include <stdint.h> // uint64_t

struct steppersync;
void steppersync_set_time(struct steppersync *ss, double time_offset
                          , double mcu_freq);

struct steppersyncmgr *steppersyncmgr_alloc(void);
void steppersyncmgr_free(struct steppersyncmgr *ssm);
struct serialqueue;
struct steppersync *steppersyncmgr_alloc_steppersync(
    struct steppersyncmgr *ssm, struct serialqueue *sq
    , struct stepcompress **sc_list, int sc_num, int move_num);
int32_t steppersyncmgr_gen_steps(struct steppersyncmgr *ssm, double flush_time
                                 , double gen_steps_time
                                 , double clear_history_time);

#endif // steppersync.h
