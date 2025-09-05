#ifndef STEPPERSYNC_H
#define STEPPERSYNC_H

#include <stdint.h> // uint64_t

struct serialqueue;
struct steppersync *steppersync_alloc(
    struct serialqueue *sq, struct stepcompress **sc_list, int sc_num
    , int move_num);
void steppersync_free(struct steppersync *ss);
void steppersync_set_time(struct steppersync *ss, double time_offset
                          , double mcu_freq);
void steppersync_history_expire(struct steppersync *ss, uint64_t end_clock);
void steppersync_start_gen_steps(struct steppersync *ss, double gen_steps_time
                                 , uint64_t flush_clock);
int32_t steppersync_finalize_gen_steps(struct steppersync *ss
                                       , uint64_t flush_clock);

#endif // steppersync.h
