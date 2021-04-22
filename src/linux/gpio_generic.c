// Very basic support via a Linux gpiod device
//
// Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#include "autoconf.h"
#include "gpio.h"
#include "sched.h" // shutdown

#include <fcntl.h> // open
#include <stdio.h> // snprintf
#include <string.h> // memset
#include <stdlib.h> // atexit

#include "gpio_commons.h"

struct gpio_out
gpio_out_setup(uint32_t pin, uint8_t val)
{
    struct gpio_line* line = &lines[pin];
    line->offset = GPIO2PIN(pin);
    line->chipid = GPIO2PORT(pin);
    struct gpio_out g = { .line = line };
    gpio_out_reset(g,val);
    return g;
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    gpio_claim_line(g.line, val, GPIOHANDLE_REQUEST_OUTPUT);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    struct gpiohandle_data data;
    memset(&data, 0, sizeof(data));
    data.values[0] = !!val;
    ioctl(g.line->fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
    g.line->state = !!val;
}

void
gpio_out_toggle(struct gpio_out g)
{
    gpio_out_write(g,!g.line->state);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    gpio_out_toggle(g);
}

struct gpio_in
gpio_in_setup(uint32_t pin, int8_t pull_up)
{
    struct gpio_line* line = &lines[pin];
    line->offset = GPIO2PIN(pin);
    line->chipid = GPIO2PORT(pin);
    struct gpio_in g = { .line = line };
    gpio_in_reset(g,pull_up);
    return g;
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    __u32 flags = GPIOHANDLE_REQUEST_INPUT;
#if defined(GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP)
    if (pull_up > 0) {
        flags |= GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP;
    } else if (pull_up < 0) {
        flags |= GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_DOWN;
    }
#endif

    gpio_claim_line(g.line, -1, flags);
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    struct gpiohandle_data data;
    memset(&data, 0, sizeof(data));
    ioctl(g.line->fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
    return data.values[0];
}
