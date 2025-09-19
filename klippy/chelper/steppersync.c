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

#include <stddef.h> // offsetof
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "compiler.h" // __visible
#include "serialqueue.h" // struct queue_message
#include "stepcompress.h" // stepcompress_flush
#include "steppersync.h" // steppersync_alloc


/****************************************************************
 * SyncEmitter - message generation for each stepper
 ****************************************************************/

struct syncemitter {
    // List node for storage in steppersync list
    struct list_node ss_node;
    // Transmit message queue
    struct list_head msg_queue;
    // Step compression and generation
    struct stepcompress *sc;
};

// Return this emitters 'struct stepcompress' (or NULL if not allocated)
struct stepcompress * __visible
syncemitter_get_stepcompress(struct syncemitter *se)
{
    return se->sc;
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
    struct syncemitter *se = malloc(sizeof(*se));
    memset(se, 0, sizeof(*se));
    list_add_tail(&se->ss_node, &ss->se_list);
    list_init(&se->msg_queue);
    if (alloc_stepcompress)
        se->sc = stepcompress_alloc(name, &se->msg_queue);
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
            stepcompress_free(se->sc);
            message_queue_free(&se->msg_queue);
            free(se);
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
    // Start step generation threads
    list_for_each_entry(ss, &ssm->ss_list, ssm_node) {
        uint64_t flush_clock = clock_from_time(&ss->ce, flush_time);
        struct syncemitter *se;
        list_for_each_entry(se, &ss->se_list, ss_node) {
            if (!se->sc)
                continue;
            stepcompress_start_gen_steps(se->sc, gen_steps_time, flush_clock);
        }
    }
    // Wait for step generation threads to complete
    int32_t res = 0;
    list_for_each_entry(ss, &ssm->ss_list, ssm_node) {
        struct syncemitter *se;
        list_for_each_entry(se, &ss->se_list, ss_node) {
            if (!se->sc)
                continue;
            int32_t ret = stepcompress_finalize_gen_steps(se->sc);
            if (ret)
                res = ret;
        }
        if (res)
            continue;
        uint64_t flush_clock = clock_from_time(&ss->ce, flush_time);
        steppersync_flush(ss, flush_clock);
    }
    if (res)
        return res;
    // Clear history
    list_for_each_entry(ss, &ssm->ss_list, ssm_node) {
        uint64_t end_clock = clock_from_time(&ss->ce, clear_history_time);
        struct syncemitter *se;
        list_for_each_entry(se, &ss->se_list, ss_node) {
            if (!se->sc)
                continue;
            stepcompress_history_expire(se->sc, end_clock);
        }
    }
    return 0;
}
