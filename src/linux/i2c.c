// Linux i2c implementation
//
// Copyright (C) 2020  Eric Callahan <arksine.code@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#include <fcntl.h> // open
#include <linux/i2c-dev.h> // I2C_SLAVE i2c_msg
#include <linux/i2c.h> // i2c_rdwr_ioctl_data I2C_M_RD I2C_FUNC_I2C
#include <stdio.h> // snprintf
#include <sys/ioctl.h> // ioctl
#include <unistd.h> // write
#include "gpio.h" // i2c_setup
#include "command.h" // shutdown
#include "internal.h" // report_errno
#include "sched.h" // sched_shutdown

DECL_ENUMERATION_RANGE("i2c_bus", "i2c.0", 0, 15);

struct i2c_s {
    uint32_t bus;
    uint8_t addr;
    int fd;
};

static struct i2c_s devices[16];
static int devices_count;

static int
i2c_open(uint32_t bus, uint8_t addr)
{
    // Find existing device (if already opened)
    int i;
    for (i=0; i<devices_count; i++) {
        if (devices[i].bus == bus && devices[i].addr == addr) {
            return devices[i].fd;
        }
    }

    char fname[256];
    snprintf(fname, sizeof(fname), "/dev/i2c-%d", bus);
    int fd = open(fname, O_RDWR|O_CLOEXEC);
    if (fd < 0) {
        report_errno("open i2c", fd);
        goto fail;
    }
    // Test for I2C_RDWR support
    unsigned long i2c_funcs; // datatype from ioctl spec.
    ioctl(fd, I2C_FUNCS, &i2c_funcs);
    if ((i2c_funcs & I2C_FUNC_I2C) == 0) {
        report_errno("i2c does not support I2C_RDWR", fd);
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

    devices[devices_count].bus = bus;
    devices[devices_count].addr = addr;
    devices[devices_count].fd = fd;
    devices_count++;

    return fd;

fail:
    if (fd >= 0)
        close(fd);
    shutdown("Unable to open i2c device");
}

struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    // Note:  The rate is set by the kernel driver, for a Raspberry Pi this
    // is done with the following setting in /boot/config.txt:
    //
    // dtparam=i2c_baudrate=<rate>

    int fd = i2c_open(bus, addr);
    return (struct i2c_config){.fd=fd, .addr=addr};
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *data)
{
    int ret = write(config.fd, data, write_len);
    if (ret != write_len) {
        if (ret < 0)
            report_errno("write value i2c", ret);
        try_shutdown("Unable write i2c device");
    }
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *data)
{
    struct i2c_rdwr_ioctl_data i2c_data;
    struct i2c_msg msgs[2];

    if(reg_len != 0) {
        msgs[0].addr = config.addr;
        msgs[0].flags = 0x0;
        msgs[0].len = reg_len;
        msgs[0].buf = reg;
        i2c_data.nmsgs = 2;
        i2c_data.msgs = &msgs[0];
    } else {
        i2c_data.nmsgs = 1;
        i2c_data.msgs = &msgs[1];
    }

    msgs[1].addr = config.addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = read_len;
    msgs[1].buf = data;

    int ret = ioctl(config.fd, I2C_RDWR, &i2c_data);

    if(ret < 0) {
        try_shutdown("Unable to read i2c device");
    }
}
