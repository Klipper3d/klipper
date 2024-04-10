#ifndef STEPCOMPRESS_H
#define STEPCOMPRESS_H

#include <stdint.h> // uint32_t

#define ERROR_RET -989898989

struct stepcompress *stepcompress_alloc(uint32_t oid);
void stepcompress_fill(struct stepcompress *sc, uint32_t max_error
                       , uint32_t invert_sdir, uint32_t queue_step_msgid
                       , uint32_t set_next_step_dir_msgid);
void stepcompress_free(struct stepcompress *sc);
int stepcompress_reset(struct stepcompress *sc, uint64_t last_step_clock);
int stepcompress_queue_msg(struct stepcompress *sc, uint32_t *data, int len);
double stepcompress_get_mcu_freq(struct stepcompress *sc);
uint32_t stepcompress_get_oid(struct stepcompress *sc);
int stepcompress_get_step_dir(struct stepcompress *sc);

struct queue_append {
    struct stepcompress *sc;
    uint32_t *qnext, *qend, last_step_clock_32;
    double clock_offset;
};
struct queue_append queue_append_start(
    struct stepcompress *sc, double print_time, double adjust);
void queue_append_finish(struct queue_append qa);
int queue_append(struct queue_append *qa, double step_clock);
int queue_append_set_next_step_dir(struct queue_append *qa, int sdir);

struct serialqueue;
struct steppersync *steppersync_alloc(
    struct serialqueue *sq, struct stepcompress **sc_list, int sc_num
    , int move_num);
void steppersync_free(struct steppersync *ss);
void steppersync_set_time(struct steppersync *ss, double time_offset
                          , double mcu_freq);
int steppersync_flush(struct steppersync *ss, uint64_t move_clock);

#endif // stepcompress.h
