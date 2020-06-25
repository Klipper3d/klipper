#ifndef __AVR_INTERNAL_H
#define __AVR_INTERNAL_H
// Local definitions for avr code

#define GPIO(PORT, NUM) (((PORT)-'A') * 8 + (NUM))
#define GPIO2PORT(PIN) ((PIN) / 8)
#define GPIO2BIT(PIN) (1<<((PIN) % 8))

struct gpio_digital_regs {
    // gcc (pre v6) does better optimization when uint8_t are bitfields
    volatile uint8_t in : 8, mode : 8, out : 8;
};
extern volatile uint8_t * const digital_regs[];

#define GPIO2REGS(pin)                                                  \
    ((struct gpio_digital_regs*)READP(digital_regs[GPIO2PORT(pin)]))

#endif // internal.h
