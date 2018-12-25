#ifndef __SAM4_INTERNAL_H
#define __SAM4_INTERNAL_H
// Local definitions for sam4 code

#include <stdint.h> // uint32_t

#define GPIO(PORT, NUM) (((PORT)-'A') * 32 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 32)
#define GPIO2BIT(PIN) (1<<((PIN) % 32))

void gpio_set_peripheral(char bank, uint32_t bit, char ptype, uint32_t pull_up);

#endif // internal.h
