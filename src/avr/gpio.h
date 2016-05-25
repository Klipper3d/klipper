#ifndef __AVR_GPIO_H
#define __AVR_GPIO_H

#include <stdint.h>
#include "compiler.h" // __always_inline

struct gpio_out {
    struct gpio_digital_regs *regs;
    // gcc (pre v6) does better optimization when uint8_t are bitfields
    uint8_t bit : 8;
};
struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val);
void gpio_out_toggle(struct gpio_out g);
void gpio_out_write(struct gpio_out g, uint8_t val);

struct gpio_in {
    struct gpio_digital_regs *regs;
    uint8_t bit;
};
struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up);
uint8_t gpio_in_read(struct gpio_in g);

struct gpio_pwm {
    void *reg;
    uint8_t size8;
};
struct gpio_pwm gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val);
void gpio_pwm_write(struct gpio_pwm g, uint8_t val);

struct gpio_adc {
    uint8_t chan;
};
struct gpio_adc gpio_adc_setup(uint8_t pin);
uint32_t gpio_adc_sample_time(void);
uint8_t gpio_adc_sample(struct gpio_adc g);
void gpio_adc_clear_sample(struct gpio_adc g);
uint16_t gpio_adc_read(struct gpio_adc g);

void spi_config(void);
void spi_transfer(char *data, uint8_t len);

#endif // gpio.h
