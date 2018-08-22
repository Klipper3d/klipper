#ifndef __LPC176X_GPIO_H
#define __LPC176X_GPIO_H

#include <stdint.h>

struct gpio_out {
    void *regs;
    uint32_t bit;
};
struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val);
void gpio_out_toggle_noirq(struct gpio_out g);
void gpio_out_toggle(struct gpio_out g);
void gpio_out_write(struct gpio_out g, uint8_t val);

struct gpio_in {
    void *regs;
    uint32_t bit;
};
struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up);
uint8_t gpio_in_read(struct gpio_in g);

struct gpio_adc {
    uint32_t cmd;
};
struct gpio_adc gpio_adc_setup(uint8_t pin);
uint32_t gpio_adc_sample(struct gpio_adc g);
uint16_t gpio_adc_read(struct gpio_adc g);
void gpio_adc_cancel_sample(struct gpio_adc g);

struct spi_config {
    uint32_t cr0, cpsr;
};
struct spi_config spi_setup(uint32_t bus, uint8_t mode, uint32_t rate);
void spi_prepare(struct spi_config config);
void spi_transfer(struct spi_config config, uint8_t receive_data
                  , uint8_t len, uint8_t *data);

#endif // gpio.h
