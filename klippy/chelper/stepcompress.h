#ifndef STEPCOMPRESS_H
#define STEPCOMPRESS_H

#include <stdint.h> // uint32_t

#define ERROR_RET -989898989

struct pull_history_steps {
    uint64_t first_clock, last_clock;
    int64_t start_position;
    int step_count, interval, add;
};

struct stepcompress *stepcompress_alloc(uint32_t oid);
void stepcompress_fill(struct stepcompress *sc, uint32_t max_error
                       , int32_t queue_step_msgtag
                       , int32_t set_next_step_dir_msgtag);
void stepcompress_set_invert_sdir(struct stepcompress *sc
                                  , uint32_t invert_sdir);
void stepcompress_free(struct stepcompress *sc);
uint32_t stepcompress_get_oid(struct stepcompress *sc);
int stepcompress_get_step_dir(struct stepcompress *sc);
int stepcompress_append(struct stepcompress *sc, int sdir
                        , double print_time, double step_time);
int stepcompress_commit(struct stepcompress *sc);
int stepcompress_reset(struct stepcompress *sc, uint64_t last_step_clock);
int stepcompress_set_last_position(struct stepcompress *sc, uint64_t clock
                                   , int64_t last_position);
int64_t stepcompress_find_past_position(struct stepcompress *sc
                                        , uint64_t clock);
int stepcompress_queue_msg(struct stepcompress *sc, uint32_t *data, int len);
int stepcompress_queue_mq_msg(struct stepcompress *sc, uint64_t req_clock
                              , uint32_t *data, int len);
int stepcompress_extract_old(struct stepcompress *sc
                             , struct pull_history_steps *p, int max
                             , uint64_t start_clock, uint64_t end_clock);

struct serialqueue;
struct steppersync *steppersync_alloc(
    struct serialqueue *sq, struct stepcompress **sc_list, int sc_num
    , int move_num);
void steppersync_free(struct steppersync *ss);
void steppersync_set_time(struct steppersync *ss, double time_offset
                          , double mcu_freq);
int steppersync_flush(struct steppersync *ss, uint64_t move_clock
                      , uint64_t clear_history_clock);

#endif // stepcompress.h
