// Communicating with a PCA9685 pwm device via linux i2c
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <fcntl.h> // open
#include <linux/i2c-dev.h> // I2C_SLAVE
#include <stdio.h> // snprintf
#include <sys/ioctl.h> // ioctl
#include <unistd.h> // write
#include "basecmd.h" // oid_alloc
#include "board/misc.h" // timer_from_us
#include "command.h" // DECL_COMMAND
#include "internal.h" // report_errno
#include "sched.h" // DECL_SHUTDOWN


/****************************************************************
 * Low-level i2c
 ****************************************************************/

#define P9_MODE1    0x00
#define P9_PRESCALE 0xfe
#define CHANNEL_MAX 15
#define CHANNEL_ALL 61
#define VALUE_MAX 4096

#define OSC_MHZ   25
#define CLOCK_MHZ timer_from_us(1)

struct i2c_s {
    uint8_t bus;
    uint8_t addr;
    uint32_t cycle_ticks;
    int fd;
};
static struct i2c_s devices[16];
static int devices_count;

static void
pca9685_write(int fd, uint8_t channel, uint16_t value)
{
    uint8_t full_on = 0x00;
    if (value >= VALUE_MAX) {
        full_on = 0x10;
        value = 0x0000;
    } else if (!value) {
        value = 0x1000;
    }
    uint8_t msg[5] = { channel*4 + 0x06, 0x00, full_on, value, value >> 8 };
    int ret = write(fd, msg, sizeof(msg));
    if (ret < 0) {
        report_errno("write value i2c", ret);
        try_shutdown("Unable to update PCA9685 value");
    }
}

static int
open_i2c(uint8_t bus, uint8_t addr, uint32_t cycle_ticks)
{
    // Find existing device (if already opened)
    int i;
    for (i=0; i<devices_count; i++)
        if (devices[i].bus == bus && devices[i].addr == addr) {
            if (cycle_ticks != devices[i].cycle_ticks)
                shutdown("All PCA9685 channels must have the same cycle_ticks");
            return devices[i].fd;
        }

    // Setup new I2C device
    if (devices_count >= ARRAY_SIZE(devices))
        shutdown("Too many i2c devices");
    char fname[256];
    snprintf(fname, sizeof(fname), "/dev/i2c-%d", bus);
    int fd = open(fname, O_RDWR|O_CLOEXEC);
    if (fd < 0) {
        report_errno("open i2c", fd);
        goto fail;
    }
    int ret = ioctl(fd, I2C_SLAVE, addr);
    if (ret < 0) {
        report_errno("ioctl i2c", fd);
        goto fail;
    }
    ret = set_non_blocking(fd);
    if (ret < 0)
        goto fail;

    // Init PCA9685
    const uint8_t sleep_msg[2] = { P9_MODE1, 0x31 };
    ret = write(fd, sleep_msg, sizeof(sleep_msg));
    if (ret < 0) {
        report_errno("write sleep i2c", ret);
        goto fail;
    }
    uint32_t freq = DIV_ROUND_CLOSEST(OSC_MHZ*cycle_ticks, 4096*CLOCK_MHZ) - 1;
    freq = freq > 0xff ? 0xff : (freq < 0x03 ? 0x03 : freq);
    uint8_t freq_msg[2] = { P9_PRESCALE, freq };
    ret = write(fd, freq_msg, sizeof(freq_msg));
    if (ret < 0) {
        report_errno("write freq i2c", ret);
        goto fail;
    }
    const uint8_t wake_msg[2] = { P9_MODE1, 0x21 };
    ret = write(fd, wake_msg, sizeof(wake_msg));
    if (ret < 0) {
        report_errno("write unsleep i2c", ret);
        goto fail;
    }
    usleep(500);
    pca9685_write(fd, CHANNEL_ALL, 0);

    devices[devices_count].bus = bus;
    devices[devices_count].addr = addr;
    devices[devices_count].cycle_ticks = cycle_ticks;
    devices[devices_count].fd = fd;
    devices_count++;
    return fd;
fail:
    if (fd >= 0)
        close(fd);
    shutdown("Unable to open and init PCA9685 device");
}


/****************************************************************
 * Command interface
 ****************************************************************/

struct i2cpwm_s {
    struct timer timer;
    int fd;
    uint8_t channel;
    uint16_t value, default_value;
    uint32_t max_duration;
};

DECL_CONSTANT("PCA9685_MAX", VALUE_MAX);

static uint_fast8_t
pca9685_end_event(struct timer *timer)
{
    shutdown("Missed scheduling of next pca9685 event");
}

static uint_fast8_t
pca9685_event(struct timer *timer)
{
    struct i2cpwm_s *p = container_of(timer, struct i2cpwm_s, timer);
    pca9685_write(p->fd, p->channel, p->value);
    if (p->value == p->default_value || !p->max_duration)
        return SF_DONE;
    p->timer.waketime += p->max_duration;
    p->timer.func = pca9685_end_event;
    return SF_RESCHEDULE;
}

void
command_config_pca9685(uint32_t *args)
{
    uint8_t bus = args[1], addr = args[2], channel = args[3];
    uint16_t value = args[5], default_value = args[6];
    if (channel > CHANNEL_MAX || value > VALUE_MAX || default_value > VALUE_MAX)
        shutdown("Invalid pca9685 channel or value");
    int fd = open_i2c(bus, addr, args[4]);
    if (value)
        pca9685_write(fd, channel, value);
    struct i2cpwm_s *p = oid_alloc(args[0], command_config_pca9685, sizeof(*p));
    p->fd = fd;
    p->channel = channel;
    p->default_value = default_value;
    p->max_duration = args[7];
}
DECL_COMMAND(command_config_pca9685, "config_pca9685 oid=%c bus=%c addr=%c"
             " channel=%c cycle_ticks=%u value=%hu"
             " default_value=%hu max_duration=%u");

void
command_schedule_pca9685_out(uint32_t *args)
{
    struct i2cpwm_s *p = oid_lookup(args[0], command_config_pca9685);
    sched_del_timer(&p->timer);
    p->timer.func = pca9685_event;
    p->timer.waketime = args[1];
    p->value = args[2];
    if (p->value > VALUE_MAX)
        shutdown("Invalid pca9685 value");
    sched_add_timer(&p->timer);
}
DECL_COMMAND(command_schedule_pca9685_out,
             "schedule_pca9685_out oid=%c clock=%u value=%hu");

void
command_set_pca9685_out(uint32_t *args)
{
    uint8_t bus = args[0], addr = args[1], channel = args[2];
    uint16_t value = args[4];
    if (channel > CHANNEL_MAX || value > VALUE_MAX)
        shutdown("Invalid pca9685 channel or value");
    int fd = open_i2c(bus, addr, args[3]);
    pca9685_write(fd, channel, value);
}
DECL_COMMAND(command_set_pca9685_out, "set_pca9685_out bus=%c addr=%c"
             " channel=%c cycle_ticks=%u value=%hu");

void
pca9685_shutdown(void)
{
    int i;
    for (i=0; i<devices_count; i++)
        pca9685_write(devices[i].fd, CHANNEL_ALL, 0);
    uint8_t j;
    struct i2cpwm_s *p;
    foreach_oid(j, p, command_config_pca9685) {
        if (p->default_value)
            pca9685_write(p->fd, p->channel, p->default_value);
    }
}
DECL_SHUTDOWN(pca9685_shutdown);
