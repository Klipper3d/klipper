#ifndef __LOAD_CELL_ENDSTOP_H
#define __LOAD_CELL_ENDSTOP_H

#include <stdint.h> // uint8_t

struct load_cell_endstop *load_cell_endstop_oid_lookup(uint8_t oid);
void load_cell_endstop_report_sample(struct load_cell_endstop *lce
                        , int32_t sample, uint32_t ticks);

#endif // load_cell_endstop.h
