// SPI functions on AT32
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/io.h" // readb, writeb
#include "command.h" // shutdown
#include "gpio.h" // spi_setup
#include "internal.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

struct spi_info {
    void *spi;
    uint8_t miso_pin, mosi_pin, sck_pin, function;
};

//DECL_ENUMERATION("spi_bus", "spi1", 0);
//DECL_CONSTANT_STR("BUS_PINS_spi2", "PB14,PB15,PB13");
//DECL_ENUMERATION("spi_bus", "spi2", 1);
//DECL_CONSTANT_STR("BUS_PINS_spi1", "PA6,PA7,PA5");

//static const struct spi_info spi_bus[] = {
//  { SPI2, GPIO('B', 14), GPIO('B', 15), GPIO('B', 13), GPIO_MUX_0 },
//    { SPI1, GPIO('A', 6), GPIO('A', 7), GPIO('A', 5), GPIO_MUX_0 },
//};

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    //if (bus >= sizeof(spi_bus)/sizeof(spi_bus[0])
    //    shutdown("Invalid spi bus");

    return (struct spi_config){0,0,0};
}

void
spi_prepare(struct spi_config config)
{
	return;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data,
             uint8_t len, uint8_t *data)
{
	return;
}
