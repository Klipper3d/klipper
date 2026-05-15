#ifndef __AVR_HC595_H
#define __AVR_HC595_H
// SNx4HC595 shift register support for AVR

#include "autoconf.h" // CONFIG_HC595_LENGTH
#include <stdint.h> // uint8_t
#include "gpio.h" // gpio_out

struct hc595 {
    struct gpio_out data_pin, latch_pin, clock_pin;
    uint8_t val[CONFIG_HC595_LENGTH];
};

extern struct hc595 *hc595_sr;

void hc595_set_bit(uint8_t bit, uint8_t value);
void hc595_toggle_bit(uint8_t bit);
void hc595_flush(void);

#endif // hc595.h
