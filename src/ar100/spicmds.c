// Commands for sending messages on an SPI bus
//
// Copyright (C) 2016-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "autoconf.h" // CONFIG_HAVE_GPIO_BITBANGING
#include "board/gpio.h" // gpio_out_write
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_SHUTDOWN
#include "spi_software.h" // spi_software_setup
#include "spicmds.h" // spidev_transfer

struct spidev_s {
    struct spi_software *spi_software;
    struct gpio_out pin;
    uint8_t flags;
};

enum {
    SF_HAVE_PIN = 1, SF_SOFTWARE = 2, SF_HARDWARE = 4,
};

void
command_config_spi(uint32_t *args)
{
    struct spidev_s *spi = oid_alloc(args[0], command_config_spi, sizeof(*spi));
    spi->pin = gpio_out_setup(args[1], 1);
    spi->flags |= SF_HAVE_PIN;
}
DECL_COMMAND(command_config_spi, "config_spi oid=%c pin=%u");

void
command_config_spi_without_cs(uint32_t *args)
{
    struct spidev_s *spi = oid_alloc(args[0], command_config_spi, sizeof(*spi));
}
DECL_COMMAND(command_config_spi_without_cs, "config_spi_without_cs oid=%c");

struct spidev_s *
spidev_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_spi);
}

void
spidev_set_software_bus(struct spidev_s *spi, struct spi_software *ss)
{
    if (spi->flags & (SF_SOFTWARE|SF_HARDWARE))
        shutdown("Invalid spi config");
    spi->spi_software = ss;
    spi->flags |= SF_SOFTWARE;
}

void
spidev_transfer(struct spidev_s *spi, uint8_t receive_data
                , uint8_t data_len, uint8_t *data)
{
    if (!(spi->flags & (SF_SOFTWARE|SF_HARDWARE)))
        // Not yet initialized
        return;

    spi_software_prepare(spi->spi_software);

    if (spi->flags & SF_HAVE_PIN)
        gpio_out_write(spi->pin, 0);

    spi_software_transfer(spi->spi_software, receive_data, data_len, data);

    if (spi->flags & SF_HAVE_PIN)
        gpio_out_write(spi->pin, 1);
}

void
command_spi_transfer(uint32_t *args)
{
    uint8_t oid = args[0];
    struct spidev_s *spi = spidev_oid_lookup(oid);
    uint8_t data_len = args[1];
    uint8_t *data = command_decode_ptr(args[2]);
    spidev_transfer(spi, 1, data_len, data);
    sendf("spi_transfer_response oid=%c response=%*s", oid, data_len, data);
}
DECL_COMMAND(command_spi_transfer, "spi_transfer oid=%c data=%*s");

void
command_spi_send(uint32_t *args)
{
    struct spidev_s *spi = spidev_oid_lookup(args[0]);
    uint8_t data_len = args[1];
    uint8_t *data = command_decode_ptr(args[2]);
    spidev_transfer(spi, 0, data_len, data);
}
DECL_COMMAND(command_spi_send, "spi_send oid=%c data=%*s");


/****************************************************************
 * Shutdown handling
 ****************************************************************/

struct spidev_shutdown_s {
    struct spidev_s *spi;
    uint8_t shutdown_msg_len;
    uint8_t shutdown_msg[];
};

void
command_config_spi_shutdown(uint32_t *args)
{
    struct spidev_s *spi = spidev_oid_lookup(args[1]);
    uint8_t shutdown_msg_len = args[2];
    struct spidev_shutdown_s *sd = oid_alloc(
        args[0], command_config_spi_shutdown, sizeof(*sd) + shutdown_msg_len);
    sd->spi = spi;
    sd->shutdown_msg_len = shutdown_msg_len;
    uint8_t *shutdown_msg = command_decode_ptr(args[3]);
    memcpy(sd->shutdown_msg, shutdown_msg, shutdown_msg_len);
}
DECL_COMMAND(command_config_spi_shutdown,
             "config_spi_shutdown oid=%c spi_oid=%c shutdown_msg=%*s");

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
    struct spidev_shutdown_s *sd;
    foreach_oid(oid, sd, command_config_spi_shutdown) {
        spidev_transfer(sd->spi, 0, sd->shutdown_msg_len, sd->shutdown_msg);
    }
}
DECL_SHUTDOWN(spidev_shutdown);
