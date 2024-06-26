// Commands for sending messages on an SPI bus
//
// Copyright (C) 2016-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "autoconf.h" // CONFIG_WANT_SOFTWARE_SPI
#include "board/gpio.h" // gpio_out_write
#include "basecmd.h" // oid_alloc
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_SHUTDOWN
#include "spi_software.h" // spi_software_setup
#include "spicmds.h" // spidev_transfer

struct spidev_s {
    union {
        struct spi_config spi_config;
        struct spi_software *spi_software;
    };
    struct gpio_out pin;
    uint8_t flags;
};

enum {
    SF_HAVE_PIN = 1, SF_SOFTWARE = 2, SF_HARDWARE = 4, SF_CS_ACTIVE_HIGH = 8
};

void
command_config_spi(uint32_t *args)
{
    struct spidev_s *spi = oid_alloc(args[0], command_config_spi, sizeof(*spi));
    uint_fast8_t cs_active_high = args[2];
    spi->pin = gpio_out_setup(args[1], !cs_active_high);
    spi->flags |= SF_HAVE_PIN | (cs_active_high ? SF_CS_ACTIVE_HIGH : 0);
}
DECL_COMMAND(command_config_spi, "config_spi oid=%c pin=%u cs_active_high=%c");

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
command_spi_set_bus(uint32_t *args)
{
    struct spidev_s *spi = spidev_oid_lookup(args[0]);
    uint8_t mode = args[2];
    if (mode > 3 || spi->flags & (SF_SOFTWARE|SF_HARDWARE))
        shutdown("Invalid spi config");
    spi->spi_config = spi_setup(args[1], mode, args[3]);
    spi->flags |= SF_HARDWARE;
}
DECL_COMMAND(command_spi_set_bus,
             "spi_set_bus oid=%c spi_bus=%u mode=%u rate=%u");

void
spidev_set_software_bus(struct spidev_s *spi, struct spi_software *ss)
{
    if (spi->flags & (SF_SOFTWARE|SF_HARDWARE))
        shutdown("Invalid spi config");
    spi->spi_software = ss;
    spi->flags |= SF_SOFTWARE;
}

int
spidev_have_cs_pin(struct spidev_s *spi)
{
    return spi->flags & SF_HAVE_PIN;
}

struct gpio_out
spidev_get_cs_pin(struct spidev_s *spi)
{
    return spi->pin;
}

void
spidev_transfer(struct spidev_s *spi, uint8_t receive_data
                , uint8_t data_len, uint8_t *data)
{
    uint_fast8_t flags = spi->flags;
    if (!(flags & (SF_SOFTWARE|SF_HARDWARE)))
        // Not yet initialized
        return;

    if (CONFIG_WANT_SOFTWARE_SPI && flags & SF_SOFTWARE)
        spi_software_prepare(spi->spi_software);
    else
        spi_prepare(spi->spi_config);

    if (flags & SF_HAVE_PIN)
        gpio_out_write(spi->pin, !!(flags & SF_CS_ACTIVE_HIGH));

    if (CONFIG_WANT_SOFTWARE_SPI && flags & SF_SOFTWARE)
        spi_software_transfer(spi->spi_software, receive_data, data_len, data);
    else
        spi_transfer(spi->spi_config, receive_data, data_len, data);

    if (flags & SF_HAVE_PIN)
        gpio_out_write(spi->pin, !(flags & SF_CS_ACTIVE_HIGH));
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
            gpio_out_write(spi->pin, !(spi->flags & SF_CS_ACTIVE_HIGH));
    }

    // Send shutdown messages
    struct spidev_shutdown_s *sd;
    foreach_oid(oid, sd, command_config_spi_shutdown) {
        spidev_transfer(sd->spi, 0, sd->shutdown_msg_len, sd->shutdown_msg);
    }
}
DECL_SHUTDOWN(spidev_shutdown);
