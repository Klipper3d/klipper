#ifndef __LINUX_GPIO_H
#define __LINUX_GPIO_H

#include <stdint.h> // uint8_t


#define PIO_BASE 0x01C20800
#define PIOB_BASE PIO_BASE + (1*0x24)
#define PIOE_BASE PIO_BASE + (4*0x24)
#define PB_CFG0 PIOB_BASE + 0x0
#define PB_DATA PIOB_BASE + 0x10
#define PE_CFG0 PIOE_BASE + 0x0
#define PE_CFG1 PIOE_BASE + 0x4
#define PE_DATA PIOE_BASE + 0x10


struct gpio_out {
    uint32_t reg;
    uint32_t pin;
    uint32_t val;
};
struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val);
void gpio_out_reset(struct gpio_out g, uint8_t val);
void gpio_out_toggle_noirq(struct gpio_out g);
void gpio_out_toggle(struct gpio_out g);
void gpio_out_write(struct gpio_out g, uint8_t val);


struct gpio_in {
    uint8_t pin;
};
struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up);
void gpio_in_reset(struct gpio_in g, int8_t pull_up);
uint8_t gpio_in_read(struct gpio_in g);


#endif // gpio.h
