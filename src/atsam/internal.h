#ifndef __ATSAM_INTERNAL_H
#define __ATSAM_INTERNAL_H
// Local definitions for sam3/sam4 code

#include <stdint.h> // uint32_t
#include "autoconf.h" // CONFIG_MACH_SAM3X

#if CONFIG_MACH_SAM3X
#include "sam3xa.h"
#elif CONFIG_MACH_SAM4S
#include "sam4s.h"
#elif CONFIG_MACH_SAM4E
#include "sam4e.h"
#elif CONFIG_MACH_SAME70
#include "sam.h"
#endif

#define GPIO(PORT, NUM) (((PORT)-'A') * 32 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 32)
#define GPIO2BIT(PIN) (1<<((PIN) % 32))

void gpio_peripheral(uint32_t gpio, char ptype, int32_t pull_up);
int is_enabled_pclock(uint32_t id);
void enable_pclock(uint32_t id);
uint32_t get_pclock_frequency(uint32_t id);

#endif // internal.h
