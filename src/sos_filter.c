// Second Order sections Filter implementation using Fixed Point math
//
// Copyright (C) 2025  Gareth Farrington <gareth@waves.ky>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // shutdown
#include "sos_filter.h" // sos_filter

typedef int32_t fixedQ_coeff_t;
typedef int32_t fixedQ_value_t;

// filter strucutre sizes
#define SECTION_WIDTH 5
#define STATE_WIDTH 2

struct sos_filter_section {
    // filter composed of second order sections
    fixedQ_coeff_t coeff[SECTION_WIDTH]; // aka sos
    fixedQ_value_t state[STATE_WIDTH];   // aka zi
};

struct sos_filter {
    uint8_t max_sections, n_sections, coeff_frac_bits, is_active;
    uint32_t coeff_rounding;
    // filter composed of second order sections
    struct sos_filter_section filter[0];
};

inline uint8_t
overflows_int32(int64_t value) {
    return value > (int64_t)INT32_MAX || value < (int64_t)INT32_MIN;
}

// Multiply a coefficient in fixedQ_coeff_t by a value fixedQ_value_t
static inline fixedQ_value_t
fixed_mul(struct sos_filter *sf, const fixedQ_coeff_t coeff
        , const fixedQ_value_t value) {
    // This optimizes to single cycle SMULL on Arm Coretex M0+
    int64_t product = (int64_t)coeff * (int64_t)value;
    // round up at the last bit to be shifted away
    product += sf->coeff_rounding;
    // shift the decimal right to discard the coefficient fractional bits
    int64_t result = product >> sf->coeff_frac_bits;
    // check for overflow of int32_t
    if (overflows_int32(result)) {
        shutdown("fixed_mul: overflow");
    }
    // truncate significant 32 bits
    return (fixedQ_value_t)result;
}

// Apply the sosfilt algorithm to a new datapoint
// returns the fixedQ_value_t filtered value
int32_t
sosfilt(struct sos_filter *sf, const int32_t unfiltered_value) {
    if (!sf->is_active) {
        shutdown("sos_filter not property initialized");
    }

    // an empty filter performs no filtering
    if (sf->n_sections == 0) {
        return unfiltered_value;
    }

    fixedQ_value_t cur_val = unfiltered_value;
    // foreach section
    for (int section_idx = 0; section_idx < sf->n_sections; section_idx++) {
        struct sos_filter_section *section = &(sf->filter[section_idx]);
        // apply the section's filter coefficients to input
        fixedQ_value_t next_val = fixed_mul(sf, section->coeff[0], cur_val);
        next_val += section->state[0];
        section->state[0] = fixed_mul(sf, section->coeff[1], cur_val)
                            - fixed_mul(sf, section->coeff[3], next_val)
                            + (section->state[1]);
        section->state[1] = fixed_mul(sf, section->coeff[2], cur_val)
                            - fixed_mul(sf, section->coeff[4], next_val);
        cur_val = next_val;
    }

    return (int32_t)cur_val;
}

// Create an sos_filter
void
command_config_sos_filter(uint32_t *args)
{
    uint8_t max_sections = args[1];
    uint32_t size = offsetof(struct sos_filter, filter[max_sections]);
    struct sos_filter *sf = oid_alloc(args[0]
                            , command_config_sos_filter, size);
    sf->max_sections = max_sections;
    sf->is_active = 0;
}
DECL_COMMAND(command_config_sos_filter, "config_sos_filter oid=%c"
    " max_sections=%c");

// Lookup an sos_filter
struct sos_filter *
sos_filter_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_sos_filter);
}

// Check that a section index parameter is valid
static void
validate_section_index(struct sos_filter *sf, uint8_t section_idx)
{
    if (section_idx > sf->max_sections)
        shutdown("Filter section index larger than max_sections");
}

// Set one section of the filter
void
command_sos_filter_set_section(uint32_t *args)
{
    struct sos_filter *sf = sos_filter_oid_lookup(args[0]);
    // setting a section marks the filter as inactive
    sf->is_active = 0;
    uint8_t section_idx = args[1];
    validate_section_index(sf, section_idx);
    // copy section data
    const uint8_t arg_base = 2;
    for (uint8_t i = 0; i < SECTION_WIDTH; i++) {
        sf->filter[section_idx].coeff[i] = args[i + arg_base];
    }
}
DECL_COMMAND(command_sos_filter_set_section
    , "sos_filter_set_section oid=%c section_idx=%c"
    " sos0=%i sos1=%i sos2=%i sos3=%i sos4=%i");

// Set the state of one section of the filter
void
command_sos_filter_set_state(uint32_t *args)
{
    struct sos_filter *sf = sos_filter_oid_lookup(args[0]);
    // setting a section's state marks the filter as inactive
    sf->is_active = 0;
    // copy state data
    uint8_t section_idx = args[1];
    validate_section_index(sf, section_idx);
    const uint8_t arg_base = 2;
    sf->filter[section_idx].state[0] = args[0 + arg_base];
    sf->filter[section_idx].state[1] = args[1 + arg_base];
}
DECL_COMMAND(command_sos_filter_set_state
    , "sos_filter_set_state oid=%c section_idx=%c state0=%i state1=%i");

// Set one section of the filter
void
command_sos_filter_activate(uint32_t *args)
{
    struct sos_filter *sf = sos_filter_oid_lookup(args[0]);
    uint8_t n_sections = args[1];
    validate_section_index(sf, n_sections);
    sf->n_sections = n_sections;
    const uint8_t coeff_int_bits = args[2];
    sf->coeff_frac_bits = (31 - coeff_int_bits);
    sf->coeff_rounding  = (1 << (sf->coeff_frac_bits - 1));
    // mark filter as ready to use
    sf->is_active = 1;
}
DECL_COMMAND(command_sos_filter_activate
    , "sos_filter_set_active oid=%c n_sections=%c coeff_int_bits=%c");
