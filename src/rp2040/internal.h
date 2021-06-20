#ifndef __RP2040_INTERNAL_H
#define __RP2040_INTERNAL_H
// Local definitions for rp2040

#include "RP2040.h"

void enable_pclock(uint32_t reset_bit);
int is_enabled_pclock(uint32_t reset_bit);
uint32_t get_pclock_frequency(uint32_t reset_bit);
void gpio_peripheral(uint32_t gpio, int func, int pull_up);

#endif // internal.h
