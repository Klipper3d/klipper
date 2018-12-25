#ifndef __SAM3_INTERNAL_H
#define __SAM3_INTERNAL_H
// Local definitions for sam3 code

#include <stdint.h> // uint32_t

#define GPIO(PORT, NUM) (((PORT)-'A') * 32 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 32)
#define GPIO2BIT(PIN) (1<<((PIN) % 32))

void gpio_peripheral(uint32_t gpio, char ptype, int32_t pull_up);

#endif // internal.h
