// Communicating with an SPI device via linux spidev
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <fcntl.h> // open
#include <stdio.h> // snprintf
#include <unistd.h> // write
#include "command.h" // DECL_COMMAND
#include "internal.h" // report_errno
#include "sched.h" // shutdown

struct spi_s {
    uint32_t bus, dev;
    int fd;
};
static struct spi_s devices[16];
static int devices_count;

static int
spi_open(uint32_t bus, uint32_t dev)
{
    // Find existing device (if already opened)
    int i;
    for (i=0; i<devices_count; i++)
        if (devices[i].bus == bus && devices[i].dev == dev)
            return devices[i].fd;

    // Setup new SPI device
    if (devices_count >= ARRAY_SIZE(devices))
        shutdown("Too many spi devices");
    char fname[256];
    snprintf(fname, sizeof(fname), "/dev/spidev%d.%d", bus, dev);
    int fd = open(fname, O_RDWR|O_CLOEXEC);
    if (fd < 0) {
        report_errno("open spi", fd);
        shutdown("Unable to open spi device");
    }
    int ret = set_non_blocking(fd);
    if (ret < 0)
        shutdown("Unable to set non-blocking on spi device");

    devices[devices_count].bus = bus;
    devices[devices_count].dev = dev;
    devices[devices_count].fd = fd;
    return fd;
}

static void
spi_write(int fd, char *data, int len)
{
    int ret = write(fd, data, len);
    if (ret < 0) {
        report_errno("write spi", ret);
        shutdown("Unable to write to spi");
    }
}

void
command_send_spi(uint32_t *args)
{
    int fd = spi_open(args[0], args[1]);
    uint8_t len = args[2];
    char *msg = (void*)(size_t)args[3];
    spi_write(fd, msg, len);
}
DECL_COMMAND(command_send_spi, "send_spi bus=%u dev=%u msg=%*s");
