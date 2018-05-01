#include <stddef.h>   // NULL
#include "autoconf.h"
#include "gpio.h"
#include "generic/spi.h"
#include "sched.h"
#include <sam3x8e.h>
#include <string.h>

#define REGPTR     SPI0
#define PERIPH_ID  ID_SPI0

#define CHANNEL    0 // Use same channel for all

#define SPCK  GPIO('A', 27)
#define MOSI  GPIO('A', 26)
#define MISO  GPIO('A', 25)

struct spi_config spi_basic_config = {.cfg = 0};

void
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

    spi_basic_config = spi_get_config(0, 4000000);
    /* Set basic channel config */
    REGPTR->SPI_CSR[CHANNEL] = spi_basic_config.cfg;
    /* Enable SPI */
    REGPTR->SPI_CR = SPI_CR_SPIEN;
}
DECL_INIT(spi_init);

struct spi_config
spi_get_config(uint8_t const mode, uint32_t const clock)
{
    uint32_t config = 0;
    uint32_t clockDiv;
    if (clock < (CHIP_FREQ_CPU_MAX / 255)) {
        clockDiv = 255;
    } else if (clock >= (CHIP_FREQ_CPU_MAX / 2)) {
        clockDiv = 2;
    } else {
        clockDiv = (CHIP_FREQ_CPU_MAX / (clock + 1)) + 1;
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


static uint8_t volatile reserved = 0;
uint8_t spi_set_config(struct spi_config const config) {
    if (!!reserved) return 0;
    REGPTR->SPI_CSR[CHANNEL] = config.cfg;
    return ++reserved;
}

void spi_set_ready(void) {
    reserved = 0;
}

void spi_transfer_len(char *data, uint8_t len) __attribute__ ((cold));
void spi_transfer_len(char *data, uint8_t len) {
    Spi* const pSpi = REGPTR;
    while (len--) {
        pSpi->SPI_TDR = *data;
        // wait for receive register
        while (!(pSpi->SPI_SR & SPI_SR_RDRF)) { asm volatile("nop"); };
        // get data
        *data++ = pSpi->SPI_RDR;
    }
}

uint8_t spi_transfer(uint8_t const data) __attribute__ ((cold));
uint8_t spi_transfer(uint8_t const data) {
    /* DMA is not best for one byte. */
    Spi* const pSpi = REGPTR;
    pSpi->SPI_TDR = data;
    // wait for receive register
    while (!(pSpi->SPI_SR & SPI_SR_RDRF)) { asm volatile("nop"); };
    // get data
    return pSpi->SPI_RDR;
}

uint8_t spi_read_rdy(void) {
    return (!!(REGPTR->SPI_SR & SPI_SR_RDRF));
}
