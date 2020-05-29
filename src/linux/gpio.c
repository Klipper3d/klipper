// Very basic support via a Linux gpiod device
//
// Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#include "autoconf.h"
#include "gpio.h"
#include "command.h" // shutdown
#include "sched.h" // shutdown

#include <fcntl.h> // open
#include <stdio.h> // snprintf
#include <string.h> // memset
#include <stdlib.h> // atexit

#include "internal.h" // report_errno

#include </usr/include/linux/gpio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define CHIP_FILE_NAME "/dev/gpiochip%u"
#define GPIO_CONSUMER "klipper"


DECL_ENUMERATION_RANGE("pin", "P0.0", GPIO(0, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "P1.0", GPIO(1, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "P2.0", GPIO(2, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "P3.0", GPIO(3, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "P4.0", GPIO(4, 0), MAX_GPIO_LINES);

struct gpio_line {
    int offset;
    int fd;
    int state;
};

static struct gpio_line lines[TOTAL_GPIO_LINES];

static int gpio_chip_fd[MAX_CHIP_NUMBER] = { -1 };

static int
get_chip_fd(uint8_t chipId) {
    int i = 0;
    if (gpio_chip_fd[chipId] <= 0) {
        char *chipFilename = malloc(sizeof(CHIP_FILE_NAME)+1);
        sprintf(chipFilename, CHIP_FILE_NAME, chipId);
        gpio_chip_fd[chipId] = open(chipFilename,O_RDWR | O_CLOEXEC);
        if (gpio_chip_fd[chipId] < 0) {
            char *errorMessage = malloc(sizeof("Unable to open GPIO" CHIP_FILE_NAME));
            sprintf(errorMessage,"Unable to open GPIO %s",chipFilename);
            report_errno(errorMessage,-1);
            shutdown("Unable to open GPIO chip device");
        }
        for (i=0; i<MAX_GPIO_LINES; ++i) {
            lines[GPIO(chipId,i)].offset = i;
            lines[GPIO(chipId,i)].fd = -1;
        }
    }
    return gpio_chip_fd[chipId];
}

// Dummy versions of gpio_out functions
struct gpio_out
gpio_out_setup(uint32_t pin, uint8_t val)
{
    struct gpio_line* line = &lines[pin];
//    line->offset = pin;
    struct gpio_out g = { .line = line };
    gpio_out_reset(g,val);
    return g;
}

static void
gpio_release_line(struct gpio_line* line)
{
    if (line->fd > 0) {
        close(line->fd);
        line->fd = -1;
    }
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    int rv;
    struct gpiohandle_request req;
    int chipId = get_chip_fd(GPIO2PORT(g.line->offset));
    gpio_release_line(g.line);
    memset(&req, 0, sizeof(req));
    req.lines = 1;
    req.flags = GPIOHANDLE_REQUEST_OUTPUT;
    req.lineoffsets[0] = g.line->offset;
    req.default_values[0] = !!val;
    strncpy(req.consumer_label,GPIO_CONSUMER,sizeof(req.consumer_label) - 1);
    rv = ioctl(chipId, GPIO_GET_LINEHANDLE_IOCTL, &req);
    if (rv < 0) {
        report_errno("gpio_out_reset get line",rv);
        shutdown("Unable to open out GPIO chip line");
    }
    set_close_on_exec(req.fd);
    g.line->fd = req.fd;
    g.line->state = !!val;
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
//    line->offset = pin;
    struct gpio_in g = { .line = line };
    gpio_in_reset(g,pull_up);
    return g;
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    int rv;    
    struct gpiohandle_request req;   
    gpio_release_line(g.line);
    memset(&req, 0, sizeof(req));
    req.lines = 1;
    req.flags = GPIOHANDLE_REQUEST_INPUT;
#if defined(GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP)
    if (pull_up > 0) {
        req.flags |= GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP;
    } else if (pull_up < 0) {
        req.flags |= GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_DOWN;
    }
#endif
    req.lineoffsets[0] = g.line->offset;
    strncpy(req.consumer_label,GPIO_CONSUMER,sizeof(req.consumer_label) - 1);
    rv = ioctl(get_chip_fd(GPIO2PORT(g.line->offset)), GPIO_GET_LINEHANDLE_IOCTL, &req);
    if (rv < 0) {
        report_errno("gpio_in_reset get line",rv);
        shutdown("Unable to open in GPIO chip line");
    }
    set_close_on_exec(req.fd);
    g.line->fd = req.fd;
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    struct gpiohandle_data data;
    memset(&data, 0, sizeof(data));
    ioctl(g.line->fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
    return data.values[0];
}
