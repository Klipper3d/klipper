// Basic infrastructure commands.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h> // malloc
#include <string.h> // memset
#include "basecmd.h" // oid_lookup
#include "board/irq.h" // irq_save
#include "board/misc.h" // alloc_maxsize
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_clear_shutdown


/****************************************************************
 * Move queue
 ****************************************************************/

struct move_freed {
    struct move_freed *next;
};

static struct move_freed *move_free_list;
static void *move_list;
static uint16_t move_count;
static uint8_t move_item_size;

// Is the config and move queue finalized?
static int
is_finalized(void)
{
    return !!move_count;
}

// Free previously allocated storage from move_alloc(). Caller must
// disable irqs.
void
move_free(void *m)
{
    struct move_freed *mf = m;
    mf->next = move_free_list;
    move_free_list = mf;
}

// Allocate runtime storage
void *
move_alloc(void)
{
    irqstatus_t flag = irq_save();
    struct move_freed *mf = move_free_list;
    if (!mf)
        shutdown("Move queue empty");
    move_free_list = mf->next;
    irq_restore(flag);
    return mf;
}

// Request minimum size of runtime allocations returned by move_alloc()
void
move_request_size(int size)
{
    if (size > UINT8_MAX || is_finalized())
        shutdown("Invalid move request size");
    if (size > move_item_size)
        move_item_size = size;
}

static void
move_reset(void)
{
    if (!move_count)
        return;
    // Add everything in move_list to the free list.
    uint32_t i;
    for (i=0; i<move_count-1; i++) {
        struct move_freed *mf = move_list + i*move_item_size;
        mf->next = move_list + (i + 1)*move_item_size;
    }
    struct move_freed *mf = move_list + (move_count - 1)*move_item_size;
    mf->next = NULL;
    move_free_list = move_list;
}
DECL_SHUTDOWN(move_reset);

static void
move_finalize(void)
{
    move_request_size(sizeof(*move_free_list));
    uint16_t count = alloc_maxsize(move_item_size*1024) / move_item_size;
    move_list = malloc(count * move_item_size);
    if (!count || !move_list)
        shutdown("move queue malloc failed");
    move_count = count;
    move_reset();
}


/****************************************************************
 * Generic object ids (oid)
 ****************************************************************/

struct oid_s {
    void *type, *data;
};

static struct oid_s *oids;
static uint8_t oid_count;

void *
oid_lookup(uint8_t oid, void *type)
{
    if (oid >= oid_count || type != oids[oid].type)
        shutdown("Invalid oid type");
    return oids[oid].data;
}

static void
oid_assign(uint8_t oid, void *type, void *data)
{
    if (oid >= oid_count || oids[oid].type || is_finalized())
        shutdown("Can't assign oid");
    oids[oid].type = type;
    oids[oid].data = data;
}

void *
oid_alloc(uint8_t oid, void *type, uint16_t size)
{
    void *data = malloc(size);
    if (!data)
        shutdown("malloc failed");
    memset(data, 0, size);
    oid_assign(oid, type, data);
    return data;
}

void *
oid_next(uint8_t *i, void *type)
{
    uint8_t oid = *i;
    for (;;) {
        oid++;
        if (oid >= oid_count)
            return NULL;
        if (oids[oid].type == type) {
            *i = oid;
            return oids[oid].data;
        }
    }
}

void
command_allocate_oids(uint32_t *args)
{
    if (oids)
        shutdown("oids already allocated");
    uint8_t count = args[0];
    oids = malloc(sizeof(oids[0]) * count);
    if (!oids)
        shutdown("malloc failed");
    memset(oids, 0, sizeof(oids[0]) * count);
    oid_count = count;
}
DECL_COMMAND(command_allocate_oids, "allocate_oids count=%c");


/****************************************************************
 * Config CRC
 ****************************************************************/

static uint32_t config_crc;

void
command_get_config(uint32_t *args)
{
    sendf("config is_config=%c crc=%u move_count=%hu"
          , is_finalized(), config_crc, move_count);
}
DECL_COMMAND_FLAGS(command_get_config, HF_IN_SHUTDOWN, "get_config");

void
command_finalize_config(uint32_t *args)
{
    if (!oids || is_finalized())
        shutdown("Can't finalize");
    move_finalize();
    config_crc = args[0];
    command_get_config(NULL);
}
DECL_COMMAND(command_finalize_config, "finalize_config crc=%u");


/****************************************************************
 * Group commands
 ****************************************************************/

static struct timer group_timer;

static uint_fast8_t
group_end_event(struct timer *timer)
{
    shutdown("Missed scheduling of next event");
}

void
command_start_group(uint32_t *args)
{
    sched_del_timer(&group_timer);
    group_timer.func = group_end_event;
    group_timer.waketime = args[0];
    sched_timer(&group_timer);
}
DECL_COMMAND(command_start_group, "start_group clock=%u");

