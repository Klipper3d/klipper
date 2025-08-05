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
int steppersync_flush(struct steppersync *ss, uint64_t move_clock);

#endif // steppersync.h
