// Load Cell based end stops.
//
// Copyright (C) 2025  Gareth Farrington <gareth@waves.ky>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // shutdown
#include "trsync.h" // trsync_do_trigger
#include "board/misc.h" // timer_read_time
#include "sos_filter.h" // fixedQ12_t
#include "load_cell_probe.h" //load_cell_probe_report_sample
#include <stdint.h> // int32_t
#include <stdlib.h> // abs

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

#define MAX_TRIGGER_GRAMS ((1 << FIXEDQ16) - 1)
#define ERROR_SAFETY_RANGE 0
#define ERROR_OVERFLOW 1
#define ERROR_WATCHDOG 2

// Flags
enum {FLAG_IS_HOMING = 1 << 0
    , FLAG_IS_HOMING_TRIGGER = 1 << 1
    , FLAG_AWAIT_HOMING = 1 << 2
    };

// Endstop Structure
struct load_cell_probe {
    struct timer time;
    uint32_t trigger_grams, trigger_ticks, last_sample_ticks, rest_ticks;
    uint32_t homing_start_time;
    struct trsync *ts;
    int32_t safety_counts_min, safety_counts_max, tare_counts;
    uint8_t flags, trigger_reason, error_reason, watchdog_max
            , watchdog_count;
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
counts_to_grams(struct load_cell_probe *lce, const int32_t counts) {
    // tearing ensures readings are referenced to 0.0g
    const int32_t delta = counts - lce->tare_counts;
    // convert sensor counts to grams by multiplication: 124 * 0.051 = 6.324
    // this optimizes to single cycle SMULL instruction
    const fixedQ32_t product = (int64_t)delta * (int64_t)lce->grams_per_count;
    // after multiplication there are 30 fraction bits, reduce to 15
    // caller verifies this wont overflow a 32bit int when truncated
    const fixedQ48_t grams = product >>
                                (FIXEDQ32_FRAC_BITS - FIXEDQ48_FRAC_BITS);
    return grams;
}

static inline uint8_t
is_flag_set(const uint8_t mask, struct load_cell_probe *lce)
{
    return !!(mask & lce->flags);
}

static inline void
set_flag(uint8_t mask, struct load_cell_probe *lce)
{
    lce->flags |= mask;
}

static inline void
clear_flag(uint8_t mask, struct load_cell_probe *lce)
{
    lce->flags &= ~mask;
}

void
try_trigger(struct load_cell_probe *lce, uint32_t ticks)
{
    uint8_t is_homing_triggered = is_flag_set(FLAG_IS_HOMING_TRIGGER, lce);
    if (!is_homing_triggered) {
        // the first triggering sample when homing sets the trigger time
        lce->trigger_ticks = ticks;
        // this flag latches until a reset, disabling further triggering
        set_flag(FLAG_IS_HOMING_TRIGGER, lce);
        trsync_do_trigger(lce->ts, lce->trigger_reason);
    }
}

void
trigger_error(struct load_cell_probe *lce, uint8_t error_code)
{
    trsync_do_trigger(lce->ts, lce->error_reason + error_code);
}

// Used by Sensors to report new raw ADC sample
void
load_cell_probe_report_sample(struct load_cell_probe *lce
                                , const int32_t sample)
{
    // only process samples when homing
    uint8_t is_homing = is_flag_set(FLAG_IS_HOMING, lce);
    if (!is_homing) {
        return;
    }

    // save new sample
    uint32_t ticks = timer_read_time();
    lce->last_sample_ticks = ticks;
    lce->watchdog_count = 0;

    // do not trigger before homing start time
    uint8_t await_homing = is_flag_set(FLAG_AWAIT_HOMING, lce);
    if (await_homing && timer_is_before(ticks, lce->homing_start_time)) {
        return;
    }
    clear_flag(FLAG_AWAIT_HOMING, lce);

    // check for safety limit violations
    const uint8_t is_safety_trigger = sample <= lce->safety_counts_min
                                        || sample >= lce->safety_counts_max;
    // too much force, this is an error while homing
    if (is_safety_trigger) {
        trigger_error(lce, ERROR_SAFETY_RANGE);
        return;
    }

    // convert sample to grams
    const fixedQ48_t raw_grams = counts_to_grams(lce, sample);
    if (overflows_int32(raw_grams)) {
        trigger_error(lce, ERROR_OVERFLOW);
        return;
    }

    // perform filtering
    const fixedQ16_t filtered_grams = sosfilt(lce->sf, (fixedQ16_t)raw_grams);

    // update trigger state
    if (abs(filtered_grams) >= lce->trigger_grams_fixed) {
        try_trigger(lce, lce->last_sample_ticks);
    }
}

// Timer callback that monitors for timeouts
static uint_fast8_t
watchdog_event(struct timer *t)
{
    struct load_cell_probe *lce = container_of(t, struct load_cell_probe
                                        , time);
    uint8_t is_homing = is_flag_set(FLAG_IS_HOMING, lce);
    uint8_t is_homing_trigger = is_flag_set(FLAG_IS_HOMING_TRIGGER, lce);
    // the watchdog stops when not homing or when trsync becomes triggered
    if (!is_homing || is_homing_trigger) {
        return SF_DONE;
    }

    if (lce->watchdog_count > lce->watchdog_max) {
        trigger_error(lce, ERROR_WATCHDOG);
    }
    lce->watchdog_count += 1;

    // A sample was recently delivered, continue monitoring
    lce->time.waketime += lce->rest_ticks;
    return SF_RESCHEDULE;
}

static void
set_endstop_range(struct load_cell_probe *lce
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
    const fixedQ2_t one = 1 << FIXEDQ2_FRAC_BITS;
    if (grams_per_count < 0 || grams_per_count >= one) {
        shutdown("grams_per_count is invalid");
    }
    lce->safety_counts_min = safety_counts_min;
    lce->safety_counts_max = safety_counts_max;
    lce->tare_counts = tare_counts;
    lce->trigger_grams = trigger_grams;
    lce->trigger_grams_fixed = trigger_grams << FIXEDQ16_FRAC_BITS;
    lce->grams_per_count = grams_per_count;
}

// Create a load_cell_probe
void
command_config_load_cell_probe(uint32_t *args)
{
    struct load_cell_probe *lce = oid_alloc(args[0]
                            , command_config_load_cell_probe, sizeof(*lce));
    lce->flags = 0;
    lce->trigger_ticks = 0;
    lce->watchdog_max = 0;
    lce->watchdog_count = 0;
    lce->sf = sos_filter_oid_lookup(args[1]);
    set_endstop_range(lce, 0, 0, 0, 0, 0);
}
DECL_COMMAND(command_config_load_cell_probe, "config_load_cell_probe"
                                               " oid=%c sos_filter_oid=%c");

// Lookup a load_cell_probe
struct load_cell_probe *
load_cell_probe_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_load_cell_probe);
}

