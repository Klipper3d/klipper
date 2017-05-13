// Debugging commands.
//
// Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "command.h" // DECL_COMMAND
#include "sched.h" // sched_add_timer


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
    sched_add_timer(&group_timer);
}
DECL_COMMAND(command_start_group, "start_group clock=%u");

void
command_end_group(uint32_t *args)
{
    sched_del_timer(&group_timer);
}
DECL_COMMAND(command_end_group, "end_group");


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

void
command_debug_ping(uint32_t *args)
{
    uint8_t len = args[0];
    char *data = (void*)(size_t)args[1];
    sendf("pong data=%*s", len, data);
}
DECL_COMMAND_FLAGS(command_debug_ping, HF_IN_SHUTDOWN, "debug_ping data=%*s");

void
command_debug_nop(uint32_t *args)
{
}
DECL_COMMAND_FLAGS(command_debug_nop, HF_IN_SHUTDOWN, "debug_nop data=%*s");
