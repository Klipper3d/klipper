// Raspberry Pi memory-mapped GPIO.
// Based on https://abyz.me.uk/rpi/pigpio/examples.html#Misc_minimal_gpio
//
// Copyright (C) 2017-2021  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2024 Liam Powell <klipper@liampwll.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "gpio.h"
#include "internal.h"
#include "sched.h"

static uint32_t pi_periph_base = 0x20000000;

static int pi_is_2711 = 0;

#define SYST_BASE (pi_periph_base + 0x003000)
#define DMA_BASE (pi_periph_base + 0x007000)
#define CLK_BASE (pi_periph_base + 0x101000)
#define GPIO_BASE (pi_periph_base + 0x200000)
#define UART0_BASE (pi_periph_base + 0x201000)
#define PCM_BASE (pi_periph_base + 0x203000)
#define SPI0_BASE (pi_periph_base + 0x204000)
#define I2C0_BASE (pi_periph_base + 0x205000)
#define PWM_BASE (pi_periph_base + 0x20C000)
#define BSCS_BASE (pi_periph_base + 0x214000)
#define UART1_BASE (pi_periph_base + 0x215000)
#define I2C1_BASE (pi_periph_base + 0x804000)
#define I2C2_BASE (pi_periph_base + 0x805000)
#define DMA15_BASE (pi_periph_base + 0xE05000)

#define DMA_LEN 0x1000 /* allow access to all channels */
#define CLK_LEN 0xA8
#define GPIO_LEN 0xF4
#define SYST_LEN 0x1C
#define PCM_LEN 0x24
#define PWM_LEN 0x28
#define I2C_LEN 0x1C
#define BSCS_LEN 0x40

#define GPSET0 7
#define GPSET1 8

#define GPCLR0 10
#define GPCLR1 11

#define GPLEV0 13
#define GPLEV1 14

#define GPPUD 37
#define GPPUDCLK0 38
#define GPPUDCLK1 39

/* BCM2711 has different pulls */

#define GPPUPPDN0 57
#define GPPUPPDN1 58
#define GPPUPPDN2 59
#define GPPUPPDN3 60

#define SYST_CS 0
#define SYST_CLO 1
#define SYST_CHI 2

static volatile uint32_t *gpio_reg = MAP_FAILED;
static volatile uint32_t *syst_reg = MAP_FAILED;
static volatile uint32_t *bscs_reg = MAP_FAILED;

#define PI_BANK(gpio) (gpio >> 5)
#define PI_BIT(gpio) (1 << (gpio & 0x1F))

/* gpio modes. */

#define PI_INPUT 0
#define PI_OUTPUT 1
#define PI_ALT0 4
#define PI_ALT1 5
#define PI_ALT2 6
#define PI_ALT3 7
#define PI_ALT4 3
#define PI_ALT5 2

static uint8_t gpio_out_states[58] = {0};

static unsigned
gpio_hardware_revision(void)
{
    static unsigned rev = 0;

    FILE *filp;
    char buf[512];
    char term;
    int chars = 4; /* number of chars in revision string */

    filp = fopen("/proc/cpuinfo", "r");

    if (filp != NULL) {
        while (fgets(buf, sizeof(buf), filp) != NULL) {
            if (!strncasecmp("revision", buf, 8)) {
                if (sscanf(buf + strlen(buf) - (chars + 1), "%x%c", &rev, &term) == 2) {
                    if (term != '\n')
                        rev = 0;
                    else
                        rev &= 0xFFFFFF; /* mask out warranty bit */
                }
            }
        }

        fclose(filp);
    }

    if ((filp = fopen("/proc/device-tree/soc/ranges", "rb"))) {
        if (fread(buf, 1, sizeof(buf), filp) >= 8) {
            pi_periph_base = buf[4] << 24 | buf[5] << 16 | buf[6] << 8 | buf[7];
            if (!pi_periph_base)
                pi_periph_base = buf[8] << 24 | buf[9] << 16 | buf[10] << 8 | buf[11];

            if (pi_periph_base == 0xFE00000)
                pi_is_2711 = 1;
        }
        fclose(filp);
    }

    return rev;
}

