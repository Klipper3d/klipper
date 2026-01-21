// Support homing/probing "trigger" notification from analog sensors
//
// Copyright (C) 2025  Gareth Farrington <gareth@waves.ky>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h> // abs
#include "basecmd.h" // oid_alloc
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "sched.h" // shutdown
#include "sos_filter.h" // sos_filter_apply
#include "trigger_analog.h" // trigger_analog_update
#include "trsync.h" // trsync_do_trigger

#define ERROR_SAFETY_RANGE 0
#define ERROR_OVERFLOW 1
#define ERROR_WATCHDOG 2

// Flags
enum {FLAG_IS_HOMING = 1 << 0
    , FLAG_IS_HOMING_TRIGGER = 1 << 1
    , FLAG_AWAIT_HOMING = 1 << 2
};

// Endstop Structure
struct trigger_analog {
    struct timer time;
    uint32_t trigger_ticks, last_sample_ticks, rest_ticks;
    uint32_t homing_start_time;
    struct trsync *ts;
    int32_t safety_counts_min, safety_counts_max;
    uint8_t flags, trigger_reason, error_reason, watchdog_max, watchdog_count;
    int32_t trigger_value;
    struct sos_filter *sf;
};

static inline uint8_t
is_flag_set(const uint8_t mask, struct trigger_analog *ta)
{
    return !!(mask & ta->flags);
}

static inline void
set_flag(uint8_t mask, struct trigger_analog *ta)
{
    ta->flags |= mask;
}

static inline void
clear_flag(uint8_t mask, struct trigger_analog *ta)
{
    ta->flags &= ~mask;
}

void
try_trigger(struct trigger_analog *ta, uint32_t ticks)
{
    uint8_t is_homing_triggered = is_flag_set(FLAG_IS_HOMING_TRIGGER, ta);
    if (!is_homing_triggered) {
        // the first triggering sample when homing sets the trigger time
        ta->trigger_ticks = ticks;
        // this flag latches until a reset, disabling further triggering
        set_flag(FLAG_IS_HOMING_TRIGGER, ta);
        trsync_do_trigger(ta->ts, ta->trigger_reason);
    }
}

void
trigger_error(struct trigger_analog *ta, uint8_t error_code)
{
    trsync_do_trigger(ta->ts, ta->error_reason + error_code);
}

// Used by Sensors to report new raw ADC sample
void
trigger_analog_update(struct trigger_analog *ta, const int32_t sample)
{
    if (!ta)
        return;

    // only process samples when homing
    uint8_t is_homing = is_flag_set(FLAG_IS_HOMING, ta);
    if (!is_homing) {
        return;
    }

    // save new sample
    uint32_t ticks = timer_read_time();
    ta->last_sample_ticks = ticks;
    ta->watchdog_count = 0;

    // do not trigger before homing start time
    uint8_t await_homing = is_flag_set(FLAG_AWAIT_HOMING, ta);
    if (await_homing && timer_is_before(ticks, ta->homing_start_time)) {
        return;
    }
    clear_flag(FLAG_AWAIT_HOMING, ta);

    // check for safety limit violations
    const uint8_t is_safety_trigger = sample <= ta->safety_counts_min
                                        || sample >= ta->safety_counts_max;
    // too much force, this is an error while homing
    if (is_safety_trigger) {
        trigger_error(ta, ERROR_SAFETY_RANGE);
        return;
    }

    // perform filtering
    int32_t filtered_value = sample;
    int ret = sos_filter_apply(ta->sf, &filtered_value);
    if (ret) {
        trigger_error(ta, ERROR_OVERFLOW);
        return;
    }

    // update trigger state
    if (abs(filtered_value) >= ta->trigger_value) {
        try_trigger(ta, ta->last_sample_ticks);
    }
}

// Timer callback that monitors for timeouts
static uint_fast8_t
watchdog_event(struct timer *t)
{
    struct trigger_analog *ta = container_of(t, struct trigger_analog, time);
    uint8_t is_homing = is_flag_set(FLAG_IS_HOMING, ta);
    uint8_t is_homing_trigger = is_flag_set(FLAG_IS_HOMING_TRIGGER, ta);
    // the watchdog stops when not homing or when trsync becomes triggered
    if (!is_homing || is_homing_trigger) {
        return SF_DONE;
    }

    if (ta->watchdog_count > ta->watchdog_max) {
        trigger_error(ta, ERROR_WATCHDOG);
    }
    ta->watchdog_count += 1;

    // A sample was recently delivered, continue monitoring
    ta->time.waketime += ta->rest_ticks;
    return SF_RESCHEDULE;
}

// Create a trigger_analog
void
command_config_trigger_analog(uint32_t *args)
{
    struct trigger_analog *ta = oid_alloc(
        args[0], command_config_trigger_analog, sizeof(*ta));
    ta->sf = sos_filter_oid_lookup(args[1]);
}
DECL_COMMAND(command_config_trigger_analog
             , "config_trigger_analog oid=%c sos_filter_oid=%c");

// Lookup a trigger_analog
struct trigger_analog *
trigger_analog_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_trigger_analog);
}

// Set the triggering range and tare value
void
command_trigger_analog_set_range(uint32_t *args)
{
    struct trigger_analog *ta = trigger_analog_oid_lookup(args[0]);
    ta->safety_counts_min = args[1];
    ta->safety_counts_max = args[2];
    ta->trigger_value = args[3];
}
DECL_COMMAND(command_trigger_analog_set_range, "trigger_analog_set_range"
    " oid=%c safety_counts_min=%i safety_counts_max=%i trigger_value=%i");

// Home an axis
void
command_trigger_analog_home(uint32_t *args)
{
    struct trigger_analog *ta = trigger_analog_oid_lookup(args[0]);
    sched_del_timer(&ta->time);
    // clear the homing trigger flag
    clear_flag(FLAG_IS_HOMING_TRIGGER, ta);
    clear_flag(FLAG_IS_HOMING, ta);
    ta->trigger_ticks = 0;
    ta->ts = NULL;
    // 0 samples indicates homing is finished
    if (args[3] == 0) {
        // Disable end stop checking
        return;
    }
    ta->ts = trsync_oid_lookup(args[1]);
    ta->trigger_reason = args[2];
    ta->error_reason = args[3];
    ta->time.waketime = args[4];
    ta->homing_start_time = args[4];
    ta->rest_ticks = args[5];
    ta->watchdog_max = args[6];
    ta->watchdog_count = 0;
    ta->time.func = watchdog_event;
    set_flag(FLAG_IS_HOMING, ta);
    set_flag(FLAG_AWAIT_HOMING, ta);
    sched_add_timer(&ta->time);
}
DECL_COMMAND(command_trigger_analog_home,
             "trigger_analog_home oid=%c trsync_oid=%c trigger_reason=%c"
             " error_reason=%c clock=%u rest_ticks=%u timeout=%u");

void
command_trigger_analog_query_state(uint32_t *args)
{
    uint8_t oid = args[0];
    struct trigger_analog *ta = trigger_analog_oid_lookup(args[0]);
    sendf("trigger_analog_state oid=%c is_homing_trigger=%c trigger_ticks=%u"
            , oid
            , is_flag_set(FLAG_IS_HOMING_TRIGGER, ta)
            , ta->trigger_ticks);
}
DECL_COMMAND(command_trigger_analog_query_state
             , "trigger_analog_query_state oid=%c");
