// Support homing/probing "trigger" notification from analog sensors
//
// Copyright (C) 2025  Gareth Farrington <gareth@waves.ky>
// Copyright (C) 2024-2026  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h> // abs
#include "basecmd.h" // oid_alloc
#include "board/io.h" // writeb
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "sched.h" // shutdown
#include "sos_filter.h" // sos_filter_apply
#include "trigger_analog.h" // trigger_analog_update
#include "trsync.h" // trsync_do_trigger

// Main trigger_analog storage
struct trigger_analog {
    // Raw value range check
    int32_t raw_min, raw_max;
    // Filtering
    struct sos_filter *sf;
    // Trigger value checking
    int32_t trigger_value, trigger_peak;
    uint32_t trigger_clock;
    uint8_t trigger_type;
    // Trsync triggering
    uint8_t flags, trigger_reason, error_reason;
    struct trsync *ts;
    uint32_t homing_clock;
    // Sensor activity monitoring
    uint8_t monitor_max, monitor_count;
    struct timer time;
    uint32_t monitor_ticks;
};

// Homing flags
enum {
    TA_AWAIT_HOMING = 1<<1, TA_CAN_TRIGGER = 1<<2
};

// Trigger types
enum {
    TT_ABS_GE, TT_GT, TT_DIFF_PEAK_GT
};
DECL_ENUMERATION("trigger_analog_type", "abs_ge", TT_ABS_GE);
DECL_ENUMERATION("trigger_analog_type", "gt", TT_GT);
DECL_ENUMERATION("trigger_analog_type", "diff_peak_gt", TT_DIFF_PEAK_GT);

// Sample errors sent via trsync error code
enum {
    TE_RAW_RANGE, TE_OVERFLOW, TE_MONITOR, TE_SENSOR_SPECIFIC
};
DECL_ENUMERATION("trigger_analog_error:", "RAW_RANGE", TE_RAW_RANGE);
DECL_ENUMERATION("trigger_analog_error:", "OVERFLOW", TE_OVERFLOW);
DECL_ENUMERATION("trigger_analog_error:", "MONITOR", TE_MONITOR);
DECL_ENUMERATION("trigger_analog_error:", "SENSOR_SPECIFIC"
                 , TE_SENSOR_SPECIFIC);

// Timer callback that monitors for sensor timeouts
static uint_fast8_t
monitor_event(struct timer *t)
{
    struct trigger_analog *ta = container_of(t, struct trigger_analog, time);

    if (!(ta->flags & TA_CAN_TRIGGER))
        return SF_DONE;

    if (ta->monitor_count > ta->monitor_max) {
        trsync_do_trigger(ta->ts, ta->error_reason + TE_MONITOR);
        return SF_DONE;
    }

    // A sample was recently delivered, continue monitoring
    ta->monitor_count++;
    ta->time.waketime += ta->monitor_ticks;
    return SF_RESCHEDULE;
}

// Note recent activity
static void
monitor_note_activity(struct trigger_analog *ta)
{
    writeb(&ta->monitor_count, 0);
}

// Check if a value should signal a "trigger" event
static int
check_trigger(struct trigger_analog *ta, uint32_t time, int32_t value)
{
    switch (ta->trigger_type) {
    case TT_ABS_GE:
        ta->trigger_clock = time;
        return abs(value) >= ta->trigger_value;
    case TT_GT:
        ta->trigger_clock = time;
        return value > ta->trigger_value;
    case TT_DIFF_PEAK_GT:
        if (value > ta->trigger_peak) {
            ta->trigger_clock = time;
            ta->trigger_peak = value;
            return 0;
        }
        uint32_t delta = ta->trigger_peak - value;
        return delta > ta->trigger_value;
    }
    return 0;
}

// Reset fields associated with trigger checking
static void
trigger_reset(struct trigger_analog *ta)
{
    ta->trigger_peak = INT32_MIN;
}

// Stop homing due to an error
static void
cancel_homing(struct trigger_analog *ta, uint8_t error_code)
{
    if (!(ta->flags & TA_CAN_TRIGGER))
        return;
    ta->flags = 0;
    trsync_do_trigger(ta->ts, ta->error_reason + error_code);
}

