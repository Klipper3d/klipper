#ifndef __LOAD_CELL_PROBE_H
#define __LOAD_CELL_PROBE_H

#include <stdint.h> // uint8_t

struct load_cell_probe *load_cell_probe_oid_lookup(uint8_t oid);
void load_cell_probe_report_sample(struct load_cell_probe *lce
                        , int32_t sample);

#endif // load_cell_probe.h
