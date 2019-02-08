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
#include "hardware_spi.h"

struct spidev_s *
hardware_spi_oid_lookup(uint8_t oid)
{
    return oid_lookup(oid, command_config_spi);
}

void
hardware_spi_transfer(struct spidev_s *spi, uint8_t receive_data
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
hardware_spi_shutdown(spidev_s *spi) {
    gpio_out_write(spi->pin, 1);
    hardware_spi_transfer(spi, 0, spi->shutdown_msg_len, spi->shutdown_msg);
}
