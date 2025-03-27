// Second Order sections Filter implementation using Fixed Point math
//
// Copyright (C) 2020-2025 Gareth Farrington <gareth@waves.ky>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // shutdown
#include "sos_filter.h" // sos_filter


inline uint8_t
overflows_int32(int64_t value) {
    return value > (int64_t)INT32_MAX || value < (int64_t)INT32_MIN;
}

// Multiply a coefficient in fixedQ1_t by a value fixedQ16_t
static inline fixedQ16_t
fixed_mul(const fixedQ1_t coeff, const fixedQ16_t value) {
    // This optimizes to single cycle SMULL on Arm Coretex M0+
    // product is Q18.45
    int64_t product = (int64_t)coeff * (int64_t)value;
    // round up at the last bit to be shifted away
    product += FIXEDQ1_ROUNDING;
    // shift Q18.45 down to Q48.15
    fixedQ48_t result = (product) >> FIXEDQ1_FRAC_BITS;
    // check for overflow of int32_t
    if (overflows_int32(result)) {
        shutdown("fixed_mul: overflow");
    }
    // truncate significant 32 bits
    return (fixedQ16_t)result;
}

// Apply the sosfilt algorithm to a new datapoint
// returns the fixedQ16_t filtered value
fixedQ16_t
sosfilt(struct sos_filter *sf, const fixedQ16_t unfiltered_value) {
    if (!sf->is_filter_ready) {
        shutdown("sos_filter not initialized");
    }

    // an empty filter performs no filtering
    if (sf->n_sections == 0) {
        return unfiltered_value;
    }

    fixedQ16_t cur_val = unfiltered_value;
    // foreach section
    for (int section = 0; section < sf->n_sections; section++) {
        // apply section's filter coefficients to input
        fixedQ16_t next_val = fixed_mul(sf->filter[section][0], cur_val)
                              + sf->filter_state[section][0];
        sf->filter_state[section][0] =
            fixed_mul(sf->filter[section][1], cur_val)
            - fixed_mul(sf->filter[section][3], next_val)
            + (sf->filter_state[section][1]);
        sf->filter_state[section][1] =
            fixed_mul(sf->filter[section][2], cur_val)
            - fixed_mul(sf->filter[section][4], next_val);
        cur_val = next_val;
    }

    return cur_val;
}


// resets the filter state (zi) to 0 to start a new filtering session
void
reset_filter_state(struct sos_filter *sf) {
    for (uint8_t i = 0; i < MAX_SECTIONS; i++){
        sf->filter_state[i][0] = 0;
        sf->filter_state[i][1] = 0;
    }
}

// mark filter as not ready for use, ready to be updated
void
reset_filter(struct sos_filter *sf, uint8_t n_sections) {
    if (n_sections > MAX_SECTIONS) {
        shutdown("sos_filter: too many filter sections");
    }
    sf->n_sections = n_sections;
    sf->last_section_idx = -1;
    sf->is_filter_ready = 0;
}

// Create an sos_filter
void
command_config_sos_filter(uint32_t *args)
{
    struct sos_filter *sf = oid_alloc(args[0]
                            , command_config_sos_filter, sizeof(*sf));
    reset_filter(sf, 0);
}
DECL_COMMAND(command_config_sos_filter, "config_sos_filter oid=%c");

// Lookup an sos_filter
struct sos_filter *
sos_filter_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_sos_filter);
}

// Set one section of the filter
void
command_config_filter_section(uint32_t *args)
{
    struct sos_filter *sf = sos_filter_oid_lookup(args[0]);
    uint8_t n_sections = args[1];
    uint8_t section_idx = args[2];
    // if this is the first filter section, initalize
    if (section_idx <= 0)  {
        reset_filter(sf, n_sections);
    } else {
        // validate that this new section is in order
        if (n_sections != sf->n_sections) {
            shutdown("Filter size mismatch");
        }
        if (section_idx != sf->last_section_idx + 1) {
            shutdown("Filter section out of order");
        }
    }

    // if the number of configured sections positive
    if (n_sections > 0) {
        // copy section data
        const uint8_t arg_base = 3;
        for (uint8_t i = 0; i < SECTION_WIDTH; i++) {
            sf->filter[section_idx][i] = args[i + arg_base];
        }
        sf->last_section_idx = section_idx;
        /*
        // DEBUG: uncomment to log filter section contents
        for (uint8_t i = 0; i < SECTION_WIDTH; i++) {
            output("section[%c][%c]=%i", section_idx, i,
                                (int32_t)sf->sos[section_idx][i]);
        }*/
    }

    // check for last section
    if (sf->last_section_idx == sf->n_sections - 1) {
        sf->is_filter_ready = 1;
    }
}
DECL_COMMAND(command_config_filter_section
    , "config_sos_filter_section oid=%c n_sections=%c"
    " section_idx=%c sos0=%i sos1=%i sos2=%i sos3=%i sos4=%i");