/* Map in registers. */
static uint32_t *
init_map_mem(int fd, uint32_t addr, uint32_t len)
{
    return (uint32_t *)mmap(0, len, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED | MAP_LOCKED, fd, addr);
}

int
gpio_initialise(void)
{
    int fd;

    gpio_hardware_revision(); /* sets rev and peripherals base address */

    fd = open("/dev/mem", O_RDWR | O_SYNC);

    if (fd < 0) {
        fprintf(stderr, "This program needs root privileges.  Try using sudo\n");
        return -1;
    }

    gpio_reg = init_map_mem(fd, GPIO_BASE, GPIO_LEN);
    syst_reg = init_map_mem(fd, SYST_BASE, SYST_LEN);
    bscs_reg = init_map_mem(fd, BSCS_BASE, BSCS_LEN);

    close(fd);

    if ((gpio_reg == MAP_FAILED) || (syst_reg == MAP_FAILED) || (bscs_reg == MAP_FAILED)) {
        fprintf(stderr, "Bad, mmap failed\n");
        return -1;
    }
    return 0;
}

struct gpio_out
gpio_out_setup(uint32_t pin, uint8_t val)
{
    struct gpio_out g = { .pin = pin };
    gpio_out_reset(g, val);
    return g;
}

void
gpio_out_reset(struct gpio_out g, uint8_t val)
{
    int reg, shift;
    reg = g.pin / 10;
    shift = (g.pin % 10) * 3;
    gpio_reg[reg] = (gpio_reg[reg] & ~(7 << shift)) | (PI_OUTPUT << shift);

    gpio_out_write(g, val);
}

void
gpio_out_write(struct gpio_out g, uint8_t val)
{
    if (val == 0) {
        *(gpio_reg + GPCLR0 + PI_BANK(g.pin)) = PI_BIT(g.pin);
    } else {
        *(gpio_reg + GPSET0 + PI_BANK(g.pin)) = PI_BIT(g.pin);
    }
    gpio_out_states[g.pin] = val;
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    gpio_out_write(g, !gpio_out_states[g.pin]);
}

void
gpio_out_toggle(struct gpio_out g)
{
    gpio_out_toggle_noirq(g);
}

struct gpio_in
gpio_in_setup(uint32_t pin, int8_t pull_up)
{
    struct gpio_in g = { .pin = pin };
    gpio_in_reset(g, pull_up);
    return g;
}

void
gpio_in_reset(struct gpio_in g, int8_t pull_up)
{
    {
        int reg, shift;
        reg = g.pin / 10;
        shift = (g.pin % 10) * 3;
        gpio_reg[reg] = (gpio_reg[reg] & ~(7 << shift)) | (PI_INPUT << shift);
    }

    {
        uint32_t pull_reg_val;

        if (pull_up > 0) {
            if (pi_is_2711) {
                pull_reg_val = 1;
            } else {
                pull_reg_val = 2;
            }
        } else if (pull_up < 0) {
            if (pi_is_2711) {
                pull_reg_val = 2;
            } else {
                pull_reg_val = 1;
            }
        } else {
            pull_reg_val = 0;
        }

        if (pi_is_2711) {
            int shift = (g.pin & 0xf) << 1;
            uint32_t bits = *(gpio_reg + GPPUPPDN0 + (g.pin >> 4));
            bits &= ~(3 << shift);
            bits |= (pull_reg_val << shift);
            *(gpio_reg + GPPUPPDN0 + (g.pin >> 4)) = bits;
        } else {
            *(gpio_reg + GPPUD) = pull_reg_val;
            usleep(20);
            *(gpio_reg + GPPUDCLK0 + PI_BANK(g.pin)) = PI_BIT(g.pin);
            usleep(20);
            *(gpio_reg + GPPUD) = 0;
            *(gpio_reg + GPPUDCLK0 + PI_BANK(g.pin)) = 0;
        }
    }
}

uint8_t
gpio_in_read(struct gpio_in g)
{
    return *(gpio_reg + GPLEV0 + PI_BANK(g.pin)) & PI_BIT(g.pin);
}
