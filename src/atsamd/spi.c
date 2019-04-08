// spi support on samd
//
// Copyright (C) 2019  Florian Heilmann <Florian.Heilmann@gmx.net>
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "internal.h" // enable_pclock
#include "command.h" // shutdown
#include "gpio.h" // spi_setup
#include "sched.h" // sched_shutdown

void
spi_init(uint32_t bus, SercomSpi *ss, uint32_t ctrla, uint32_t baud)
{
    static uint8_t have_run_init;
    if (have_run_init & (1<<bus))
        return;
    have_run_init |= 1<<bus;

    ss->CTRLA.reg = 0;
    ss->CTRLA.reg = ctrla & ~SERCOM_SPI_CTRLA_ENABLE;
    ss->CTRLB.reg = SERCOM_SPI_CTRLB_RXEN;
    ss->BAUD.reg = baud;
    ss->CTRLA.reg = ctrla;
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    uint32_t dipo_dopo = sercom_spi_pins(bus);
    uint32_t ctrla = (SERCOM_SPI_CTRLA_MODE(3)
                      | (mode << SERCOM_SPI_CTRLA_CPHA_Pos)
                      | dipo_dopo
                      | SERCOM_SPI_CTRLA_ENABLE);
    Sercom *sercom = sercom_enable_pclock(bus);
    SercomSpi *ss = &sercom->SPI;
    uint32_t baud = sercom_get_pclock_frequency(bus) / (2 * rate) - 1;
    spi_init(bus, ss, ctrla, baud);
    return (struct spi_config){ .ss = ss, .ctrla = ctrla, .baud = baud };
}

void
spi_prepare(struct spi_config config)
{
    uint32_t ctrla = config.ctrla, baud = config.baud;
    SercomSpi *ss = (SercomSpi *)config.ss;
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
    SercomSpi *ss = (SercomSpi *)config.ss;
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
