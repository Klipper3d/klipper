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
#include <errno.h> // errno, ENOENT

#include "gpio.h" // struct gpio_pwm
#include "internal.h" // NSECS_PER_TICK
#include "command.h" // shutdown
#include "sched.h" // sched_shutdown

#define MAX_PWM (1 << 15)
DECL_CONSTANT("PWM_MAX", MAX_PWM);

#define HARD_PWM_START (1<<16)
#define HARD_PWM(chip, pin) (((chip)<<4) + (pin) + HARD_PWM_START)
#define HARD_PWM_TO_CHIP(hard_pwm) (((hard_pwm) - HARD_PWM_START) >> 4)
#define HARD_PWM_TO_PIN(hard_pwm)  (((hard_pwm) - HARD_PWM_START) & 0xf)

DECL_ENUMERATION_RANGE("pin", "pwmchip0/pwm0", HARD_PWM(0, 0), 16);
DECL_ENUMERATION_RANGE("pin", "pwmchip1/pwm0", HARD_PWM(1, 0), 16);
DECL_ENUMERATION_RANGE("pin", "pwmchip2/pwm0", HARD_PWM(2, 0), 16);
DECL_ENUMERATION_RANGE("pin", "pwmchip3/pwm0", HARD_PWM(3, 0), 16);
DECL_ENUMERATION_RANGE("pin", "pwmchip4/pwm0", HARD_PWM(4, 0), 16);
DECL_ENUMERATION_RANGE("pin", "pwmchip5/pwm0", HARD_PWM(5, 0), 16);
DECL_ENUMERATION_RANGE("pin", "pwmchip6/pwm0", HARD_PWM(6, 0), 16);
DECL_ENUMERATION_RANGE("pin", "pwmchip7/pwm0", HARD_PWM(7, 0), 16);

#define PWM_PATH "/sys/class/pwm/pwmchip%u/pwm%u/%s"
#define PWM_PATH_BB "/sys/class/pwm/pwm-%u:%u/%s"

struct gpio_pwm gpio_pwm_setup(uint32_t pin, uint32_t cycle_time, uint16_t val)
{
    char filename[256];
    char scratch[16];
    uint8_t chip_id = HARD_PWM_TO_CHIP(pin);
    uint8_t pwm_id = HARD_PWM_TO_PIN(pin);
    const char * pwm_path = PWM_PATH;

    struct gpio_pwm g = {};
    g.period = cycle_time * NSECS_PER_TICK;

    // configure period/cycle time. Always in nanoseconds
    snprintf(filename, sizeof(filename), pwm_path, chip_id, pwm_id, "period");
    int fd = open(filename, O_WRONLY|O_CLOEXEC);
    if (fd == -1 && errno == ENOENT) {
        // upstream pwm control file does not exists try BeagleBoard scheme
        pwm_path = PWM_PATH_BB;
        snprintf(filename, sizeof(filename), pwm_path, chip_id, pwm_id,
                 "period");
        fd = open(filename, O_WRONLY|O_CLOEXEC);
        if (fd == -1) {
            report_errno("pwm " PWM_PATH_BB "period for pin", pin);
            goto fail;
        }
    } else if (fd == -1) {
        report_errno("pwm " PWM_PATH "period for pin", pin);
        goto fail;
    }
    snprintf(scratch, sizeof(scratch), "%u", cycle_time * NSECS_PER_TICK);
    write(fd, scratch, strlen(scratch));
    close(fd);

    // write duty cycle
    snprintf(filename, sizeof(filename), pwm_path, chip_id, pwm_id,
             "duty_cycle");
    fd = open(filename, O_WRONLY|O_CLOEXEC);
    if (fd == -1) {
        report_errno("pwm duty_cycle", fd);
        goto fail;
    }
    g.duty_fd = fd;

    // enable PWM
    snprintf(filename, sizeof(filename), pwm_path, chip_id, pwm_id, "enable");
    fd = open(filename, O_WRONLY|O_CLOEXEC);
    if (fd == -1) {
        report_errno("pwm enable", fd);
        close(g.duty_fd);
        goto fail;
    }
    g.enable_fd = fd;
    gpio_pwm_write(g, val);

    return g;

fail:
    if (fd >= 0)
        close(fd);
    shutdown("Unable to config pwm device");
}


void gpio_pwm_write(struct gpio_pwm g, uint16_t val)
{
    if (!val) {
        write(g.enable_fd, "0", 2);
        return;
    }
    char scratch[16];
    uint32_t duty_cycle = g.period * (uint64_t)val / MAX_PWM;
    int len = snprintf(scratch, sizeof(scratch), "%u", duty_cycle);
    write(g.duty_fd, scratch, len + 1);
    write(g.enable_fd, "1", 2);
}
