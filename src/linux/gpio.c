// Very basic support via a Linux gpiod device
//
// Copyright (C) 2017-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <fcntl.h> // open
#include <stdio.h> // snprintf
#include <stdlib.h> // atexit
#include <string.h> // memset
#include <sys/ioctl.h> // ioctl
#include <unistd.h> // close
#include </usr/include/linux/gpio.h> // GPIOHANDLE_REQUEST_OUTPUT
#include "command.h" // shutdown
#include "gpio.h" // gpio_out_write
#include "internal.h" // report_errno
#include "sched.h" // sched_shutdown

#define GPIO_CONSUMER "klipper"

DECL_ENUMERATION_RANGE("pin", "gpio0", GPIO(0, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip0/gpio0", GPIO(0, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip1/gpio0", GPIO(1, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip2/gpio0", GPIO(2, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip3/gpio0", GPIO(3, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip4/gpio0", GPIO(4, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip5/gpio0", GPIO(5, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip6/gpio0", GPIO(6, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip7/gpio0", GPIO(7, 0), MAX_GPIO_LINES);

struct gpio_line {
    int chipid;
    int offset;
    int fd;
    int state;
};
static struct gpio_line gpio_lines[8 * MAX_GPIO_LINES];
static int gpio_chip_fd[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

static int
get_chip_fd(uint8_t chipId)
{
    if (gpio_chip_fd[chipId] >= 0)
        return gpio_chip_fd[chipId];
    char chipFilename[64];
    snprintf(chipFilename, sizeof(chipFilename), "/dev/gpiochip%u", chipId);
    int ret = access(chipFilename, F_OK);
    if (ret < 0) {
        report_errno("gpio access", ret);
        shutdown("GPIO chip device not found");
    }
    int fd = open(chipFilename, O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        report_errno("gpio open", fd);
        shutdown("Unable to open GPIO chip device");
    }
    gpio_chip_fd[chipId] = fd;
    int i;
    for (i=0; i<MAX_GPIO_LINES; i++) {
        gpio_lines[GPIO(chipId, i)].offset = i;
        gpio_lines[GPIO(chipId, i)].fd = -1;
        gpio_lines[GPIO(chipId, i)].chipid = chipId;
    }
    return fd;
}

struct gpio_out
gpio_out_setup(uint32_t pin, uint8_t val)
{
    struct gpio_line *line = &gpio_lines[pin];
    line->offset = GPIO2PIN(pin);
    line->chipid = GPIO2PORT(pin);
    struct gpio_out g = { .line = line };
    gpio_out_reset(g,val);
    return g;
}

static void
gpio_release_line(struct gpio_line *line)
{
    if (line->fd > 0) {
        close(line->fd);
        line->fd = -1;
    }
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    gpio_release_line(g.line);
    struct gpiohandle_request req;
    memset(&req, 0, sizeof(req));
    req.lines = 1;
    req.flags = GPIOHANDLE_REQUEST_OUTPUT;
    req.lineoffsets[0] = g.line->offset;
    req.default_values[0] = !!val;
    strncpy(req.consumer_label, GPIO_CONSUMER, sizeof(req.consumer_label) - 1);
    int fd = get_chip_fd(g.line->chipid);
    int ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
    if (ret < 0) {
        report_errno("gpio_out_reset get line", ret);
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
gpio_out_toggle_noirq(struct gpio_out g)
{
    gpio_out_write(g, !g.line->state);
}

void
gpio_out_toggle(struct gpio_out g)
{
    gpio_out_toggle_noirq(g);
}

struct gpio_in
gpio_in_setup(uint32_t pin, int8_t pull_up)
{
    struct gpio_line *line = &gpio_lines[pin];
    line->offset = GPIO2PIN(pin);
    line->chipid = GPIO2PORT(pin);
    struct gpio_in g = { .line = line };
    gpio_in_reset(g, pull_up);
    return g;
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    gpio_release_line(g.line);
    struct gpiohandle_request req;
    memset(&req, 0, sizeof(req));
    req.lines = 1;
    req.flags = GPIOHANDLE_REQUEST_INPUT;
#if defined(GPIOHANDLE_REQUEST_BIAS_PULL_UP)
    if (pull_up > 0) {
        req.flags |= GPIOHANDLE_REQUEST_BIAS_PULL_UP;
    } else if (pull_up < 0) {
        req.flags |= GPIOHANDLE_REQUEST_BIAS_PULL_DOWN;
    }
#endif
    req.lineoffsets[0] = g.line->offset;
    strncpy(req.consumer_label, GPIO_CONSUMER, sizeof(req.consumer_label) - 1);
    int fd = get_chip_fd(g.line->chipid);
    int ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
    if (ret < 0) {
        report_errno("gpio_in_reset get line", ret);
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
