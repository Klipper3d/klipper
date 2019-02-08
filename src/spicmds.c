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
#include "spicmds.h" // spidev_transfer

struct spidev_s {
    struct spi_config spi_config;
    struct gpio_out pin;
    uint8_t flags;
    uint8_t shutdown_msg_len;
    uint8_t shutdown_msg[];
};

enum {
    SF_HAVE_PIN = 1,
};

void
command_config_spi(uint32_t *args)
{
    uint8_t mode = args[3], shutdown_msg_len = args[5];
    if (mode > 3)
        shutdown("Invalid spi mode");
    struct spidev_s *spi = oid_alloc(args[0], command_config_spi
                                     , sizeof(*spi) + shutdown_msg_len);
    spi->pin = gpio_out_setup(args[2], 1);
    spi->flags = SF_HAVE_PIN;
    spi->spi_config = spi_setup(args[1], mode, args[4]);
    spi->shutdown_msg_len = shutdown_msg_len;
    uint8_t *shutdown_msg = (void*)(size_t)args[6];
    memcpy(spi->shutdown_msg, shutdown_msg, shutdown_msg_len);
}
DECL_COMMAND(command_config_spi,
             "config_spi oid=%c bus=%u pin=%u mode=%u rate=%u shutdown_msg=%*s");

void
command_config_spi_without_cs(uint32_t *args)
{
    uint8_t mode = args[2], shutdown_msg_len = args[4];
    if (mode > 3)
        shutdown("Invalid spi mode");
    struct spidev_s *spi = oid_alloc(args[0], command_config_spi
                                     , sizeof(*spi) + shutdown_msg_len);
    spi->spi_config = spi_setup(args[1], mode, args[3]);
    spi->shutdown_msg_len = shutdown_msg_len;
    uint8_t *shutdown_msg = (void*)(size_t)args[5];
    memcpy(spi->shutdown_msg, shutdown_msg, shutdown_msg_len);
}
DECL_COMMAND(command_config_spi_without_cs,
             "config_spi_without_cs oid=%c bus=%u mode=%u rate=%u"
             " shutdown_msg=%*s");

struct spidev_s *
spidev_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_spi);
}

void
spidev_transfer(struct spidev_s *spi, uint8_t receive_data
                , uint8_t data_len, uint8_t *data)
{
    spi_prepare(spi->spi_config);

    if (spi->flags & SF_HAVE_PIN)
        gpio_out_write(spi->pin, 0);

    spi_transfer(spi->spi_config, receive_data, data_len, data);

    if (spi->flags & SF_HAVE_PIN)
        gpio_out_write(spi->pin, 1);
}

void
command_spi_transfer(uint32_t *args)
{
    uint8_t oid = args[0];
    struct spidev_s *spi = oid_lookup(oid, command_config_spi);
    uint8_t data_len = args[1];
    uint8_t *data = (void*)(size_t)args[2];
    spidev_transfer(spi, 1, data_len, data);
    sendf("spi_transfer_response oid=%c response=%*s", oid, data_len, data);
}
DECL_COMMAND(command_spi_transfer, "spi_transfer oid=%c data=%*s");

void
command_spi_send(uint32_t *args)
{
    uint8_t oid = args[0];
    struct spidev_s *spi = oid_lookup(oid, command_config_spi);
    uint8_t data_len = args[1];
    uint8_t *data = (void*)(size_t)args[2];
    spidev_transfer(spi, 0, data_len, data);
}
DECL_COMMAND(command_spi_send, "spi_send oid=%c data=%*s");

void
spidev_shutdown(void)
{
    // Cancel any transmissions that may be in progress
    uint8_t oid;
    struct spidev_s *spi;
    foreach_oid(oid, spi, command_config_spi) {
        if (spi->flags & SF_HAVE_PIN)
            gpio_out_write(spi->pin, 1);
    }

    // Send shutdown messages
    foreach_oid(oid, spi, command_config_spi) {
        if (spi->shutdown_msg_len)
            spidev_transfer(spi, 0, spi->shutdown_msg_len, spi->shutdown_msg);
    }
}
DECL_SHUTDOWN(spidev_shutdown);
