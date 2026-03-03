// Stepper step transmit synchronization
//
// Copyright (C) 2016-2025  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// The steppersync object is used to synchronize the output of mcu
// step commands.  The mcu can only queue a limited number of step
// commands - this code tracks when items on the mcu step queue become
// free so that new commands can be transmitted.  It also ensures the
// mcu step queue is ordered between steppers so that no stepper
// starves the other steppers of space in the mcu step queue.

#include <pthread.h> // pthread_mutex_lock
#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "pyhelper.h" // set_thread_name
#include "itersolve.h" // itersolve_generate_steps
#include "serialqueue.h" // struct queue_message
#include "stepcompress.h" // stepcompress_flush
#include "steppersync.h" // steppersync_alloc
#include "trapq.h" // trapq_check_sentinels


/****************************************************************
 * SyncEmitter - message generation for each stepper
 ****************************************************************/

struct syncemitter {
    // List node for storage in steppersync list
    struct list_node ss_node;
    // Transmit message queue
    struct list_head msg_queue;
    // Thread for step generation
    struct stepcompress *sc;
    struct stepper_kinematics *sk;
    char name[16];
    pthread_t tid;
    pthread_mutex_t lock; // protects variables below
    pthread_cond_t cond;
    int have_work;
    double bg_gen_steps_time;
    uint64_t bg_flush_clock, bg_clear_history_clock;
    int32_t bg_result;
};

// Return this emitters 'struct stepcompress' (or NULL if not allocated)
struct stepcompress * __visible
syncemitter_get_stepcompress(struct syncemitter *se)
{
    return se->sc;
}

// Store a reference to stepper_kinematics
void __visible
syncemitter_set_stepper_kinematics(struct syncemitter *se
                                   , struct stepper_kinematics *sk)
{
    se->sk = sk;
}

// Report current stepper_kinematics
struct stepper_kinematics * __visible
syncemitter_get_stepper_kinematics(struct syncemitter *se)
{
    return se->sk;
}

// Queue an mcu command that will consume space in the mcu move queue
void __visible
syncemitter_queue_msg(struct syncemitter *se, uint64_t req_clock
                      , uint32_t *data, int len)
{
    struct queue_message *qm = message_alloc_and_encode(data, len);
    qm->min_clock = qm->req_clock = req_clock;
    list_add_tail(&qm->node, &se->msg_queue);
}

// Generate steps (via itersolve) and flush
static int32_t
se_generate_steps(struct syncemitter *se)
{
    if (!se->sc || !se->sk)
        return 0;
    double gen_steps_time = se->bg_gen_steps_time;
    uint64_t flush_clock = se->bg_flush_clock;
    uint64_t clear_history_clock = se->bg_clear_history_clock;
    // Generate steps
    int32_t ret = itersolve_generate_steps(se->sk, se->sc, gen_steps_time);
    if (ret)
        return ret;
    // Flush steps
    ret = stepcompress_flush(se->sc, flush_clock);
    if (ret)
        return ret;
    // Clear history
    stepcompress_history_expire(se->sc, clear_history_clock);
    return 0;
}

// Main background thread for generating steps
static void *
se_background_thread(void *data)
{
    struct syncemitter *se = data;
    set_thread_name(se->name);

    pthread_mutex_lock(&se->lock);
    for (;;) {
        if (!se->have_work) {
            pthread_cond_wait(&se->cond, &se->lock);
            continue;
        }
        if (se->have_work < 0)
            // Exit request
            break;

        // Request to generate steps
        se->bg_result = se_generate_steps(se);
        if (se->bg_result)
            errorf("Error in syncemitter '%s' step generation", se->name);
        se->have_work = 0;
        pthread_cond_signal(&se->cond);
    }
    pthread_mutex_unlock(&se->lock);

    return NULL;
}

// Signal background thread to start step generation
static void
se_start_gen_steps(struct syncemitter *se, double gen_steps_time
                   , uint64_t flush_clock, uint64_t clear_history_clock)
{
    if (!se->sc || !se->sk)
        return;
    pthread_mutex_lock(&se->lock);
    while (se->have_work)
        pthread_cond_wait(&se->cond, &se->lock);
    se->bg_gen_steps_time = gen_steps_time;
    se->bg_flush_clock = flush_clock;
    se->bg_clear_history_clock = clear_history_clock;
    se->have_work = 1;
    pthread_mutex_unlock(&se->lock);
    pthread_cond_signal(&se->cond);
}

// Wait for background thread to complete last step generation request
static int32_t
se_finalize_gen_steps(struct syncemitter *se)
{
    if (!se->sc || !se->sk)
        return 0;
    pthread_mutex_lock(&se->lock);
    while (se->have_work)
        pthread_cond_wait(&se->cond, &se->lock);
    int32_t res = se->bg_result;
    pthread_mutex_unlock(&se->lock);
    return res;
}

