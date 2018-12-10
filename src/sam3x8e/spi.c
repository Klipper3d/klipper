// SPI transmissions on sam3x8e
//
// Copyright (C) 2018  Petri Honkala <cruwaller@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <sam3x8e.h> // REGPTR
#include "command.h" // shutdown
#include "gpio.h" // gpio_peripheral
#include "sched.h" // sched_shutdown

#define REGPTR     SPI0
#define PERIPH_ID  ID_SPI0

#define CHANNEL    0 // Use same channel for all

static void
spi_init(void)
{
    /* Configure SCK, MISO and MOSI */
    gpio_peripheral('A', PIO_PA25A_SPI0_MISO, 'A', 0); // Arduino 74
    gpio_peripheral('A', PIO_PA26A_SPI0_MOSI, 'A', 0); // Arduino 75
    gpio_peripheral('A', PIO_PA27A_SPI0_SPCK, 'A', 0); // Arduino 76

    // Enable SPI clocks
    if (!(PMC->PMC_PCSR0 & (1u << PERIPH_ID))) {
        PMC->PMC_PCER0 = (1 << PERIPH_ID);
    }

    /* Disable SPI */
    REGPTR->SPI_CR  = SPI_CR_SPIDIS;
    /* Execute a software reset of the SPI twice */
    REGPTR->SPI_CR  = SPI_CR_SWRST;
    REGPTR->SPI_CR  = SPI_CR_SWRST;

    REGPTR->SPI_MR = ( SPI_MR_MSTR |       // Set master mode
                       SPI_MR_MODFDIS |    // Disable fault detection
                       SPI_MR_PCS(CHANNEL) // Fixes peripheral select
                     );
    REGPTR->SPI_IDR = 0xffffffff; // Disable ISRs

    /* Clear Chip Select Registers */
    REGPTR->SPI_CSR[0] = 0;
    REGPTR->SPI_CSR[1] = 0;
    REGPTR->SPI_CSR[2] = 0;
    REGPTR->SPI_CSR[3] = 0;

    /* Set basic channel config */
    REGPTR->SPI_CSR[CHANNEL] = 0;
    /* Enable SPI */
    REGPTR->SPI_CR = SPI_CR_SPIEN;
}

struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    if (bus != CHANNEL)
        shutdown("Invalid spi_setup parameters");

    // Make sure bus is enabled
    spi_init();

    uint32_t config = 0;
    uint32_t clockDiv;
    if (rate < (CHIP_FREQ_CPU_MAX / 255)) {
        clockDiv = 255;
    } else if (rate >= (CHIP_FREQ_CPU_MAX / 2)) {
        clockDiv = 2;
    } else {
        clockDiv = (CHIP_FREQ_CPU_MAX / (rate + 1)) + 1;
    }

    /****** Will be written to SPI_CSRx register ******/
    // CSAAT  : Chip Select Active After Transfer
    config  = SPI_CSR_CSAAT;
    config |= SPI_CSR_BITS_8_BIT; // TODO: support for SPI_CSR_BITS_16_BIT
    // NOTE: NCPHA is inverted, CPHA normal!!
    switch(mode) {
        case 0:
            config |= SPI_CSR_NCPHA;
            break;
        case 1:
            config |= 0;
            break;
        case 2:
            config |= SPI_CSR_NCPHA;
            config |= SPI_CSR_CPOL;
            break;
        case 3:
            config |= SPI_CSR_CPOL;
            break;
    };

    config |= ((clockDiv & 0xffu) << SPI_CSR_SCBR_Pos);
    return (struct spi_config){.cfg = config};
}

void
spi_prepare(struct spi_config config)
{
    REGPTR->SPI_CSR[CHANNEL] = config.cfg;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data
             , uint8_t len, uint8_t *data)
{
    Spi* const pSpi = REGPTR;
    if (receive_data) {
        while (len--) {
            pSpi->SPI_TDR = *data;
            // wait for receive register
            while (!(pSpi->SPI_SR & SPI_SR_RDRF))
                ;
            // get data
            *data++ = pSpi->SPI_RDR;
        }
    } else {
        while (len--) {
            pSpi->SPI_TDR = *data++;
            // wait for receive register
            while (!(pSpi->SPI_SR & SPI_SR_RDRF))
                ;
            // read data (to clear RDRF)
            pSpi->SPI_RDR;
        }
    }
}
