//
// Created by Klaus Schwartz on 22/04/2021.
//
//

#ifndef KLIPPER_GPIO_COMMONS_H
#define KLIPPER_GPIO_COMMONS_H

#include "command.h" // shutdown
#include "internal.h" // report_errno
#include <unistd.h>

#include </usr/include/linux/gpio.h>
#include <sys/ioctl.h>


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

#endif //KLIPPER_GPIO_COMMONS_H
