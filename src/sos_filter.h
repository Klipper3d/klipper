#ifndef __SOS_FILTER_H
#define __SOS_FILTER_H

#include <stdint.h>

/*
 * Fix Point Support
 */
// https://en.wikipedia.org/wiki/Q_(number_format)
// fixed point types for 32 bit storage
// Q1.30
typedef int32_t fixedQ1_t;
#define FIXEDQ1 1
#define FIXEDQ1_FRAC_BITS (31 - FIXEDQ1)
#define FIXEDQ1_ROUNDING (1 << (FIXEDQ1_FRAC_BITS - 1))

// Q16.15
typedef int32_t fixedQ16_t;
#define FIXEDQ16 16
#define FIXEDQ16_FRAC_BITS (31 - FIXEDQ16)

// Q48.15
typedef int64_t fixedQ48_t; // Q16.15 value stored in int64
#define FIXEDQ48_FRAC_BITS FIXEDQ16_FRAC_BITS

// filter strucutre sizes
#define MAX_SECTIONS 4
#define SECTION_WIDTH 5
#define STATE_WIDTH 2

struct sos_filter {
    int8_t last_section_idx;
    uint8_t n_sections, is_filter_ready;
    // filter composed of second order sections
    fixedQ1_t filter[MAX_SECTIONS][SECTION_WIDTH];     // aka sos
    fixedQ16_t filter_state[MAX_SECTIONS][STATE_WIDTH]; // aka zi
};

uint8_t overflows_int32(int64_t value);
void reset_filter(struct sos_filter *sf, uint8_t n_sections);
void reset_filter_state(struct sos_filter *sf);
fixedQ16_t sosfilt(struct sos_filter *sf, const fixedQ16_t unfiltered_value);
struct sos_filter *sos_filter_oid_lookup(uint8_t oid);

#endif // sos_filter.h