// Allocate syncemitter and start thread
static struct syncemitter *
syncemitter_alloc(char name[16], int alloc_stepcompress)
{
    struct syncemitter *se = malloc(sizeof(*se));
    memset(se, 0, sizeof(*se));
    list_init(&se->msg_queue);
    strncpy(se->name, name, sizeof(se->name));
    se->name[sizeof(se->name)-1] = '\0';
    if (!alloc_stepcompress)
        return se;
    se->sc = stepcompress_alloc(&se->msg_queue);
    int ret = pthread_mutex_init(&se->lock, NULL);
    if (ret)
        goto fail;
    ret = pthread_cond_init(&se->cond, NULL);
    if (ret)
        goto fail;
    ret = pthread_create(&se->tid, NULL, se_background_thread, se);
    if (ret)
        goto fail;
    return se;
fail:
    report_errno("se alloc", ret);
    return NULL;
}

// Free syncemitter and exit background thread
static void
syncemitter_free(struct syncemitter *se)
{
    if (!se)
        return;
    if (se->sc) {
        pthread_mutex_lock(&se->lock);
        while (se->have_work)
            pthread_cond_wait(&se->cond, &se->lock);
        se->have_work = -1;
        pthread_cond_signal(&se->cond);
        pthread_mutex_unlock(&se->lock);
        int ret = pthread_join(se->tid, NULL);
        if (ret)
            report_errno("se pthread_join", ret);
        stepcompress_free(se->sc);
    }
    message_queue_free(&se->msg_queue);
    free(se);
}


/****************************************************************
 * StepperSync - sort move queue for a micro-controller
 ****************************************************************/

struct steppersync {
    // List node for storage in steppersyncmgr list
    struct list_node ssm_node;
    // Serial port
    struct serialqueue *sq;
    struct command_queue *cq;
    // The syncemitters that generate messages on this mcu
    struct list_head se_list;
    // Convert from time to clock
    struct clock_estimate ce;
    // Storage for list of pending move clocks
    uint64_t *move_clocks;
    int num_move_clocks;
};

// Allocate a new syncemitter instance
struct syncemitter * __visible
steppersync_alloc_syncemitter(struct steppersync *ss, char name[16]
                              , int alloc_stepcompress)
{
    struct syncemitter *se = syncemitter_alloc(name, alloc_stepcompress);
    if (se)
        list_add_tail(&se->ss_node, &ss->se_list);
    return se;
}

// Fill information on mcu move queue
void __visible
steppersync_setup_movequeue(struct steppersync *ss, struct serialqueue *sq
                            , int move_num)
{
    serialqueue_free_commandqueue(ss->cq);
    free(ss->move_clocks);

    ss->sq = sq;
    ss->cq = serialqueue_alloc_commandqueue();

    ss->move_clocks = malloc(sizeof(*ss->move_clocks)*move_num);
    memset(ss->move_clocks, 0, sizeof(*ss->move_clocks)*move_num);
    ss->num_move_clocks = move_num;
}

// Set the conversion rate of 'print_time' to mcu clock
void __visible
steppersync_set_time(struct steppersync *ss, double time_offset
                     , double mcu_freq)
{
    clock_fill(&ss->ce, mcu_freq, time_offset, 0, 0);
    struct syncemitter *se;
    list_for_each_entry(se, &ss->se_list, ss_node) {
        if (se->sc)
            stepcompress_set_time(se->sc, time_offset, mcu_freq);
    }
}

// Implement a binary heap algorithm to track when the next available
// 'struct move' in the mcu will be available
static void
heap_replace(struct steppersync *ss, uint64_t req_clock)
{
    uint64_t *mc = ss->move_clocks;
    int nmc = ss->num_move_clocks, pos = 0;
    for (;;) {
        int child1_pos = 2*pos+1, child2_pos = 2*pos+2;
        uint64_t child2_clock = child2_pos < nmc ? mc[child2_pos] : UINT64_MAX;
        uint64_t child1_clock = child1_pos < nmc ? mc[child1_pos] : UINT64_MAX;
        if (req_clock <= child1_clock && req_clock <= child2_clock) {
            mc[pos] = req_clock;
            break;
        }
        if (child1_clock < child2_clock) {
            mc[pos] = child1_clock;
            pos = child1_pos;
        } else {
            mc[pos] = child2_clock;
            pos = child2_pos;
        }
    }
}