void
command_end_group(uint32_t *args)
{
    sched_del_timer(&group_timer);
}
DECL_COMMAND(command_end_group, "end_group");


/****************************************************************
 * Timing and load stats
 ****************************************************************/

void
command_get_status(uint32_t *args)
{
    sendf("status clock=%u status=%c", sched_read_time(), sched_is_shutdown());
}
DECL_COMMAND_FLAGS(command_get_status, HF_IN_SHUTDOWN, "get_status");

static uint32_t stats_send_time, stats_send_time_high;

void
command_get_uptime(uint32_t *args)
{
    uint32_t cur = sched_read_time();
    uint32_t high = stats_send_time_high + (cur < stats_send_time);
    sendf("uptime high=%u clock=%u", high, cur);
}
DECL_COMMAND_FLAGS(command_get_uptime, HF_IN_SHUTDOWN, "get_uptime");

#define SUMSQ_BASE 256
DECL_CONSTANT(STATS_SUMSQ_BASE, SUMSQ_BASE);

static void
stats_task(void)
{
    static uint32_t last, count, sumsq;
    uint32_t cur = sched_read_time();
    uint32_t diff = cur - last;
    last = cur;
    count++;
    // Calculate sum of diff^2 - be careful of integer overflow
    uint32_t nextsumsq;
    if (diff <= 0xffff) {
        nextsumsq = sumsq + DIV_ROUND_UP(diff * diff, SUMSQ_BASE);
    } else if (diff <= 0xfffff) {
        nextsumsq = sumsq + DIV_ROUND_UP(diff, SUMSQ_BASE) * diff;
    } else {
        nextsumsq = 0xffffffff;
    }
    if (nextsumsq < sumsq)
        nextsumsq = 0xffffffff;
    sumsq = nextsumsq;

    if (sched_is_before(cur, stats_send_time + sched_from_us(5000000)))
        return;
    sendf("stats count=%u sum=%u sumsq=%u", count, cur - stats_send_time, sumsq);
    if (cur < stats_send_time)
        stats_send_time_high++;
    stats_send_time = cur;
    count = sumsq = 0;
}
DECL_TASK(stats_task);


/****************************************************************
 * Register debug commands
 ****************************************************************/

void
command_debug_read8(uint32_t *args)
{
    uint8_t *ptr = (void*)(size_t)args[0];
    uint16_t v = *ptr;
    sendf("debug_result val=%hu", v);
}
DECL_COMMAND_FLAGS(command_debug_read8, HF_IN_SHUTDOWN, "debug_read8 addr=%u");

void
command_debug_read16(uint32_t *args)
{
    uint16_t *ptr = (void*)(size_t)args[0];
    irqstatus_t flag = irq_save();
    uint16_t v = *ptr;
    irq_restore(flag);
    sendf("debug_result val=%hu", v);
}
DECL_COMMAND_FLAGS(command_debug_read16, HF_IN_SHUTDOWN, "debug_read16 addr=%u");

void
command_debug_write8(uint32_t *args)
{
    uint8_t *ptr = (void*)(size_t)args[0];
    *ptr = args[1];
}
DECL_COMMAND_FLAGS(command_debug_write8, HF_IN_SHUTDOWN,
                   "debug_write8 addr=%u val=%u");

void
command_debug_write16(uint32_t *args)
{
    uint16_t *ptr = (void*)(size_t)args[0];
    irqstatus_t flag = irq_save();
    *ptr = args[1];
    irq_restore(flag);
}
DECL_COMMAND_FLAGS(command_debug_write16, HF_IN_SHUTDOWN,
                   "debug_write16 addr=%u val=%u");


/****************************************************************
 * Misc commands
 ****************************************************************/

void
command_emergency_stop(uint32_t *args)
{
    shutdown("command request");
}
DECL_COMMAND_FLAGS(command_emergency_stop, HF_IN_SHUTDOWN, "emergency_stop");

void
command_clear_shutdown(uint32_t *args)
{
    sched_clear_shutdown();
}
DECL_COMMAND_FLAGS(command_clear_shutdown, HF_IN_SHUTDOWN, "clear_shutdown");

void
command_identify(uint32_t *args)
{
    uint32_t offset = args[0];
    uint8_t count = args[1];
    uint32_t isize = READP(command_identify_size);
    if (offset >= isize)
        count = 0;
    else if (offset + count > isize)
        count = isize - offset;
    sendf("identify_response offset=%u data=%.*s"
          , offset, count, &command_identify_data[offset]);
}
DECL_COMMAND_FLAGS(command_identify, HF_IN_SHUTDOWN,
                   "identify offset=%u count=%c");
