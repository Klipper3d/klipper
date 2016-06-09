// Basic infrastructure commands.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h> // malloc
#include <string.h> // memcpy
#include "basecmd.h" // lookup_oid
#include "board/irq.h" // irq_save
#include "board/misc.h" // alloc_maxsize
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_clear_shutdown


/****************************************************************
 * Move queue
 ****************************************************************/

static struct move *move_list, *move_free_list;
static uint16_t move_count;

void
move_free(struct move *m)
{
    m->next = move_free_list;
    move_free_list = m;
}

struct move *
move_alloc(void)
{
    irqstatus_t flag = irq_save();
    struct move *m = move_free_list;
    if (!m)
        shutdown("Move queue empty");
    move_free_list = m->next;
    irq_restore(flag);
    return m;
}

static void
move_reset(void)
{
    if (!move_count)
        return;
    // Add everything in move_list to the free list.
    uint32_t i;
    for (i=0; i<move_count-1; i++)
        move_list[i].next = &move_list[i+1];
    move_list[move_count-1].next = NULL;
    move_free_list = &move_list[0];
}
DECL_SHUTDOWN(move_reset);


/****************************************************************
 * Generic object ids (oid)
 ****************************************************************/

struct oid_s {
    void *type, *data;
};

static struct oid_s *oids;
static uint8_t num_oid;
static uint32_t config_crc;
static uint8_t config_finalized;

void *
lookup_oid(uint8_t oid, void *type)
{
    if (oid >= num_oid || type != oids[oid].type)
        shutdown("Invalid oid type");
    return oids[oid].data;
}

static void
assign_oid(uint8_t oid, void *type, void *data)
{
    if (oid >= num_oid || oids[oid].type || config_finalized)
        shutdown("Can't assign oid");
    oids[oid].type = type;
    oids[oid].data = data;
}

void *
alloc_oid(uint8_t oid, void *type, uint16_t size)
{
    void *data = malloc(size);
    if (!data)
        shutdown("malloc failed");
    memset(data, 0, size);
    assign_oid(oid, type, data);
    return data;
}

void *
next_oid(uint8_t *i, void *type)
{
    uint8_t oid = *i;
    for (;;) {
        oid++;
        if (oid >= num_oid)
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
    num_oid = count;
}
DECL_COMMAND(command_allocate_oids, "allocate_oids count=%c");

void
command_get_config(uint32_t *args)
{
    sendf("config is_config=%c crc=%u move_count=%hu"
          , config_finalized, config_crc, move_count);
}
DECL_COMMAND_FLAGS(command_get_config, HF_IN_SHUTDOWN, "get_config");

void
command_finalize_config(uint32_t *args)
{
    if (!oids || config_finalized)
        shutdown("Can't finalize");
    uint16_t count = alloc_maxsize(sizeof(*move_list)*1024) / sizeof(*move_list);
    move_list = malloc(count * sizeof(*move_list));
    if (!count || !move_list)
        shutdown("malloc failed");
    move_count = count;
    move_reset();
    config_crc = args[0];
    config_finalized = 1;
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

static void
stats_task(void)
{
    static uint32_t last, count, sumsq;
    uint32_t cur = sched_read_time();
    uint32_t diff = (cur - last) >> 8;
    last = cur;
    count++;
    uint32_t nextsumsq = sumsq + diff*diff;
    if (nextsumsq < sumsq)
        nextsumsq = 0xffffffff;
    sumsq = nextsumsq;

    static uint32_t prev;
    if (sched_is_before(cur, prev + sched_from_us(5000000)))
        return;
    sendf("stats count=%u sum=%u sumsq=%u", count, cur - prev, sumsq);
    prev = cur;
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
command_reset(uint32_t *args)
{
    // XXX - implement reset
}
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "msg_reset");

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
