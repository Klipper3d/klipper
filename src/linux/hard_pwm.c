// HW PWM upport via Linux PWM sysfs interface
//
// Copyright (C) 2019  Janne Grunau <janne-3d@jannau.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "gpio.h" // struct gpio_pwm
#include "internal.h" // NSECS_PER_TICK
#include "command.h" // shutdown
#include "sched.h" // sched_shutdown

#define MAX_PWM (1 << 15)
DECL_CONSTANT("PWM_MAX", MAX_PWM);

#define HARD_PWM_START (1<<16)
#define HARD_PWM(chip, pin) (((chip)<<8) + (pin) + HARD_PWM_START)
#define HARD_PWM_TO_CHIP(hard_pwm) (((hard_pwm) - HARD_PWM_START) >> 8)
#define HARD_PWM_TO_PIN(hard_pwm)  (((hard_pwm) - HARD_PWM_START) & 0xff)

DECL_ENUMERATION_RANGE("pin", "pwmchip0/pwm0", HARD_PWM(0, 0), 256);
DECL_ENUMERATION_RANGE("pin", "pwmchip1/pwm0", HARD_PWM(1, 0), 256);
DECL_ENUMERATION_RANGE("pin", "pwmchip2/pwm0", HARD_PWM(2, 0), 256);
DECL_ENUMERATION_RANGE("pin", "pwmchip3/pwm0", HARD_PWM(3, 0), 256);

#define PWM_PATH "/sys/class/pwm/pwmchip%u/pwm%u/%s"

struct gpio_pwm gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint16_t val)
{
    char filename[256];
    char scratch[16];
    uint8_t chip_id = HARD_PWM_TO_CHIP(pin);
    uint8_t pwm_id = HARD_PWM_TO_PIN(pin);

    struct gpio_pwm g = {};
    g.period = cycle_time * NSECS_PER_TICK;

    // configure period/cycle time. Always in nanoseconds
    snprintf(filename, sizeof(filename), PWM_PATH, chip_id, pwm_id, "period");
    int fd = open(filename, O_WRONLY|O_CLOEXEC);
    if (fd == -1) {
        report_errno("pwm period", fd);
        goto fail;
    }
    snprintf(scratch, sizeof(scratch), "%u", cycle_time * NSECS_PER_TICK);
    write(fd, scratch, strlen(scratch));
    close(fd);

    // write duty cycle
    snprintf(filename, sizeof(filename), PWM_PATH, chip_id, pwm_id,
             "duty_cycle");
    fd = open(filename, O_WRONLY|O_CLOEXEC);
    if (fd == -1) {
        report_errno("pwm duty_cycle", fd);
        goto fail;
    }

    g.fd = fd;
    gpio_pwm_write(g, val);

    // enable PWM
    snprintf(filename, sizeof(filename), PWM_PATH, chip_id, pwm_id, "enable");
    fd = open(filename, O_WRONLY|O_CLOEXEC);
    if (fd == -1) {
        close(g.fd);
        report_errno("pwm enable", fd);
        goto fail;
    }
    write(fd, "1", 2);
    close(fd);

    return g;

fail:
    if (fd >= 0)
        close(fd);
    shutdown("Unable to config pwm device");
}


void gpio_pwm_write(struct gpio_pwm g, uint16_t val)
{
    char scratch[16];
    uint32_t duty_cycle = g.period * (uint64_t)val / MAX_PWM;
    snprintf(scratch, sizeof(scratch), "%u", duty_cycle);
    if (g.fd != -1) {
        write(g.fd, scratch, strlen(scratch));
    } else {
        report_errno("pwm set duty_cycle", g.fd);
    }
}
