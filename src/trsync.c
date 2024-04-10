// Handling of synchronized "trigger" dispatch
//
// Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "sched.h" // struct timer
#include "trsync.h" // trsync_do_trigger

struct trsync {
    struct timer report_time, expire_time;
    uint32_t report_ticks;
    struct trsync_signal *signals;
    uint8_t flags, trigger_reason, expire_reason;
};

enum { TSF_CAN_TRIGGER=1<<0, TSF_REPORT=1<<2 };

static struct task_wake trsync_wake;

// Activate a trigger (caller must disable IRQs)
void
trsync_do_trigger(struct trsync *ts, uint8_t reason)
{
    uint8_t flags = ts->flags;
    if (!(flags & TSF_CAN_TRIGGER))
        return;
    ts->trigger_reason = reason;
    ts->flags = (flags & ~TSF_CAN_TRIGGER) | TSF_REPORT;
    // Dispatch signals
    while (ts->signals) {
        struct trsync_signal *tss = ts->signals;
        trsync_callback_t func = tss->func;
        ts->signals = tss->next;
        tss->next = NULL;
        tss->func = NULL;
        func(tss, reason);
    }
    sched_wake_task(&trsync_wake);
}

// Timeout handler
static uint_fast8_t
trsync_expire_event(struct timer *t)
{
    struct trsync *ts = container_of(t, struct trsync, expire_time);
    trsync_do_trigger(ts, ts->expire_reason);
    return SF_DONE;
}

// Report handler
static uint_fast8_t
trsync_report_event(struct timer *t)
{
    struct trsync *ts = container_of(t, struct trsync, report_time);
    ts->flags |= TSF_REPORT;
    sched_wake_task(&trsync_wake);
    ts->report_time.waketime += ts->report_ticks;
    return SF_RESCHEDULE;
}

void
command_config_trsync(uint32_t *args)
{
    struct trsync *ts = oid_alloc(args[0], command_config_trsync, sizeof(*ts));
    ts->report_time.func = trsync_report_event;
    ts->expire_time.func = trsync_expire_event;
}
DECL_COMMAND(command_config_trsync, "config_trsync oid=%c");

// Return the 'struct trsync' for a given trsync oid
struct trsync *
trsync_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_trsync);
}

// Add a callback to invoke on a trigger
void
trsync_add_signal(struct trsync *ts, struct trsync_signal *tss
                  , trsync_callback_t func)
{
    irqstatus_t flag = irq_save();
    if (tss->func || !func)
        shutdown("Can't add signal that is already active");
    tss->func = func;
    tss->next = ts->signals;
    ts->signals = tss;
    irq_restore(flag);
}

// Disable trigger and unregister any signal handlers (caller must disable IRQs)
static void
trsync_clear(struct trsync *ts)
{
    sched_del_timer(&ts->report_time);
    sched_del_timer(&ts->expire_time);
    struct trsync_signal *tss = ts->signals;
    while (tss) {
        struct trsync_signal *next = tss->next;
        tss->func = NULL;
        tss->next = NULL;
        tss = next;
    }
    ts->signals = NULL;
    ts->flags = ts->trigger_reason = ts->expire_reason = 0;
}

void
command_trsync_start(uint32_t *args)
{
    struct trsync *ts = trsync_oid_lookup(args[0]);
    irq_disable();
    trsync_clear(ts);
    ts->flags = TSF_CAN_TRIGGER;
    ts->report_time.waketime = args[1];
    ts->report_ticks = args[2];
    if (ts->report_ticks)
        sched_add_timer(&ts->report_time);
    ts->expire_reason = args[3];
    irq_enable();
}
DECL_COMMAND(command_trsync_start,
             "trsync_start oid=%c report_clock=%u report_ticks=%u"
             " expire_reason=%c");

void
command_trsync_set_timeout(uint32_t *args)
{
    struct trsync *ts = trsync_oid_lookup(args[0]);
    irq_disable();
    uint8_t flags = ts->flags;
    if (flags & TSF_CAN_TRIGGER) {
        sched_del_timer(&ts->expire_time);
        ts->expire_time.waketime = args[1];
        sched_add_timer(&ts->expire_time);
    }
    irq_enable();
}
DECL_COMMAND(command_trsync_set_timeout, "trsync_set_timeout oid=%c clock=%u");

static void
trsync_report(uint8_t oid, uint8_t flags, uint8_t reason, uint32_t clock)
{
    sendf("trsync_state oid=%c can_trigger=%c trigger_reason=%c clock=%u"
          , oid, !!(flags & TSF_CAN_TRIGGER), reason, clock);
}

void
command_trsync_trigger(uint32_t *args)
{
    uint8_t oid = args[0];
    struct trsync *ts = trsync_oid_lookup(oid);
    irq_disable();
    trsync_do_trigger(ts, args[1]);
    sched_del_timer(&ts->report_time);
    sched_del_timer(&ts->expire_time);
    ts->flags = 0;
    uint8_t trigger_reason = ts->trigger_reason;
    irq_enable();
    trsync_report(oid, 0, trigger_reason, 0);
}
DECL_COMMAND(command_trsync_trigger, "trsync_trigger oid=%c reason=%c");

void
trsync_task(void)
{
    if (!sched_check_wake(&trsync_wake))
        return;
    uint8_t oid;
    struct trsync *ts;
    foreach_oid(oid, ts, command_config_trsync) {
        if (!(ts->flags & TSF_REPORT))
            continue;
        uint32_t time = timer_read_time();
        irq_disable();
        uint8_t trigger_reason = ts->trigger_reason, flags = ts->flags;
        ts->flags = flags & ~TSF_REPORT;
        irq_enable();

        trsync_report(oid, flags, trigger_reason, time);
    }
}
DECL_TASK(trsync_task);

void
trsync_shutdown(void)
{
    uint8_t oid;
    struct trsync *ts;
    foreach_oid(oid, ts, command_config_trsync) {
        trsync_clear(ts);
    }
}
DECL_SHUTDOWN(trsync_shutdown);
