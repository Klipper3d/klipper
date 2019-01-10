#ifndef __SAM3_INTERNAL_H
#define __SAM3_INTERNAL_H
// Local definitions for sam3/sam4 code

#include <stdint.h> // uint32_t
#include "autoconf.h" // CONFIG_MACH_SAM3X8E

#if CONFIG_MACH_SAM3X8E
#include "sam3x8e.h"
#elif CONFIG_MACH_SAM4S8C
#include "sam4s.h"
#elif CONFIG_MACH_SAM4E8E
#include "sam4e.h"
#endif

#define GPIO(PORT, NUM) (((PORT)-'A') * 32 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 32)
#define GPIO2BIT(PIN) (1<<((PIN) % 32))

void gpio_peripheral(uint32_t gpio, char ptype, int32_t pull_up);
int is_enabled_pclock(uint32_t id);
void enable_pclock(uint32_t id);

#endif // internal.h
