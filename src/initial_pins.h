#ifndef __INITIAl_PINS_H
#define __INITIAl_PINS_H

struct initial_pin_s {
    int pin;
    uint8_t flags;
};

enum { IP_OUT_HIGH = 1 };

// out/compile_time_request.c (auto generated file)
extern const struct initial_pin_s initial_pins[];
extern const int initial_pins_size;

#endif // initial_pins.h
