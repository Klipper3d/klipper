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
    uint32_t *queue, *queue_end, *queue_pos, *queue_next;
    // Internal tracking
    uint32_t relclock, max_error;
    // Error checking
    uint32_t errors;
    // Message generation
    uint64_t last_step_clock;
    struct list_head msg_queue;
    uint32_t queue_step_msgid, oid;
};


/****************************************************************
 * Queue management
 ****************************************************************/

// Shuffle the internal queue to avoid having to allocate more ram
static void
clean_queue(struct stepcompress *sc)
{
    uint32_t *src = sc->queue_pos, *dest = sc->queue;
    while (src < sc->queue_next)
        *dest++ = *src++ - sc->relclock;
    sc->queue_pos = sc->queue;
    sc->queue_next = dest;
    sc->relclock = 0;
}

// Expand the internal queue of step times
static void
expand_queue(struct stepcompress *sc, int count)
{
    if (sc->queue + count <= sc->queue_end) {
        clean_queue(sc);
        return;
    }
    int alloc = sc->queue_end - sc->queue;
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

// Check if the internal queue needs to be expanded, and expand if so
static inline void
check_expand(struct stepcompress *sc, int count)
{
    if (sc->queue_next + count > sc->queue_end)
        expand_queue(sc, count);
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
minmax_point(struct stepcompress *sc, uint32_t *pos)
{
    uint32_t prevpoint = pos > sc->queue_pos ? *(pos-1) - sc->relclock : 0;
    uint32_t point = *pos - sc->relclock;
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
    uint32_t *last = sc->queue_next;
    if (last > sc->queue_pos + 65535)
        last = sc->queue_pos + 65535;
    struct points point = minmax_point(sc, sc->queue_pos);
    int32_t origmininterval = point.minp, origmaxinterval = point.maxp;
    int32_t add = 0, minadd=-0x8001, maxadd=0x8000;
    int32_t bestadd=0, bestcount=0, bestinterval=0;

    for (;;) {
        // Find longest valid sequence with the given 'add'
        int32_t mininterval = origmininterval, maxinterval = origmaxinterval;
        int32_t count = 1, addfactor = 0;
        for (;;) {
            if (sc->queue_pos + count >= last)
                return (struct step_move){ maxinterval, count, add };
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
        if (count > bestcount || (count == bestcount && add > bestadd)) {
            bestcount = count;
            bestadd = add;
            bestinterval = maxinterval;
        }

        // Check if a greater or lesser add could extend the sequence
        int32_t maxreach = add*addfactor + maxinterval*(count+1);
        if (maxreach < point.minp) {
            minadd = add;
            origmaxinterval = maxinterval;
        } else {
            maxadd = add;
            origmininterval = mininterval;
        }

        // See if next point would further limit the add range
        if ((minadd+1)*addfactor + origmaxinterval*(count+1) < point.minp)
            minadd = idiv_up(point.minp - origmaxinterval*(count+1)
                             , addfactor) - 1;
        if ((maxadd-1)*addfactor + origmininterval*(count+1) > point.maxp)
            maxadd = idiv_down(point.maxp - origmininterval*(count+1)
                               , addfactor) + 1;

        // The maximum valid deviation between two quadratic sequences
        // can be calculated and used to further limit the add range.
        if (count > 1) {
            int32_t errdelta = DIV_UP(sc->max_error*QUADRATIC_DEV, count*count);
            if (minadd < add - errdelta)
                minadd = add - errdelta;
            if (maxadd > add + errdelta)
                maxadd = add + errdelta;
        }

        // Bisect valid add range and try again with new 'add'
        add = (maxadd + minadd) / 2;
        if (add <= minadd || add >= maxadd)
            break;
    }
    if (bestcount < 2)
        bestadd = 0;
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
    int err = 0;
    if (!move.count || !move.interval || move.interval >= 0x80000000) {
        fprintf(stderr, "ERROR: Point out of range: %d %d %d\n"
                , move.interval, move.count, move.add);
        err++;
    }
    uint32_t interval = move.interval, p = interval;
    uint16_t i;
    for (i=0; i<move.count; i++) {
        struct points point = minmax_point(sc, sc->queue_pos + i);
        if (p < point.minp || p > point.maxp) {
            fprintf(stderr, "ERROR: Point %d of %d: %d not in %d:%d\n"
                    , i+1, move.count, p, point.minp, point.maxp);
            err++;
        }
        interval += move.add;
        p += interval;
    }
    sc->errors += err;
}


/****************************************************************
 * Step compress interface
 ****************************************************************/

// Allocate a new 'stepcompress' object
struct stepcompress *
stepcompress_alloc(uint32_t max_error, uint32_t queue_step_msgid, uint32_t oid)
{
    struct stepcompress *sc = malloc(sizeof(*sc));
    memset(sc, 0, sizeof(*sc));
    sc->max_error = max_error;
    list_init(&sc->msg_queue);
    sc->queue_step_msgid = queue_step_msgid;
    sc->oid = oid;
    return sc;
}

// Schedule a step event at the specified step_clock time
void
stepcompress_push(struct stepcompress *sc, double step_clock)
{
    check_expand(sc, 1);
    step_clock += 0.5 + sc->relclock - sc->last_step_clock;
    *sc->queue_next++ = step_clock;
}

// Schedule 'steps' number of steps with a constant time between steps
// using the formula: step_clock = clock_offset + step_num*factor
double
stepcompress_push_factor(struct stepcompress *sc
                         , double steps, double step_offset
                         , double clock_offset, double factor)
{
    // Calculate number of steps to take
    double ceil_steps = ceil(steps - step_offset);
    double next_step_offset = ceil_steps - (steps - step_offset);
    int count = ceil_steps;
    check_expand(sc, count);

    // Calculate each step time
    uint32_t *qn = sc->queue_next, *end = &qn[count];
    clock_offset += 0.5 + sc->relclock - sc->last_step_clock;
    double pos = step_offset;
    while (qn < end) {
        *qn++ = clock_offset + pos*factor;
        pos += 1.0;
    }
    sc->queue_next = qn;
    return next_step_offset;
}

// Schedule 'steps' number of steps using the formula:
//  step_clock = clock_offset + sqrt(step_num*factor + sqrt_offset)
double
stepcompress_push_sqrt(struct stepcompress *sc, double steps, double step_offset
                       , double clock_offset, double sqrt_offset, double factor)
{
    // Calculate number of steps to take
    double ceil_steps = ceil(steps - step_offset);
    double next_step_offset = ceil_steps - (steps - step_offset);
    int count = ceil_steps;
    check_expand(sc, count);

    // Calculate each step time
    uint32_t *qn = sc->queue_next, *end = &qn[count];
    clock_offset += 0.5 + sc->relclock - sc->last_step_clock;
    double pos = step_offset + sqrt_offset/factor;
    if (factor >= 0.0)
        while (qn < end) {
            *qn++ = clock_offset + sqrt(pos*factor);
            pos += 1.0;
        }
    else
        while (qn < end) {
            *qn++ = clock_offset - sqrt(pos*factor);
            pos += 1.0;
        }
    sc->queue_next = end;
    return next_step_offset;
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
        qm->req_clock = sc->last_step_clock;
        list_add_tail(&qm->node, &sc->msg_queue);

        uint32_t addfactor = move.count*(move.count-1)/2;
        uint32_t ticks = move.add*addfactor + move.interval*move.count;
        sc->last_step_clock += ticks;
        if (sc->queue_pos + move.count >= sc->queue_next) {
            sc->queue_pos = sc->queue_next = sc->queue;
            sc->relclock = 0;
            break;
        }
        sc->queue_pos += move.count;
        sc->relclock += ticks;
    }
}

// Reset the internal state of the stepcompress object
void
stepcompress_reset(struct stepcompress *sc, uint64_t last_step_clock)
{
    stepcompress_flush(sc, UINT64_MAX);
    sc->last_step_clock = last_step_clock;
}

// Queue an mcu command to go out in order with stepper commands
void
stepcompress_queue_msg(struct stepcompress *sc, uint32_t *data, int len)
{
    stepcompress_flush(sc, UINT64_MAX);

    struct queue_message *qm = message_alloc_and_encode(data, len);
    qm->min_clock = -1;
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

// Allocate a new 'stepperysync' object
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
    uint64_t min_clock = ss->move_clocks[0];
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
        if (!qm || (!qm->min_clock && req_clock > move_clock))
            break;

        // Set the min_clock for this command
        if (!qm->min_clock) {
            qm->min_clock = min_clock;
            heap_replace(ss, req_clock);
            min_clock = ss->move_clocks[0];
        } else {
            qm->min_clock = min_clock;
        }

        // Batch this command
        list_del(&qm->node);
        list_add_tail(&qm->node, &msgs);
    }

    // Transmit commands
    if (!list_empty(&msgs))
        serialqueue_send_batch(ss->sq, ss->cq, &msgs);
}
