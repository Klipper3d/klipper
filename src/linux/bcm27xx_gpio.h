// Efficient support via direct memory access for Raspberry Pi 2/3/4
//
// Copyright (C) 2021 Klaus Schwartz <klaus@eraga.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.


// References and credits:
// https://elinux.org/RPi_GPIO_Code_Samples#Direct_register_access
// https://github.com/WiringPi/WiringPi/blob/master/wiringPi/wiringPi.c

#ifndef KLIPPER_BCM27XX_GPIO_H
#define KLIPPER_BCM27XX_GPIO_H

#include "command.h" // shutdown
#include "internal.h" // report_errno
#include <unistd.h>

#include </usr/include/linux/gpio.h>
#include <sys/ioctl.h>

#include <sys/mman.h>

#if CONFIG_MACH_LINUX_BCM2709 == 1
#include "bcm2709_gpio.h"
#elif CONFIG_MACH_LINUX_BCM2711 == 1
#include "bcm2711_gpio.h"
#else
error("MACH_LINUX_BCM should be defined");
#endif

#define GPIO_BASE                (GPIO_PERI_BASE + 0x200000)

#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)
#define GPIO_CLR *(gpio+10)

#define GET_GPIO(g) (*(gpio + gpioToGPLEV [g]) & (1 << (g & 31)))

static uint8_t gpioToGPLEV [] =
        {
                13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
                13,13,13,13,13,13,13,13,13,13,13,13,13,13,
                14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
                14,14,14,14,14,14,14,14,14,14,14,14,14,14,
        } ;


#define MMAP_BLOCK_SIZE (4*1024)



#define CHIP_FILE_NAME "/dev/gpiochip%u"
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

static struct gpio_line lines[8*MAX_GPIO_LINES];

static int gpio_chip_fd[8] = { -1 };

int
get_chip_fd(uint8_t chipId) {
    char chipFilename[64],errorMessage[256];
    int i = 0;
    if (gpio_chip_fd[chipId] <= 0) {
        snprintf(chipFilename,sizeof(chipFilename), CHIP_FILE_NAME, chipId);
        if(access(chipFilename, F_OK) < 0){
            snprintf(errorMessage,sizeof(errorMessage),
                     "%s not found!",chipFilename);
            report_errno(errorMessage,-1);
            shutdown("GPIO chip device not found");
        }
        gpio_chip_fd[chipId] = open(chipFilename,O_RDWR | O_CLOEXEC);
        if (gpio_chip_fd[chipId] < 0) {
            snprintf(errorMessage,sizeof(errorMessage),
                     "Unable to open GPIO %s",chipFilename);
            report_errno(errorMessage,-1);
            shutdown("Unable to open GPIO chip device");
        }
        for (i=0; i<MAX_GPIO_LINES; ++i) {
            lines[GPIO(chipId,i)].offset = i;
            lines[GPIO(chipId,i)].fd = -1;
            lines[GPIO(chipId,i)].chipid = chipId;
        }
    }
    return gpio_chip_fd[chipId];
}



void
gpio_release_line(struct gpio_line* line)
{
    if (line->fd > 0) {
        close(line->fd);
        line->fd = -1;
    }
}

static void
gpio_claim_line(struct gpio_line* line, uint8_t val, __u32 flags) {
    int rv;
    struct gpiohandle_request req;
    gpio_release_line(line);
    memset(&req, 0, sizeof(req));
    req.lines = 1;
    req.flags = flags;
    req.lineoffsets[0] = line->offset;

    if(flags == GPIOHANDLE_REQUEST_OUTPUT) {
        req.default_values[0] = !!val;
    }

    strncpy(req.consumer_label,GPIO_CONSUMER,sizeof(req.consumer_label) - 1);
    rv = ioctl(get_chip_fd(line->chipid), GPIO_GET_LINEHANDLE_IOCTL, &req);
    if (rv < 0) {
        if(flags == GPIOHANDLE_REQUEST_OUTPUT) {
            report_errno("gpio_out_reset claim line",rv);
        } else {
            report_errno("gpio_in_reset claim line",rv);
        }
        shutdown("Unable to open out GPIO chip line");
    }
    set_close_on_exec(req.fd);
    line->fd = req.fd;
    if(flags == GPIOHANDLE_REQUEST_OUTPUT) {
        line->state = !!val;
    }
}

#endif //KLIPPER_BCM27XX_GPIO_H
