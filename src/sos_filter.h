#ifndef __SOS_FILTER_H
#define __SOS_FILTER_H

#include <stdint.h>

struct sos_filter;

int32_t sosfilt(struct sos_filter *sf
                        , const int32_t unfiltered_value);
struct sos_filter *sos_filter_oid_lookup(uint8_t oid);

#endif // sos_filter.h
