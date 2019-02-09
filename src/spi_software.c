// Software SPI emulation
//
// Copyright (C) 2019  XXX
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h" // gpio_out_setup
#include "command.h" // DECL_COMMAND

struct spi_software {
    struct gpio_out sclk, mosi;
    struct gpio_in miso;
};


void
command_config_software_spi(uint32_t *args)
{
    // XXX
}
DECL_COMMAND(command_config_software_spi,
             "config_software_spi oid=%c sclk_pin=%u mosi_pin=%u miso_pin=%u");

struct spi_software *
spi_software_setup(uint8_t oid, uint8_t mode, uint32_t rate)
{
    // XXX
    return NULL;
}

void
spi_software_prepare(struct spi_software *ss)
{
    // XXX
}

void
spi_software_transfer(struct spi_software *ss, uint8_t receive_data
                      , uint8_t len, uint8_t *data)
{
    // XXX
}
