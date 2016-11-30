// Stepper pulse schedule compression
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
//
// The goal of this code is to take a series of scheduled stepper
// pulse times and compress them into a handful of commands that can
// be efficiently transmitted and executed on a microcontroller (mcu).
// The mcu accepts step pulse commands that take interval, count, and
// add parameters such that 'count' pulses occur, with each step event
// calculating the next step event time using:
//  next_wake_time = last_wake_time + interval; interval += add
// This code is writtin in C (instead of python) for processing
// efficiency - the repetitive integer math is vastly faster in C.

#include <math.h> // sqrt
#include <stddef.h> // offsetof
#include <stdint.h> // uint32_t
#include <stdio.h> // fprintf
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "serialqueue.h" // struct queue_message

#define CHECK_LINES 1
#define QUEUE_START_SIZE 1024

struct stepcompress {
    // Buffer management
    uint64_t *queue, *queue_end, *queue_pos, *queue_next;
    // Internal tracking
    uint32_t max_error;
    // Error checking
    uint32_t errors;
    // Message generation
    uint64_t last_step_clock;
    struct list_head msg_queue;
    uint32_t queue_step_msgid, set_next_step_dir_msgid, oid;
    int sdir, invert_sdir;
};


/****************************************************************
 * Queue management
 ****************************************************************/

// Shuffle the internal queue to avoid having to allocate more ram
static void
clean_queue(struct stepcompress *sc)
{
    int in_use = sc->queue_next - sc->queue_pos;
    memmove(sc->queue, sc->queue_pos, in_use * sizeof(*sc->queue));
    sc->queue_pos = sc->queue;
    sc->queue_next = sc->queue + in_use;
}

// Expand the internal queue of step times
static void
expand_queue(struct stepcompress *sc, int count)
{
    int alloc = sc->queue_end - sc->queue;
    if (count + sc->queue_next - sc->queue_pos <= alloc) {
        clean_queue(sc);
        return;
    }
    int pos = sc->queue_pos - sc->queue;
    int next = sc->queue_next - sc->queue;
    if (!alloc)
        alloc = QUEUE_START_SIZE;
    while (next + count > alloc)
        alloc *= 2;
    sc->queue = realloc(sc->queue, alloc * sizeof(*sc->queue));
    sc->queue_end = sc->queue + alloc;
    sc->queue_pos = sc->queue + pos;
    sc->queue_next = sc->queue + next;
}


/****************************************************************
 * Step compression
 ****************************************************************/

#define DIV_UP(n,d) (((n) + (d) - 1) / (d))

static inline int32_t
idiv_up(int32_t n, int32_t d)
{
    return (n>=0) ? DIV_UP(n,d) : (n/d);
}

static inline int32_t
idiv_down(int32_t n, int32_t d)
{
    return (n>=0) ? (n/d) : (n - d + 1) / d;
}

struct points {
    int32_t minp, maxp;
};

// Given a requested step time, return the minimum and maximum
// acceptable times
static struct points
minmax_point(struct stepcompress *sc, uint64_t *pos)
{
    uint32_t prevpoint = pos > sc->queue_pos ? *(pos-1) - sc->last_step_clock : 0;
    uint32_t point = *pos - sc->last_step_clock;
    uint32_t max_error = (point - prevpoint) / 2;
    if (max_error > sc->max_error)
        max_error = sc->max_error;
    return (struct points){ point - max_error, point };
}

// The maximum add delta between two valid quadratic sequences of the
// form "add*count*(count-1)/2 + interval*count" is "(6 + 4*sqrt(2)) *
// maxerror / (count*count)".  The "6 + 4*sqrt(2)" is 11.65685, but
// using 11 and rounding up when dividing works well in practice.
#define QUADRATIC_DEV 11

struct step_move {
    uint32_t interval;
    uint16_t count;
    int16_t add;
};

