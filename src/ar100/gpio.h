#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#define R_PIO_BASE  0x01F02C00
#define PIO_BASE    0x01C20800

enum pin_func {
  PIO_INPUT,
  PIO_OUTPUT,
  PIO_ALT1,
  PIO_ALT2,
  PIO_ALT3,
  PIO_ALT4,
  PIO_ALT5,
  PIO_DISABLE
};

struct gpio_mux {
  uint32_t pin;
  uint8_t bank;
  uint32_t reg;
};

struct gpio_out {
  uint8_t pin;
  uint8_t bank;
  uint32_t reg;
};

struct gpio_in {
  uint8_t pin;
  uint8_t bank;
  uint32_t reg;
};

extern volatile uint32_t data_regs[8];


struct gpio_mux gpio_mux_setup(uint8_t pin, enum pin_func func);

struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val);
void gpio_out_write(struct gpio_out pin, uint8_t val);
void gpio_out_reset(struct gpio_out pin, uint8_t val);
static inline __attribute__((always_inline)) void
gpio_out_toggle_noirq(struct gpio_out pin){
  data_regs[pin.bank] ^= (1<<pin.pin);
  *((volatile uint32_t *)(pin.reg)) = data_regs[pin.bank];
}

static inline __attribute__((always_inline)) void
gpio_out_toggle(struct gpio_out pin){
  gpio_out_toggle_noirq(pin);
}

struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up);
void gpio_in_reset(struct gpio_in pin, int8_t pull_up);
uint8_t gpio_in_read(struct gpio_in pin);

#endif