// Find and transmit any scheduled steps prior to the given 'move_clock'
static void
steppersync_flush(struct steppersync *ss, uint64_t move_clock)
{
    // Order commands by the reqclock of each pending command
    struct list_head msgs;
    list_init(&msgs);
    for (;;) {
        // Find message with lowest reqclock
        uint64_t req_clock = MAX_CLOCK;
        struct queue_message *qm = NULL;
        struct syncemitter *se;
        list_for_each_entry(se, &ss->se_list, ss_node) {
            if (!list_empty(&se->msg_queue)) {
                struct queue_message *m = list_first_entry(
                    &se->msg_queue, struct queue_message, node);
                if (m->req_clock < req_clock) {
                    qm = m;
                    req_clock = m->req_clock;
                }
            }
        }
        if (!qm || (qm->min_clock && req_clock > move_clock))
            break;

        uint64_t next_avail = ss->move_clocks[0];
        if (qm->min_clock)
            // The qm->min_clock field is overloaded to indicate that
            // the command uses the 'move queue' and to store the time
            // that move queue item becomes available.
            heap_replace(ss, qm->min_clock);
        // Reset the min_clock to its normal meaning (minimum transmit time)
        qm->min_clock = next_avail;

        // Batch this command
        list_del(&qm->node);
        list_add_tail(&qm->node, &msgs);
    }

    // Transmit commands
    if (!list_empty(&msgs))
        serialqueue_send_batch(ss->sq, ss->cq, &msgs);
}


/****************************************************************
 * StepperSyncMgr - manage a list of steppersync
 ****************************************************************/

struct steppersyncmgr {
    struct list_head ss_list;
};

// Allocate a new 'steppersyncmgr' object
struct steppersyncmgr * __visible
steppersyncmgr_alloc(void)
{
    struct steppersyncmgr *ssm = malloc(sizeof(*ssm));
    memset(ssm, 0, sizeof(*ssm));
    list_init(&ssm->ss_list);
    return ssm;
}

// Free memory associated with a 'steppersync' object
void __visible
steppersyncmgr_free(struct steppersyncmgr *ssm)
{
    if (!ssm)
        return;
    while (!list_empty(&ssm->ss_list)) {
        struct steppersync *ss = list_first_entry(
            &ssm->ss_list, struct steppersync, ssm_node);
        list_del(&ss->ssm_node);
        free(ss->move_clocks);
        serialqueue_free_commandqueue(ss->cq);
        while (!list_empty(&ss->se_list)) {
            struct syncemitter *se = list_first_entry(
                &ss->se_list, struct syncemitter, ss_node);
            list_del(&se->ss_node);
            syncemitter_free(se);
        }
        free(ss);
    }
    free(ssm);
}

// Allocate a new 'steppersync' object
struct steppersync * __visible
steppersyncmgr_alloc_steppersync(struct steppersyncmgr *ssm)
{
    struct steppersync *ss = malloc(sizeof(*ss));
    memset(ss, 0, sizeof(*ss));
    list_init(&ss->se_list);
    list_add_tail(&ss->ssm_node, &ssm->ss_list);
    return ss;
}

// Generate and flush steps
int32_t __visible
steppersyncmgr_gen_steps(struct steppersyncmgr *ssm, double flush_time
                         , double gen_steps_time, double clear_history_time)
{
    struct steppersync *ss;
    // Prepare trapqs for step generation
    list_for_each_entry(ss, &ssm->ss_list, ssm_node) {
        struct syncemitter *se;
        list_for_each_entry(se, &ss->se_list, ss_node) {
            if (!se->sc || !se->sk)
                continue;
            struct trapq *tq = itersolve_get_trapq(se->sk);
            if (tq)
                trapq_check_sentinels(tq);
        }
    }
    // Start step generation threads
    list_for_each_entry(ss, &ssm->ss_list, ssm_node) {
        uint64_t flush_clock = clock_from_time(&ss->ce, flush_time);
        uint64_t clear_clock = clock_from_time(&ss->ce, clear_history_time);
        struct syncemitter *se;
        list_for_each_entry(se, &ss->se_list, ss_node) {
            se_start_gen_steps(se, gen_steps_time, flush_clock, clear_clock);
        }
    }
    // Wait for step generation threads to complete
    int32_t res = 0;
    list_for_each_entry(ss, &ssm->ss_list, ssm_node) {
        struct syncemitter *se;
        list_for_each_entry(se, &ss->se_list, ss_node) {
            int32_t ret = se_finalize_gen_steps(se);
            if (ret)
                res = ret;
        }
        if (res)
            continue;
        uint64_t flush_clock = clock_from_time(&ss->ce, flush_time);
        steppersync_flush(ss, flush_clock);
    }
    return res;
}
