// Efficient support via direct memory access for Raspberry Pi 2/3/4
//
// Copyright (C) 2021 Klaus Schwartz <klaus@eraga.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#include "autoconf.h"
#include "gpio.h"
#include "sched.h" // shutdown

#include <fcntl.h> // open
#include <stdio.h> // snprintf
#include <string.h> // memset
#include <stdlib.h> // atexit

#include "bcm27xx_gpio.h"


int  mem_fd = -1;
void *gpio_map;

volatile unsigned *gpio;


void
setup_io(void)
{
    if(mem_fd != -1)
        return;

    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
        shutdown("Can't open /dev/mem. Are you running klipper_mcu as root?");
    }

    gpio_map = mmap(
            NULL,
            MMAP_BLOCK_SIZE,
            PROT_READ|PROT_WRITE,
            MAP_SHARED,
            mem_fd,
            GPIO_BASE
    );

    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
        shutdown("mmap error");
        exit(-1);
    }

    gpio = (volatile unsigned *)gpio_map;
}

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
gpio_out_write(struct gpio_out g, uint8_t val)
{
    if(val) {
        GPIO_SET = 1 << g.line->offset;
    } else {
        GPIO_CLR = 1 << g.line->offset;
    }
    g.line->state = !!val;
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    setup_io();
    gpio_claim_line(g.line, val, GPIOHANDLE_REQUEST_OUTPUT);

    INP_GPIO(g.line->offset);
    OUT_GPIO(g.line->offset);

    gpio_out_write(g, val);
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
    setup_io();
    gpio_claim_line(g.line, -1, GPIOHANDLE_REQUEST_INPUT);

    int pud = 0;
    if (pull_up > 0) {
        pud = 2;
    } else if (pull_up < 0) {
        pud = 1;
    }

#if CONFIG_MACH_LINUX_BCM2709 == 1
    // Pi 2/3 pull up/down method
    GPIO_PULL   = pud & 3 ;
    usleep(5);
    GPIO_PULLCLK(g.line->offset) = 1 << (g.line->offset & 31) ;
    usleep(5);

    GPIO_PULL              = 0 ;
    usleep(5) ;
    GPIO_PULLCLK(g.line->offset) = 0 ;
    usleep(5) ;
#elif CONFIG_MACH_LINUX_BCM2711 == 1
    //TODO test
    // Pi 4B pull up/down method
    int pullreg = 57 + (g.line->offset>>4);
    int pullshift = (g.line->offset & 0xf) << 1;
    unsigned int pullbits;

    pullbits = *(gpio + pullreg);
    pullbits &= ~(3 << pullshift);
    pullbits |= (pud << pullshift);
    *(gpio + pullreg) = pullbits;
#else
    error("MACH_LINUX_BCM should be defined");
#endif
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    if (GET_GPIO(g.line->offset) != 0)
        return 1;

    return 0;
}