// Handle an error reported by the sensor
void
trigger_analog_note_error(struct trigger_analog *ta, uint8_t sensor_code)
{
    if (!ta)
        return;
    cancel_homing(ta, sensor_code + TE_SENSOR_SPECIFIC);
}

// Used by Sensors to report new raw ADC sample
void
trigger_analog_update(struct trigger_analog *ta, int32_t sample)
{
    // Check homing is active
    if (!ta)
        return;
    uint8_t flags = ta->flags;
    if (!(flags & TA_CAN_TRIGGER))
        return;

    // Check if homing has started
    uint32_t time = timer_read_time();
    if ((flags & TA_AWAIT_HOMING) && timer_is_before(time, ta->homing_clock))
        return;
    flags &= ~TA_AWAIT_HOMING;

    // Reset the sensor timeout checking
    monitor_note_activity(ta);

    // Check that raw value is in range
    if (sample < ta->raw_min || sample > ta->raw_max) {
        cancel_homing(ta, TE_RAW_RANGE);
        return;
    }

    // Perform filtering
    int32_t filtered_value = sample;
    int ret = sos_filter_apply(ta->sf, &filtered_value);
    if (ret) {
        cancel_homing(ta, TE_OVERFLOW);
        return;
    }

    // Check if this is a "trigger"
    ret = check_trigger(ta, time, filtered_value);
    if (ret) {
        trsync_do_trigger(ta->ts, ta->trigger_reason);
        flags = 0;
    }

    ta->flags = flags;
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

// Set valid raw range
void
command_trigger_analog_set_raw_range(uint32_t *args)
{
    struct trigger_analog *ta = trigger_analog_oid_lookup(args[0]);
    ta->raw_min = args[1];
    ta->raw_max = args[2];
}
DECL_COMMAND(command_trigger_analog_set_raw_range,
    "trigger_analog_set_raw_range oid=%c raw_min=%i raw_max=%i");

// Set the triggering type and value
void
command_trigger_analog_set_trigger(uint32_t *args)
{
    struct trigger_analog *ta = trigger_analog_oid_lookup(args[0]);
    ta->trigger_type = args[1];
    ta->trigger_value = args[2];
}
DECL_COMMAND(command_trigger_analog_set_trigger, "trigger_analog_set_trigger"
    " oid=%c trigger_analog_type=%c trigger_value=%i");

// Home an axis
void
command_trigger_analog_home(uint32_t *args)
{
    struct trigger_analog *ta = trigger_analog_oid_lookup(args[0]);
    sched_del_timer(&ta->time);
    ta->monitor_ticks = args[5];
    if (!ta->monitor_ticks) {
        ta->flags = 0;
        ta->ts = NULL;
        return;
    }
    ta->ts = trsync_oid_lookup(args[1]);
    ta->trigger_reason = args[2];
    ta->error_reason = args[3];
    ta->time.waketime = ta->homing_clock = args[4];
    ta->monitor_max = args[6];
    ta->monitor_count = 0;
    ta->time.func = monitor_event;
    ta->flags = TA_AWAIT_HOMING | TA_CAN_TRIGGER;
    trigger_reset(ta);
    sched_add_timer(&ta->time);
}
DECL_COMMAND(command_trigger_analog_home,
             "trigger_analog_home oid=%c trsync_oid=%c trigger_reason=%c"
             " error_reason=%c clock=%u monitor_ticks=%u monitor_max=%u");

void
command_trigger_analog_query_state(uint32_t *args)
{
    uint8_t oid = args[0];
    struct trigger_analog *ta = trigger_analog_oid_lookup(args[0]);
    sendf("trigger_analog_state oid=%c homing=%c trigger_clock=%u"
          , oid, !!(ta->flags & TA_CAN_TRIGGER), ta->trigger_clock);
}
DECL_COMMAND(command_trigger_analog_query_state
             , "trigger_analog_query_state oid=%c");
