#ifndef STEPPERSYNC_H
#define STEPPERSYNC_H

#include <stdint.h> // uint64_t

struct syncemitter;
struct stepcompress *syncemitter_get_stepcompress(struct syncemitter *se);
void syncemitter_set_stepper_kinematics(struct syncemitter *se
                                        , struct stepper_kinematics *sk);
struct stepper_kinematics *syncemitter_get_stepper_kinematics(
    struct syncemitter *se);
void syncemitter_queue_msg(struct syncemitter *se, uint64_t req_clock
                           , uint32_t *data, int len);

struct steppersync;
struct syncemitter *steppersync_alloc_syncemitter(
    struct steppersync *ss, char name[16], int alloc_stepcompress);
void steppersync_setup_movequeue(struct steppersync *ss, struct serialqueue *sq
                                 , int move_num);
void steppersync_set_time(struct steppersync *ss, double time_offset
                          , double mcu_freq);

struct steppersyncmgr *steppersyncmgr_alloc(void);
void steppersyncmgr_free(struct steppersyncmgr *ssm);
struct serialqueue;
struct steppersync *steppersyncmgr_alloc_steppersync(
    struct steppersyncmgr *ssm);
int32_t steppersyncmgr_gen_steps(struct steppersyncmgr *ssm, double flush_time
                                 , double gen_steps_time
                                 , double clear_history_time);

#endif // steppersync.h