// Find a 'step_move' that covers a series of step times
static struct step_move
compress_bisect_add(struct stepcompress *sc)
{
    struct points point = minmax_point(sc, sc->queue_pos);
    int32_t outer_mininterval = point.minp, outer_maxinterval = point.maxp;
    int32_t add = 0, minadd = -0x8001, maxadd = 0x8000;
    int32_t bestinterval = 0, bestcount = 1, bestadd = 0, bestreach = INT32_MIN;
    int32_t checked_count = 0;

    for (;;) {
        // Find longest valid sequence with the given 'add'
        int32_t mininterval = outer_mininterval, maxinterval = outer_maxinterval;
        int32_t count = 1, addfactor = 0;
        for (;;) {
            if (count > checked_count) {
                if (&sc->queue_pos[count] >= sc->queue_next || count >= 65535
                    || sc->queue_pos[count] >= sc->last_step_clock + (3<<28))
                    return (struct step_move){ maxinterval, count, add };
                checked_count++;
            }
            point = minmax_point(sc, sc->queue_pos + count);
            addfactor += count;
            int32_t c = add*addfactor;
            int32_t nextmininterval = mininterval;
            if (c + nextmininterval*(count+1) < point.minp)
                nextmininterval = DIV_UP(point.minp - c, count+1);
            int32_t nextmaxinterval = maxinterval;
            if (c + nextmaxinterval*(count+1) > point.maxp)
                nextmaxinterval = (point.maxp - c) / (count+1);
            if (nextmininterval > nextmaxinterval)
                break;
            count += 1;
            mininterval = nextmininterval;
            maxinterval = nextmaxinterval;
        }

        // Check if this is the best sequence found so far
        int32_t reach = add*(addfactor-count) + maxinterval*count;
        if (reach > bestreach) {
            bestinterval = maxinterval;
            bestcount = count;
            bestadd = add;
            bestreach = reach;
        }

        // Check if a greater or lesser add could extend the sequence
        int32_t nextreach = add*addfactor + maxinterval*(count+1);
        if (nextreach < point.minp) {
            minadd = add;
            outer_maxinterval = maxinterval;
        } else {
            maxadd = add;
            outer_mininterval = mininterval;
        }

        // The maximum valid deviation between two quadratic sequences
        // can be calculated and used to further limit the add range.
        if (count > 1) {
            int32_t errdelta = DIV_UP(sc->max_error*QUADRATIC_DEV, count*count);
            if (minadd < add - errdelta)
                minadd = add - errdelta;
            if (maxadd > add + errdelta)
                maxadd = add + errdelta;
        }

        // See if next point would further limit the add range
        if ((minadd+1)*addfactor + outer_maxinterval*(count+1) < point.minp)
            minadd = idiv_up(point.minp - outer_maxinterval*(count+1)
                             , addfactor) - 1;
        if ((maxadd-1)*addfactor + outer_mininterval*(count+1) > point.maxp)
            maxadd = idiv_down(point.maxp - outer_mininterval*(count+1)
                               , addfactor) + 1;

        // Bisect valid add range and try again with new 'add'
        add = (minadd + maxadd) / 2;
        if (add <= minadd || add >= maxadd)
            break;
    }
    return (struct step_move){ bestinterval, bestcount, bestadd };
}


/****************************************************************
 * Step compress checking
 ****************************************************************/

// Verify that a given 'step_move' matches the actual step times
static void
check_line(struct stepcompress *sc, struct step_move move)
{
    if (!CHECK_LINES)
        return;
    if (move.count == 1) {
        if (move.interval != (uint32_t)(*sc->queue_pos - sc->last_step_clock)
            || *sc->queue_pos < sc->last_step_clock) {
            fprintf(stderr, "ERROR: Count 1 point out of range: %d %d %d\n"
                    , move.interval, move.count, move.add);
            sc->errors++;
        }
        return;
    }
    int err = 0;
    if (!move.count || !move.interval || move.interval >= 0x80000000) {
        fprintf(stderr, "ERROR: Point out of range: %d %d %d\n"
                , move.interval, move.count, move.add);
        err++;
    }
    uint32_t interval = move.interval, p = 0;
    uint16_t i;
    for (i=0; i<move.count; i++) {
        struct points point = minmax_point(sc, sc->queue_pos + i);
        p += interval;
        if (p < point.minp || p > point.maxp) {
            fprintf(stderr, "ERROR: Point %d of %d: %d not in %d:%d\n"
                    , i+1, move.count, p, point.minp, point.maxp);
            err++;
        }
        if (interval >= 0x80000000) {
            fprintf(stderr, "ERROR: Point %d of %d: interval overflow %d\n"
                    , i+1, move.count, interval);
            err++;
        }
        interval += move.add;
    }
    sc->errors += err;
}


