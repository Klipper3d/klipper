#ifndef __AR100_GPIO_H
#define __AR100_GPIO_H

#include <stdint.h>

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


struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up);
void gpio_in_reset(struct gpio_in pin, int8_t pull_up);
uint8_t gpio_in_read(struct gpio_in pin);

struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val);
void gpio_out_write(struct gpio_out pin, uint8_t val);
void gpio_out_reset(struct gpio_out pin, uint8_t val);
void gpio_out_toggle_noirq(struct gpio_out pin);
void gpio_out_toggle(struct gpio_out pin);
struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up);
void gpio_in_reset(struct gpio_in pin, int8_t pull_up);
uint8_t gpio_in_read(struct gpio_in pin);

struct spi_config {
    void *spi;
    uint32_t spi_cr1;
};
struct spi_config spi_setup(uint32_t bus, uint8_t mode, uint32_t rate);
void spi_prepare(struct spi_config config);
void spi_transfer(struct spi_config config, uint8_t receive_data
  , uint8_t len, uint8_t *data);

struct i2c_config {
    void *cfg;
};

#endif
