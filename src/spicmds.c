// Commands for sending messages on an SPI bus
//
// Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "board/gpio.h" // gpio_out_write
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_SHUTDOWN
#include "spicmds.h"
#include "software_spi.h" // spidev_transfer
#include "hardware_spi.h" // spidev_transfer


void
command_config_spi(uint32_t *args)
{
    uint8_t mode = args[3], shutdown_msg_len = args[5];
    if (mode > 3)
        shutdown("Invalid spi mode");

    struct spi_command_device *spi_device = oid_alloc(args[0], command_config_spi
                                     , sizeof(*spi_device) + shutdown_msg_len);

    spi_device->is_hardware = 1;
    spi_device->hardware_spi->pin = gpio_out_setup(args[2], 1);
    spi_device->hardware_spi->flags = SF_HAVE_PIN;
    spi_device->hardware_spi->spi_config = spi_setup(args[1], mode, args[4]);
    spi_device->hardware_spi->shutdown_msg_len = shutdown_msg_len;
    uint8_t *shutdown_msg = (void*)(size_t)args[6];
    memcpy(spi_device->hardware_spi->shutdown_msg, shutdown_msg, shutdown_msg_len);
}
DECL_COMMAND(command_config_spi,
             "config_spi oid=%c bus=%u pin=%u mode=%u rate=%u shutdown_msg=%*s");

void
command_config_spi_without_cs(uint32_t *args)
{
    uint8_t mode = args[2], shutdown_msg_len = args[4];
    if (mode > 3)
        shutdown("Invalid spi mode");
    struct spi_command_device *spi_device = oid_alloc(args[0], command_config_spi
                                     , sizeof(*spi_device) + shutdown_msg_len);
    spi_device->is_hardware = 1;
    spi_device->hardware_spi->spi_config = spi_setup(args[1], mode, args[3]);
    spi_device->hardware_spi->shutdown_msg_len = shutdown_msg_len;
    uint8_t *shutdown_msg = (void*)(size_t)args[5];
    memcpy(spi_device->hardware_spi->shutdown_msg, shutdown_msg, shutdown_msg_len);
}
DECL_COMMAND(command_config_spi_without_cs,
             "config_spi_without_cs oid=%c bus=%u mode=%u rate=%u"
             " shutdown_msg=%*s");

void
command_config_software_spi(uint32_t *args)
{
    uint8_t
            oid = args[0],
            mosi = args[1],
            miso = args[2],
            ss = args[3],
            sck = args[4],
            mode = args[5],
            shutdown_msg_len = args[6],
            *shutdown_msg = (void*)(size_t)args[7];


    if (mode > 3) {
        shutdown("Invalid spi mode");
    }

    struct spi_command_device *spi_device = oid_alloc(oid, command_config_software_spi
            , sizeof(*spi_device) + shutdown_msg_len);

    spi_device->is_hardware = 0;
    spi_device->software_spi->pins.mosi = mosi;
    spi_device->software_spi->pins.miso = miso;
    spi_device->software_spi->pins.slave_select = ss;
    spi_device->software_spi->pins.sysclock = sck;
    spi_device->software_spi->shutdown_msg_len = shutdown_msg_len;

    memcpy(spi_device->software_spi->shutdown_msg, shutdown_msg, shutdown_msg_len);
}
DECL_COMMAND(command_config_spi,
             "config_software_spi oid=%c mosi=%u miso=%u ss=%u sck=%u mode=%u shutdown_msg=%*s");

void
command_config_software_spi_without_cs(uint32_t *args)
{
    uint8_t
            oid = args[0],
            mosi = args[1],
            miso = args[2],
            sck = args[3],
            mode = args[4],
            shutdown_msg_len = args[5],
            *shutdown_msg = (void*)(size_t)args[6];


    if (mode > 3) {
        shutdown("Invalid spi mode");
    }

    struct spi_command_device *spi_device = oid_alloc(oid, command_config_software_spi
            , sizeof(*spi_device) + shutdown_msg_len);


    spi_device->software_spi->pins.mosi = mosi;
    spi_device->software_spi->pins.miso = miso;
    spi_device->software_spi->pins.sysclock = sck;
    spi_device->software_spi->shutdown_msg_len = shutdown_msg_len;

    memcpy(spi_device->software_spi->shutdown_msg, shutdown_msg, shutdown_msg_len);
}
DECL_COMMAND(command_config_spi,
             "config_software_spi_without_cs oid=%c mosi=%u miso=%u sck=%u mode=%u shutdown_msg=%*s");


// @todo need to figure this out
struct spidev_s *
spidev_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_spi);
}

void
command_spi_transfer(uint32_t *args)
{
    uint8_t oid = args[0];
    struct spi_command_device *spi_device = oid_lookup(oid, command_config_spi);
    uint8_t data_len = args[1];
    uint8_t *data = (void*)(size_t)args[2];

    if (spi_device->is_hardware) {
        hardware_spi_transfer(spi_device->hardware_spi, 1, data_len, data);
    } else {
        software_spi_transfer(spi_device->software_spi, 1, data_len, data);
    }

    sendf("spi_transfer_response oid=%c response=%*s", oid, data_len, data);
}
DECL_COMMAND(command_spi_transfer, "spi_transfer oid=%c data=%*s");

void
command_spi_send(uint32_t *args)
{
    uint8_t oid = args[0];
    struct spi_command_device *spi_device = oid_lookup(oid, command_config_spi);
    uint8_t data_len = args[1];
    uint8_t *data = (void*)(size_t)args[2];

    if (spi_device->is_hardware == 1) {
        hardware_spi_transfer(spi_device->hardware_spi, 0, data_len, data);
    } else {
        software_spi_transfer(spi_device->software_spi, 0, data_len, data);
    }
}
DECL_COMMAND(command_spi_send, "spi_send oid=%c data=%*s");

void
spidev_shutdown(void)
{
    uint8_t oid;
    struct spi_command_device *spi_device;
    foreach_oid(oid, spi_device, command_spi_config) {
        if (spi_device->is_hardware == 1) {
            hardware_spi_shutdown(spi_device->hardware_spi);
        } else {
            software_spi_shutdown(spi_device->software_spi);
        }
    }
}
DECL_SHUTDOWN(spidev_shutdown);
