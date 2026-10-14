// Exercise actual firmware C: analog -> trsync -> wire -> endstop -> stop.
// Copyright (C) 2026 Xiaoyue Cui <2508041672@qq.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
//
// Only hardware, OID storage and scheduler services are stubbed. No host
// command is issued between a sensor event and the receiving stop callback.
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#define __BASECMD_H
#define __COMMAND_H
#define __SCHED_H
#define DECL_COMMAND(...)
#define DECL_ENUMERATION(...)
#define DECL_TASK(...)
#define DECL_SHUTDOWN(...)
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
#define sendf(...) ((void)0)
enum { SF_DONE, SF_RESCHEDULE };
struct timer {
    struct timer *next;
    uint_fast8_t (*func)(struct timer *);
    uint32_t waketime;
};
struct task_wake { uint8_t wake; };
static void sched_add_timer(struct timer *t) {}
static void sched_del_timer(struct timer *t) {}
static void sched_wake_task(struct task_wake *w) { w->wake = 1; }
static int sched_check_wake(struct task_wake *w) {
    int old = w->wake; w->wake = 0; return old;
}
static jmp_buf shutdown_jmp;
static int expect_shutdown;
static void shutdown(const char *reason) {
    if (expect_shutdown) longjmp(shutdown_jmp, 1);
    fprintf(stderr, "Unexpected shutdown: %s\n", reason);
    abort();
}
static void *objects[32];
static void *types[32];
static void *oid_alloc(uint8_t oid, void *type, uint16_t size) {
    assert(oid < 32 && !objects[oid]);
    types[oid] = type;
    return objects[oid] = calloc(1, size);
}
static void *oid_lookup(uint8_t oid, void *type) {
    assert(oid < 32 && types[oid] == type && objects[oid]);
    return objects[oid];
}
#define foreach_oid(oid, ptr, type) \
    for (oid = 0; oid < 32; oid++) \
        if (types[oid] == type && ((ptr) = objects[oid]))

#include "../../src/trsync.c"
#include "../../src/endstop_bridge.c"
#include "../../src/trigger_analog.c"
#include "../../src/endstop.c"

static int filter_error;
int sos_filter_apply(struct sos_filter *sf, int32_t *value) {
    return filter_error;
}
struct sos_filter *sos_filter_oid_lookup(uint8_t oid) { return NULL; }
static unsigned stopped;
static struct trsync_signal motor_signal;
static void stop_motor(struct trsync_signal *signal, uint8_t reason) {
    assert(reason == 1);
    stopped++;
}
#define CMD(fn, ...) do { uint32_t args[] = {__VA_ARGS__}; fn(args); } while (0)

static void start(void) {
    CMD(command_set_endstop_bridge, 0, EB_IDLE, 0, 0);
    CMD(command_trsync_start, 2, 0, 0, 4);
    CMD(command_trsync_start, 3, 0, 0, 4);
    trsync_add_signal(trsync_oid_lookup(3), &motor_signal, stop_motor);
    CMD(command_endstop_home, 4, 100, 15, 4, 50, 0, 3, 1);
    CMD(command_arm_endstop_bridge, 0, 2);
    CMD(command_trigger_analog_home, 5, 2, 1, 5, 100, 10, 3);
    now = 100;
    stopped = 0;
    filter_error = 0;
    assert(levels[0] == 1);
}

static void check_stop(unsigned reason) {
    assert(trsync_oid_lookup(2)->trigger_reason == reason);
    assert(levels[0] == 0);
    struct endstop *e = objects[4];
    for (int i = 0; i < 4; i++)
        assert(e->time.func(&e->time) == (i == 3 ? SF_DONE : SF_RESCHEDULE));
    assert(stopped == 1);
    // Signal is latched, and cannot retrigger an already stopped motor.
    trsync_do_trigger(trsync_oid_lookup(2), 1);
    assert(stopped == 1 && levels[0] == 0);
    CMD(command_trigger_analog_home, 5, 0, 0, 0, 0, 0, 0);
}

int main(void) {
    CMD(command_config_endstop_bridge, 0, 0, 1);
    CMD(command_config_endstop_bridge_input, 1, 0, 1, 1, 0, 2, 50);
    CMD(command_config_trsync, 2);
    CMD(command_config_trsync, 3);
    // Same fake pin represents the physical wire between two MCUs.
    CMD(command_config_endstop, 4, 0, 1);
    CMD(command_config_trigger_analog, 5, 0);
    CMD(command_trigger_analog_set_raw_range, 5, -1000, 1000);
    CMD(command_trigger_analog_set_trigger, 5, TT_ABS_GE, 100);

    struct endstop_bridge *bridge = objects[0];
    CMD(command_set_endstop_bridge, 0, EB_INPUT, 1, 0);
    levels[1] = 1;
    bridge_event(&bridge->timer);
    assert(levels[0] == 1); // one sample is insufficient
    bridge_event(&bridge->timer);
    assert(levels[0] == 0);
    levels[1] = 0;
    bridge_event(&bridge->timer);
    assert(levels[0] == 1);

    start();
    trigger_analog_update(objects[5], 99);
    assert(levels[0] == 1 && !stopped);
    trigger_analog_update(objects[5], 100);
    check_stop(1);
    start();
    trigger_analog_update(objects[5], 1001);
    check_stop(5 + TE_RAW_RANGE);
    start();
    filter_error = 1;
    trigger_analog_update(objects[5], 0);
    check_stop(5 + TE_OVERFLOW);
    start();
    trigger_analog_note_error(objects[5], 2);
    check_stop(5 + TE_SENSOR_SPECIFIC + 2);
    start();
    struct trigger_analog *ta = objects[5];
    for (int i = 0; i < 5; i++) monitor_event(&ta->time);
    check_stop(5 + TE_MONITOR);
    start();
    trsync_expire_event(&trsync_oid_lookup(2)->expire_time);
    check_stop(4);
    start();
    expect_shutdown = 1;
    if (!setjmp(shutdown_jmp)) {
        CMD(command_set_endstop_bridge, 0, EB_INPUT, 1, 0);
        assert(!"Armed bridge accepted another source");
    }
    expect_shutdown = 0;
    endstop_bridge_shutdown();
    assert(levels[0] == 0);
    trsync_shutdown();
    for (int i = 0; i < 32; i++) free(objects[i]);
    puts("PASS: digital filter; analog contact/range/filter/sensor/monitor/"
         "watchdog events; local wire stop; arbitration; shutdown");
    return 0;
}
