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

// Q2.29
typedef int32_t fixedQ2_t;
#define FIXEDQ2 2
#define FIXEDQ2_FRAC_BITS ((32 - FIXEDQ2) - 1)

// Q32.29 - a Q2.29 value stored in int64
typedef int64_t fixedQ32_t;
#define FIXEDQ32_FRAC_BITS FIXEDQ2_FRAC_BITS

// Q16.15
typedef int32_t fixedQ16_t;
#define FIXEDQ16 16
#define FIXEDQ16_FRAC_BITS ((32 - FIXEDQ16) - 1)

// Q48.15 - a Q16.15 value stored in int64
typedef int64_t fixedQ48_t;
#define FIXEDQ48_FRAC_BITS FIXEDQ16_FRAC_BITS

#define MAX_TRIGGER_GRAMS ((1L << FIXEDQ16) - 1)
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
    uint32_t trigger_grams, trigger_ticks, last_sample_ticks, rest_ticks;
    uint32_t homing_start_time;
    struct trsync *ts;
    int32_t safety_counts_min, safety_counts_max, tare_counts;
    uint8_t flags, trigger_reason, error_reason, watchdog_max, watchdog_count;
    fixedQ16_t trigger_grams_fixed;
    fixedQ2_t grams_per_count;
    struct sos_filter *sf;
};

static inline uint8_t
overflows_int32(int64_t value) {
    return value > (int64_t)INT32_MAX || value < (int64_t)INT32_MIN;
}

// returns the integer part of a fixedQ48_t
static inline int64_t
round_fixedQ48(const int64_t fixed_value) {
    return fixed_value >> FIXEDQ48_FRAC_BITS;
}

// Convert sensor counts to grams
static inline fixedQ48_t
counts_to_grams(struct trigger_analog *ta, const int32_t counts) {
    // tearing ensures readings are referenced to 0.0g
    const int32_t delta = counts - ta->tare_counts;
    // convert sensor counts to grams by multiplication: 124 * 0.051 = 6.324
    // this optimizes to single cycle SMULL instruction
    const fixedQ32_t product = (int64_t)delta * (int64_t)ta->grams_per_count;
    // after multiplication there are 30 fraction bits, reduce to 15
    // caller verifies this wont overflow a 32bit int when truncated
    const fixedQ48_t grams = product >>
                                (FIXEDQ32_FRAC_BITS - FIXEDQ48_FRAC_BITS);
    return grams;
}

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

    // convert sample to grams
    const fixedQ48_t raw_grams = counts_to_grams(ta, sample);
    if (overflows_int32(raw_grams)) {
        trigger_error(ta, ERROR_OVERFLOW);
        return;
    }

    // perform filtering
    int32_t filtered_grams = raw_grams;
    int ret = sos_filter_apply(ta->sf, &filtered_grams);
    if (ret) {
        trigger_error(ta, ERROR_OVERFLOW);
        return;
    }

    // update trigger state
    if (abs(filtered_grams) >= ta->trigger_grams_fixed) {
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

static void
set_endstop_range(struct trigger_analog *ta
                  , int32_t safety_counts_min, int32_t safety_counts_max
                  , int32_t tare_counts, uint32_t trigger_grams
                  , fixedQ2_t grams_per_count)
{
    if (!(safety_counts_max >= safety_counts_min)) {
        shutdown("Safety range reversed");
    }
    if (trigger_grams > MAX_TRIGGER_GRAMS) {
        shutdown("trigger_grams too large");
    }
    // grams_per_count must be a positive fraction in Q2 format
    const fixedQ2_t one = 1L << FIXEDQ2_FRAC_BITS;
    if (grams_per_count < 0 || grams_per_count >= one) {
        shutdown("grams_per_count is invalid");
    }
    ta->safety_counts_min = safety_counts_min;
    ta->safety_counts_max = safety_counts_max;
    ta->tare_counts = tare_counts;
    ta->trigger_grams = trigger_grams;
    ta->trigger_grams_fixed = trigger_grams << FIXEDQ16_FRAC_BITS;
    ta->grams_per_count = grams_per_count;
}

// Create a trigger_analog
void
command_config_trigger_analog(uint32_t *args)
{
    struct trigger_analog *ta = oid_alloc(
        args[0], command_config_trigger_analog, sizeof(*ta));
    ta->flags = 0;
    ta->trigger_ticks = 0;
    ta->watchdog_max = 0;
    ta->watchdog_count = 0;
    ta->sf = sos_filter_oid_lookup(args[1]);
    set_endstop_range(ta, 0, 0, 0, 0, 0);
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
    set_endstop_range(ta, args[1], args[2], args[3], args[4]
                      , (fixedQ16_t)args[5]);
}
DECL_COMMAND(command_trigger_analog_set_range, "trigger_analog_set_range"
    " oid=%c safety_counts_min=%i safety_counts_max=%i tare_counts=%i"
    " trigger_grams=%u grams_per_count=%i");

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