// Set the triggering range and tare value
void
command_load_cell_probe_set_range(uint32_t *args)
{
    struct load_cell_probe *lce = load_cell_probe_oid_lookup(args[0]);
    set_endstop_range(lce, args[1], args[2], args[3], args[4]
                , (fixedQ16_t)args[5]);
}
DECL_COMMAND(command_load_cell_probe_set_range, "load_cell_probe_set_range"
    " oid=%c safety_counts_min=%i safety_counts_max=%i tare_counts=%i"
    " trigger_grams=%u grams_per_count=%i");

// Home an axis
void
command_load_cell_probe_home(uint32_t *args)
{
    struct load_cell_probe *lce = load_cell_probe_oid_lookup(args[0]);
    sched_del_timer(&lce->time);
    // clear the homing trigger flag
    clear_flag(FLAG_IS_HOMING_TRIGGER, lce);
    clear_flag(FLAG_IS_HOMING, lce);
    lce->trigger_ticks = 0;
    lce->ts = NULL;
    // 0 samples indicates homing is finished
    if (args[3] == 0) {
        // Disable end stop checking
        return;
    }
    lce->ts = trsync_oid_lookup(args[1]);
    lce->trigger_reason = args[2];
    lce->error_reason = args[3];
    lce->time.waketime = args[4];
    lce->homing_start_time = args[4];
    lce->rest_ticks = args[5];
    lce->watchdog_max = args[6];
    lce->watchdog_count = 0;
    lce->time.func = watchdog_event;
    set_flag(FLAG_IS_HOMING, lce);
    set_flag(FLAG_AWAIT_HOMING, lce);
    sched_add_timer(&lce->time);
}
DECL_COMMAND(command_load_cell_probe_home,
             "load_cell_probe_home oid=%c trsync_oid=%c trigger_reason=%c"
             " error_reason=%c clock=%u rest_ticks=%u timeout=%u");

void
command_load_cell_probe_query_state(uint32_t *args)
{
    uint8_t oid = args[0];
    struct load_cell_probe *lce = load_cell_probe_oid_lookup(args[0]);
    sendf("load_cell_probe_state oid=%c is_homing_trigger=%c trigger_ticks=%u"
            , oid
            , is_flag_set(FLAG_IS_HOMING_TRIGGER, lce)
            , lce->trigger_ticks);
}
DECL_COMMAND(command_load_cell_probe_query_state
                , "load_cell_probe_query_state oid=%c");
