#ifndef __SOS_FILTER_H
#define __SOS_FILTER_H

#include <stdint.h>

struct sos_filter;
int sos_filter_apply(struct sos_filter *sf, int32_t *pvalue);
struct sos_filter *sos_filter_oid_lookup(uint8_t oid);

#endif // sos_filter.h
