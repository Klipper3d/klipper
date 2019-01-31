// spi support on samd
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "internal.h" // enable_pclock
#include "command.h" // shutdown
#include "gpio.h" // spi_setup
#include "sched.h" // sched_shutdown

static void
spi_init(uint32_t ctrla, uint32_t baud)
{
    static int have_run_init;
    if (have_run_init)
        return;
    have_run_init = 1;

    // Setup clock
    enable_pclock(SERCOM4_GCLK_ID_CORE, ID_SERCOM4);

    // Configure MISO, MOSI, SCK pins
    gpio_peripheral(GPIO('A', 12), 'D', 0);
    gpio_peripheral(GPIO('B', 10), 'D', 0);
    gpio_peripheral(GPIO('B', 11), 'D', 0);

    // Configure spi
    SercomSpi *ss = &SERCOM4->SPI;
    ss->CTRLA.reg = 0;
    ss->CTRLA.reg = ctrla & ~SERCOM_SPI_CTRLA_ENABLE;
    ss->CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;
    ss->BAUD.reg = baud;
    ss->CTRLA.reg = ctrla;
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus)
        shutdown("Invalid spi bus");

    uint32_t ctrla = (SERCOM_SPI_CTRLA_MODE(3)
                      | (mode << SERCOM_SPI_CTRLA_CPHA_Pos)
                      | SERCOM_SPI_CTRLA_DIPO(0)
                      | SERCOM_SPI_CTRLA_DOPO(1)
                      | SERCOM_SPI_CTRLA_ENABLE);
    uint32_t baud = get_pclock_frequency(SERCOM4_GCLK_ID_CORE) / (2 * rate) - 1;
    spi_init(ctrla, baud);
    return (struct spi_config){ .ctrla = ctrla, .baud = baud };
}

void
spi_prepare(struct spi_config config)
{
    uint32_t ctrla = config.ctrla, baud = config.baud;
    SercomSpi *ss = &SERCOM4->SPI;
    if (ctrla == ss->CTRLA.reg && baud == ss->BAUD.reg)
        return;
    ss->CTRLA.reg = ctrla & ~SERCOM_SPI_CTRLA_ENABLE;
    ss->CTRLA.reg = ctrla & ~SERCOM_SPI_CTRLA_ENABLE;
    ss->BAUD.reg = baud;
    ss->CTRLA.reg = ctrla;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data
             , uint8_t len, uint8_t *data)
{
    SercomSpi *ss = &SERCOM4->SPI;
    if (receive_data) {
        while (len--) {
            ss->DATA.reg = *data;
            // wait for receive register
            while (!(ss->INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC))
                ;
            // get data
            *data++ = ss->DATA.reg;
        }
    } else {
        while (len--) {
            ss->DATA.reg = *data++;
            // wait for receive register
            while (!(ss->INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC))
                ;
            // read data (to clear RXC)
            ss->DATA.reg;
        }
    }
}