/****************************************************************
 * Step compress interface
 ****************************************************************/

#define likely(x)       __builtin_expect(!!(x), 1)

// Wrapper around sqrt() to handle small negative numbers
static double
_safe_sqrt(double v)
{
    if (v > -0.001)
        // Due to floating point truncation, it's possible to get a
        // small negative number - treat it as zero.
        return 0.;
    return sqrt(v);
}
static inline double safe_sqrt(double v) {
    return likely(v >= 0.) ? sqrt(v) : _safe_sqrt(v);
}

// Allocate a new 'stepcompress' object
struct stepcompress *
stepcompress_alloc(uint32_t max_error, uint32_t queue_step_msgid
                   , uint32_t set_next_step_dir_msgid, uint32_t invert_sdir
                   , uint32_t oid)
{
    struct stepcompress *sc = malloc(sizeof(*sc));
    memset(sc, 0, sizeof(*sc));
    sc->max_error = max_error;
    list_init(&sc->msg_queue);
    sc->queue_step_msgid = queue_step_msgid;
    sc->set_next_step_dir_msgid = set_next_step_dir_msgid;
    sc->oid = oid;
    sc->sdir = -1;
    sc->invert_sdir = !!invert_sdir;
    return sc;
}

// Free memory associated with a 'stepcompress' object
void
stepcompress_free(struct stepcompress *sc)
{
    if (!sc)
        return;
    free(sc->queue);
    message_queue_free(&sc->msg_queue);
    free(sc);
}

// Convert previously scheduled steps into commands for the mcu
static void
stepcompress_flush(struct stepcompress *sc, uint64_t move_clock)
{
    if (sc->queue_pos >= sc->queue_next)
        return;
    while (move_clock > sc->last_step_clock) {
        struct step_move move = compress_bisect_add(sc);
        check_line(sc, move);

        uint32_t msg[5] = {
            sc->queue_step_msgid, sc->oid, move.interval, move.count, move.add
        };
        struct queue_message *qm = message_alloc_and_encode(msg, 5);
        qm->min_clock = qm->req_clock = sc->last_step_clock;
        if (move.count == 1 && sc->last_step_clock + (1<<27) < *sc->queue_pos) {
            // Be careful with 32bit overflow
            sc->last_step_clock = qm->req_clock = *sc->queue_pos;
        } else {
            uint32_t addfactor = move.count*(move.count-1)/2;
            uint32_t ticks = move.add*addfactor + move.interval*move.count;
            sc->last_step_clock += ticks;
        }
        list_add_tail(&qm->node, &sc->msg_queue);

        if (sc->queue_pos + move.count >= sc->queue_next) {
            sc->queue_pos = sc->queue_next = sc->queue;
            break;
        }
        sc->queue_pos += move.count;
    }
}

// Send the set_next_step_dir command
static void
set_next_step_dir(struct stepcompress *sc, int sdir)
{
    sc->sdir = sdir;
    stepcompress_flush(sc, UINT64_MAX);
    uint32_t msg[3] = {
        sc->set_next_step_dir_msgid, sc->oid, sdir ^ sc->invert_sdir
    };
    struct queue_message *qm = message_alloc_and_encode(msg, 3);
    qm->req_clock = sc->last_step_clock;
    list_add_tail(&qm->node, &sc->msg_queue);
}

// Check if the internal queue needs to be expanded, and expand if so
static inline void
check_expand(struct stepcompress *sc, int sdir, int count)
{
    if (sdir != sc->sdir)
        set_next_step_dir(sc, sdir);
    if (sc->queue_next + count > sc->queue_end)
        expand_queue(sc, count);
}

// Schedule a step event at the specified step_clock time
void
stepcompress_push(struct stepcompress *sc, double step_clock, int32_t sdir)
{
    sdir = !!sdir;
    check_expand(sc, sdir, 1);
    step_clock += 0.5;
    *sc->queue_next++ = step_clock;
}

