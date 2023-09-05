// Very basic support via a Linux gpio with wiringPi.
//
// Copyright (C) 2023  Steve Jeong <steve@how2flow.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <fcntl.h> // open
#include <stdio.h> // snprintf
#include <stdlib.h> // atexit
#include <string.h> // memset
#include <sys/ioctl.h> // ioctl
#include <unistd.h> // close
#include </usr/include/linux/gpio.h> //GPIO_GET_CHIPINFO_IOCTL
#include "command.h" // shutdown
#include "gpio.h" // gpio_out_write
#include "internal.h" // report_errno
#include "sched.h" // sched_shutdown
#include <wiringPi.h>

#define GPIO_CONSUMER "klipper"
#define CHIP_OFFSET 0 // If gpiochip0/gpio0 is gpio210, value is 210.

DECL_ENUMERATION_RANGE("pin", "gpio0", GPIO(0, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip0/gpio0", GPIO(0, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip1/gpio0", GPIO(1, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip2/gpio0", GPIO(2, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip3/gpio0", GPIO(3, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip4/gpio0", GPIO(4, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip5/gpio0", GPIO(5, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip6/gpio0", GPIO(6, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip7/gpio0", GPIO(7, 0), MAX_GPIO_LINES);
DECL_ENUMERATION_RANGE("pin", "gpiochip8/gpio0", GPIO(8, 0), MAX_GPIO_LINES);

struct gpio_line {
    int chipid;
    int offset;
    int wpi;
};
static struct gpio_line gpio_lines[9 * MAX_GPIO_LINES];
static int gpio_chip_fd[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
struct gpiochip_info info;

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
    memset(&info, 0, sizeof(info));
    int rv = ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, &info);
    if (rv < 0) {
        report_errno("chip info open", rv);
        shutdown("Unable to open GPIO chip info");
    }
    int bank = info.lines;
    int i, j, gpio;
    for (i=0; i<MAX_GPIO_LINES; i++) {
        gpio_lines[GPIO(chipId, i)].offset = i;
        gpio_lines[GPIO(chipId, i)].chipid = chipId;
        gpio_lines[GPIO(chipId, i)].wpi = -1;
        /* get wiringPi pin number */
        gpio = CHIP_OFFSET + (chipId * bank) + i;
        for (j=0; j<64; j++) {
            if (gpio == wpiPinToGpio(j)) {
                gpio_lines[GPIO(chipId, i)].wpi = j;
                break;
            }
        }
    }
    close(rv);
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

__attribute__ ((unused))static void
gpio_release_line(struct gpio_line *line)
{
    /* Dummy function */
    return;
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    get_chip_fd(g.line->chipid);
    if (getAlt(g.line->wpi) != OUTPUT)
        pinMode(g.line->wpi, OUTPUT);
    digitalWrite(g.line->wpi, !!val);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    digitalWrite(g.line->wpi, !!val);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    gpio_out_write(g, !digitalRead(g.line->wpi));
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
    get_chip_fd(g.line->chipid);
    if (getAlt(g.line->wpi) != INPUT) {
        if (pull_up > 0) {
            pinMode(g.line->wpi, INPUT_PULLUP);
        } else if (pull_up < 0) {
            pinMode(g.line->wpi, INPUT_PULLDOWN);
        }
    }
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    return digitalRead(g.line->wpi);
}