// Schedule 'steps' number of steps with a constant time between steps
// using the formula: step_clock = clock_offset + step_num*factor
int32_t
stepcompress_push_factor(struct stepcompress *sc
                         , double steps, double step_offset
                         , double clock_offset, double factor)
{
    // Calculate number of steps to take
    int sdir = 1;
    if (steps < 0) {
        sdir = 0;
        steps = -steps;
        step_offset = -step_offset;
    }
    int count = steps + .5 - step_offset;
    if (count <= 0 || count > 1000000) {
        if (count && steps)
            fprintf(stderr, "ERROR: push_factor invalid count %d %f %f %f %f\n"
                    , sc->oid, steps, step_offset, clock_offset, factor);
        return 0;
    }
    check_expand(sc, sdir, count);

    // Calculate each step time
    uint64_t *qn = sc->queue_next, *end = &qn[count];
    clock_offset += 0.5;
    double pos = step_offset + .5;
    while (qn < end) {
        *qn++ = clock_offset + pos*factor;
        pos += 1.0;
    }
    sc->queue_next = qn;
    return sdir ? count : -count;
}

// Schedule 'steps' number of steps using the formula:
//  step_clock = clock_offset + sqrt(step_num*factor + sqrt_offset)
int32_t
stepcompress_push_sqrt(struct stepcompress *sc, double steps, double step_offset
                       , double clock_offset, double sqrt_offset, double factor)
{
    // Calculate number of steps to take
    int sdir = 1;
    if (steps < 0) {
        sdir = 0;
        steps = -steps;
        step_offset = -step_offset;
    }
    int count = steps + .5 - step_offset;
    if (count <= 0 || count > 1000000) {
        if (count && steps)
            fprintf(stderr, "ERROR: push_sqrt invalid count %d %f %f %f %f %f\n"
                    , sc->oid, steps, step_offset, clock_offset, sqrt_offset
                    , factor);
        return 0;
    }
    check_expand(sc, sdir, count);

    // Calculate each step time
    uint64_t *qn = sc->queue_next, *end = &qn[count];
    clock_offset += 0.5;
    double pos = step_offset + .5 + sqrt_offset/factor;
    while (qn < end) {
        double v = safe_sqrt(pos*factor);
        *qn++ = clock_offset + (factor >= 0. ? v : -v);
        pos += 1.0;
    }
    sc->queue_next = qn;
    return sdir ? count : -count;
}

// Schedule 'count' number of steps using the delta kinematic const speed
int32_t
stepcompress_push_delta_const(
    struct stepcompress *sc, double clock_offset, double dist, double step_dist
    , double start_pos, double closest_height2, double height, double movez_r
    , double inv_velocity)
{
    // Calculate number of steps to take
    double zdist = dist * movez_r;
    int count = (safe_sqrt(closest_height2 - dist*dist + zdist*zdist)
                 - height - zdist) / step_dist + .5;
    if (count <= 0 || count > 1000000) {
        if (count)
            fprintf(stderr, "ERROR: push_delta_const invalid count"
                    " %d %d %f %f %f %f %f %f %f %f\n"
                    , sc->oid, count, clock_offset, dist, step_dist, start_pos
                    , closest_height2, height, movez_r, inv_velocity);
        return 0;
    }
    check_expand(sc, step_dist > 0., count);

    // Calculate each step time
    uint64_t *qn = sc->queue_next, *end = &qn[count];
    clock_offset += 0.5;
    height += .5 * step_dist;
    while (qn < end) {
        double zh = height*movez_r;
        double v = safe_sqrt(closest_height2 - height*height + zh*zh);
        double pos = start_pos + zh + (step_dist > 0. ? -v : v);
        *qn++ = clock_offset + pos * inv_velocity;
        height += step_dist;
    }
    sc->queue_next = qn;
    return step_dist > 0. ? count : -count;
}

// Schedule 'count' number of steps using delta kinematic acceleration
int32_t
stepcompress_push_delta_accel(
    struct stepcompress *sc, double clock_offset, double dist, double step_dist
    , double start_pos, double closest_height2, double height, double movez_r
    , double accel_multiplier)
{
    // Calculate number of steps to take
    double zdist = dist * movez_r;
    int count = (safe_sqrt(closest_height2 - dist*dist + zdist*zdist)
                 - height - zdist) / step_dist + .5;
    if (count <= 0 || count > 1000000) {
        if (count)
            fprintf(stderr, "ERROR: push_delta_accel invalid count"
                    " %d %d %f %f %f %f %f %f %f %f\n"
                    , sc->oid, count, clock_offset, dist, step_dist, start_pos
                    , closest_height2, height, movez_r, accel_multiplier);
        return 0;
    }
    check_expand(sc, step_dist > 0., count);

    // Calculate each step time
    uint64_t *qn = sc->queue_next, *end = &qn[count];
    clock_offset += 0.5;
    height += .5 * step_dist;
    while (qn < end) {
        double zh = height*movez_r;
        double v = safe_sqrt(closest_height2 - height*height + zh*zh);
        double pos = start_pos + zh + (step_dist > 0. ? -v : v);
        v = safe_sqrt(pos * accel_multiplier);
        *qn++ = clock_offset + (accel_multiplier >= 0. ? v : -v);
        height += step_dist;
    }
    sc->queue_next = qn;
    return step_dist > 0. ? count : -count;
}

// Reset the internal state of the stepcompress object
void
stepcompress_reset(struct stepcompress *sc, uint64_t last_step_clock)
{
    stepcompress_flush(sc, UINT64_MAX);
    sc->last_step_clock = last_step_clock;
    sc->sdir = -1;
}

// Queue an mcu command to go out in order with stepper commands
void
stepcompress_queue_msg(struct stepcompress *sc, uint32_t *data, int len)
{
    stepcompress_flush(sc, UINT64_MAX);

    struct queue_message *qm = message_alloc_and_encode(data, len);
    qm->req_clock = sc->last_step_clock;
    list_add_tail(&qm->node, &sc->msg_queue);
}

// Return the count of internal errors found
uint32_t
stepcompress_get_errors(struct stepcompress *sc)
{
    return sc->errors;
}


/****************************************************************
 * Step compress synchronization
 ****************************************************************/

// The steppersync object is used to synchronize the output of mcu
// step commands.  The mcu can only queue a limited number of step
// commands - this code tracks when items on the mcu step queue become
// free so that new commands can be transmitted.  It also ensures the
// mcu step queue is ordered between steppers so that no stepper
// starves the other steppers of space in the mcu step queue.

struct steppersync {
    // Serial port
    struct serialqueue *sq;
    struct command_queue *cq;
    // Storage for associated stepcompress objects
    struct stepcompress **sc_list;
    int sc_num;
    // Storage for list of pending move clocks
    uint64_t *move_clocks;
    int num_move_clocks;
};

// Allocate a new 'steppersync' object
struct steppersync *
steppersync_alloc(struct serialqueue *sq, struct stepcompress **sc_list
                  , int sc_num, int move_num)
{
    struct steppersync *ss = malloc(sizeof(*ss));
    memset(ss, 0, sizeof(*ss));
    ss->sq = sq;
    ss->cq = serialqueue_alloc_commandqueue();

    ss->sc_list = malloc(sizeof(*sc_list)*sc_num);
    memcpy(ss->sc_list, sc_list, sizeof(*sc_list)*sc_num);
    ss->sc_num = sc_num;

    ss->move_clocks = malloc(sizeof(*ss->move_clocks)*move_num);
    memset(ss->move_clocks, 0, sizeof(*ss->move_clocks)*move_num);
    ss->num_move_clocks = move_num;

    return ss;
}

// Free memory associated with a 'steppersync' object
void
steppersync_free(struct steppersync *ss)
{
    if (!ss)
        return;
    free(ss->sc_list);
    free(ss->move_clocks);
    serialqueue_free_commandqueue(ss->cq);
    free(ss);
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
void
steppersync_flush(struct steppersync *ss, uint64_t move_clock)
{
    // Flush each stepcompress to the specified move_clock
    int i;
    for (i=0; i<ss->sc_num; i++)
        stepcompress_flush(ss->sc_list[i], move_clock);

    // Order commands by the reqclock of each pending command
    struct list_head msgs;
    list_init(&msgs);
    for (;;) {
        // Find message with lowest reqclock
        uint64_t req_clock = MAX_CLOCK;
        struct queue_message *qm = NULL;
        for (i=0; i<ss->sc_num; i++) {
            struct stepcompress *sc = ss->sc_list[i];
            if (!list_empty(&sc->msg_queue)) {
                struct queue_message *m = list_first_entry(
                    &sc->msg_queue, struct queue_message, node);
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
