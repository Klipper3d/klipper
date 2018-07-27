/**
 * \file
 *
 * \brief Serial Peripheral Interface (SPI) driver for SAM.
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** Time-out value (number of attempts). */
#define SPI_TIMEOUT       15000

/** Status codes used by the SPI driver. */
typedef enum
{
	SPI_ERROR = -1,
	SPI_OK = 0,
	SPI_ERROR_TIMEOUT = 1,
	SPI_ERROR_ARGUMENT,
	SPI_ERROR_OVERRUN,
	SPI_ERROR_MODE_FAULT,
	SPI_ERROR_OVERRUN_AND_MODE_FAULT
} spi_status_t;

/** SPI Chip Select behavior modes while transferring. */
typedef enum spi_cs_behavior {
	/** CS does not rise until a new transfer is requested on different chip select. */
	SPI_CS_KEEP_LOW = SPI_CSR_CSAAT,
	/** CS rises if there is no more data to transfer. */
	SPI_CS_RISE_NO_TX = 0,
	/** CS is de-asserted systematically during a time DLYBCS. */
	SPI_CS_RISE_FORCED = SPI_CSR_CSNAAT
} spi_cs_behavior_t;

/**
 * \brief Generate Peripheral Chip Select Value from Chip Select ID
 * \note When chip select n is working, PCS bit n is set to low level.
 *
 * \param chip_sel_id The chip select number used
 */
#define spi_get_pcs(chip_sel_id) ((~(1u<<(chip_sel_id)))&0xF)

/**
 * \brief Reset SPI and set it to Slave mode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_reset(Spi *p_spi)
{
	p_spi->SPI_CR = SPI_CR_SWRST;
}

/**
 * \brief Enable SPI.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_enable(Spi *p_spi)
{
	p_spi->SPI_CR = SPI_CR_SPIEN;
}

/**
 * \brief Disable SPI.
 *
 * \note CS is de-asserted, which indicates that the last data is done, and user
 * should check TX_EMPTY before disabling SPI.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_disable(Spi *p_spi)
{
	p_spi->SPI_CR = SPI_CR_SPIDIS;
}

/**
 * \brief Issue a LASTXFER command.
 *  The next transfer is the last transfer and after that CS is de-asserted.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_set_lastxfer(Spi *p_spi)
{
	p_spi->SPI_CR = SPI_CR_LASTXFER;
}

/**
 * \brief Set SPI to Master mode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_set_master_mode(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_MSTR;
}

/**
 * \brief Set SPI to Slave mode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_set_slave_mode(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_MSTR);
}

/**
 * \brief Get SPI work mode.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 for master mode, 0 for slave mode.
 */
static inline uint32_t spi_get_mode(Spi *p_spi)
{
	if (p_spi->SPI_MR & SPI_MR_MSTR) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Set Variable Peripheral Select.
 * Peripheral Chip Select can be controlled by SPI_TDR.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_set_variable_peripheral_select(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_PS;
}

/**
 * \brief Set Fixed Peripheral Select.
 *  Peripheral Chip Select is controlled by SPI_MR.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_set_fixed_peripheral_select(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_PS);
}

/**
 * \brief Get Peripheral Select mode.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 for Variable mode, 0 for fixed mode.
 */
static inline uint32_t spi_get_peripheral_select_mode(Spi *p_spi)
{
	if (p_spi->SPI_MR & SPI_MR_PS) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable Peripheral Select Decode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_enable_peripheral_select_decode(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_PCSDEC;
}

/**
 * \brief Disable Peripheral Select Decode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_disable_peripheral_select_decode(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_PCSDEC);
}

/**
 * \brief Get Peripheral Select Decode mode.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 for decode mode, 0 for direct mode.
 */
static inline uint32_t spi_get_peripheral_select_decode_setting(Spi *p_spi)
{
	if (p_spi->SPI_MR & SPI_MR_PCSDEC) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable Mode Fault Detection.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_enable_mode_fault_detect(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_MODFDIS);
}

/**
 * \brief Disable Mode Fault Detection.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_disable_mode_fault_detect(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_MODFDIS;
}

/**
 * \brief Check if mode fault detection is enabled.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 for disabled, 0 for enabled.
 */
static inline uint32_t spi_get_mode_fault_detect_setting(Spi *p_spi)
{
	if (p_spi->SPI_MR & SPI_MR_MODFDIS) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable waiting RX_EMPTY before transfer starts.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_enable_tx_on_rx_empty(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_WDRBT;
}

/**
 * \brief Disable waiting RX_EMPTY before transfer starts.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_disable_tx_on_rx_empty(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_WDRBT);
}

/**
 * \brief Check if SPI waits RX_EMPTY before transfer starts.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 for SPI waits, 0 for no wait.
 */
static inline uint32_t spi_get_tx_on_rx_empty_setting(Spi *p_spi)
{
	if (p_spi->SPI_MR & SPI_MR_WDRBT) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable loopback mode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_enable_loopback(Spi *p_spi)
{
	p_spi->SPI_MR |= SPI_MR_LLB;
}

/**
 * \brief Disable loopback mode.
 *
 * \param p_spi Pointer to an SPI instance.
 */
static inline void spi_disable_loopback(Spi *p_spi)
{
	p_spi->SPI_MR &= (~SPI_MR_LLB);
}

void spi_enable_clock(Spi *p_spi);
void spi_disable_clock(Spi *p_spi);
void spi_set_peripheral_chip_select_value(Spi *p_spi, uint32_t ul_value);
void spi_set_delay_between_chip_select(Spi *p_spi, uint32_t ul_delay);
spi_status_t spi_read(Spi *p_spi, uint16_t *us_data, uint8_t *p_pcs);
spi_status_t spi_write(Spi *p_spi, uint16_t us_data, uint8_t uc_pcs,
		uint8_t uc_last);

/**
 * \brief Read status register.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return SPI status register value.
 */
static inline uint32_t spi_read_status(Spi *p_spi)
{
	return p_spi->SPI_SR;
}

/**
 * \brief Test if the SPI is enabled.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 if the SPI is enabled, otherwise 0.
 */
static inline uint32_t spi_is_enabled(Spi *p_spi)
{
	if (p_spi->SPI_SR & SPI_SR_SPIENS) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Put one data to a SPI peripheral.
 *
 * \param p_spi Base address of the SPI instance.
 * \param data The data byte to be loaded
 *
 */
static inline void spi_put(Spi *p_spi, uint16_t data)
{
	p_spi->SPI_TDR = SPI_TDR_TD(data);
}

/** \brief Get one data to a SPI peripheral.
 *
 * \param p_spi Base address of the SPI instance.
 * \return The data byte
 *
 */
static inline uint16_t spi_get(Spi *p_spi)
{
	return (p_spi->SPI_RDR & SPI_RDR_RD_Msk);
}

/**
 * \brief Check if all transmissions are complete.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \retval 1 if transmissions are complete.
 * \retval 0 if transmissions are not complete.
 */
static inline uint32_t spi_is_tx_empty(Spi *p_spi)
{
	if (p_spi->SPI_SR & SPI_SR_TXEMPTY) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Check if all transmissions are ready.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \retval 1 if transmissions are complete.
 * \retval 0 if transmissions are not complete.
 */
static inline uint32_t spi_is_tx_ready(Spi *p_spi)
{
	if (p_spi->SPI_SR & SPI_SR_TDRE) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Check if the SPI contains a received character.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 if the SPI Receive Holding Register is full, otherwise 0.
 */
static inline uint32_t spi_is_rx_full(Spi *p_spi)
{
	if (p_spi->SPI_SR & SPI_SR_RDRF) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Check if all receptions are ready.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return 1 if the SPI Receiver is ready, otherwise 0.
 */
static inline uint32_t spi_is_rx_ready(Spi *p_spi)
{
	if ((p_spi->SPI_SR & (SPI_SR_RDRF | SPI_SR_TXEMPTY))
			== (SPI_SR_RDRF | SPI_SR_TXEMPTY)) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable SPI interrupts.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_sources Interrupts to be enabled.
 */
static inline void spi_enable_interrupt(Spi *p_spi, uint32_t ul_sources)
{
	p_spi->SPI_IER = ul_sources;
}

/**
 * \brief Disable SPI interrupts.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_sources Interrupts to be disabled.
 */
static inline void spi_disable_interrupt(Spi *p_spi, uint32_t ul_sources)
{
	p_spi->SPI_IDR = ul_sources;
}

/**
 * \brief Read SPI interrupt mask.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return The interrupt mask value.
 */
static inline uint32_t spi_read_interrupt_mask(Spi *p_spi)
{
	return p_spi->SPI_IMR;
}

void spi_set_clock_polarity(Spi *p_spi, uint32_t ul_pcs_ch,
		uint32_t ul_polarity);
void spi_set_clock_phase(Spi *p_spi, uint32_t ul_pcs_ch, uint32_t ul_phase);
void spi_configure_cs_behavior(Spi *p_spi, uint32_t ul_pcs_ch,
		uint32_t ul_cs_behavior);
void spi_set_bits_per_transfer(Spi *p_spi, uint32_t ul_pcs_ch, uint32_t ul_bits);
int16_t spi_calc_baudrate_div(const uint32_t baudrate, uint32_t mck);
int16_t spi_set_baudrate_div(Spi *p_spi, uint32_t ul_pcs_ch,
		uint8_t uc_baudrate_divider);
void spi_set_transfer_delay(Spi *p_spi, uint32_t ul_pcs_ch, uint8_t uc_dlybs,
		uint8_t uc_dlybct);

#if (SAM3S || SAM3N || SAM4S || SAM4E || SAM4N || SAM4C || SAMG || SAM4CP || SAM4CM)
/**
 * \brief Get PDC registers base address.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return PDC registers base for PDC driver to access.
 */
static inline Pdc *spi_get_pdc_base(Spi *p_spi)
{
	return (Pdc *)&(p_spi->SPI_RPR);
}
#endif

#if (SAM3U  || SAM3XA || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Get transmit data register address for DMA operation.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return Transmit address for DMA access.
 */
static inline void *spi_get_tx_access(Spi *p_spi)
{
	return (void *)&(p_spi->SPI_TDR);
}

/**
 * \brief Get receive data register address for DMA operation.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return Receive address for DMA access.
 */
static inline void *spi_get_rx_access(Spi *p_spi)
{
	return (void *)&(p_spi->SPI_RDR);
}
#endif

void spi_set_writeprotect(Spi *p_spi, uint32_t ul_enable);
uint32_t spi_get_writeprotect_status(Spi *p_spi);

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \page sam_spi_quickstart Quickstart guide for SAM SPI driver
 *
 * This is the quickstart guide for the \ref spi_group "SAM SPI driver",
 * with step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g.the main application function.
 *
 * \section spi_basic_use_case Basic use case
 * In this basic use case, the SPI module are configured for:
 * - Master mode
 * - Interrupt-based handling
 *
 * \subsection sam_spi_quickstart_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (Sysclock)"
 *
 * \section spi_basic_use_case_setup Setup steps
 * \subsection spi_basic_use_case_setup_code Example code
 * Add to application C-file:
 * \code
	   void spi_master_init(Spi *p_spi)
	   {
	       spi_enable_clock(p_spi);
	       spi_reset(p_spi);
	       spi_set_master_mode(p_spi);
	       spi_disable_mode_fault_detect(p_spi);
	       spi_disable_loopback(p_spi);
	       spi_set_peripheral_chip_select_value(p_spi,
	                                            spi_get_pcs(DEFAULT_CHIP_ID));
	       spi_set_fixed_peripheral_select(p_spi);
	       spi_disable_peripheral_select_decode(p_spi);
	       spi_set_delay_between_chip_select(p_spi, CONFIG_SPI_MASTER_DELAY_BCS);
	   }
	   void spi_master_setup_device(Spi *p_spi, struct spi_device *device,
	       spi_flags_t flags, uint32_t baud_rate, board_spi_select_id_t sel_id)
	   {
	       spi_set_transfer_delay(p_spi, device->id, CONFIG_SPI_MASTER_DELAY_BS,
	                              CONFIG_SPI_MASTER_DELAY_BCT);

	       spi_set_bits_per_transfer(p_spi, device->id, CONFIG_SPI_MASTER_BITS_PER_TRANSFER);
	       spi_set_baudrate_div(p_spi, device->id,
	                            spi_calc_baudrate_div(baud_rate, sysclk_get_peripheral_hz()));

	       spi_configure_cs_behavior(p_spi, device->id, SPI_CS_KEEP_LOW);

	       spi_set_clock_polarity(p_spi, device->id, flags >> 1);
	       spi_set_clock_phase(p_spi, device->id, ((flags & 0x1) ^ 0x1));
	   }
\endcode
 *
 * \subsection spi_basic_use_case_setup_flow Workflow
 * -# Initialize the SPI in master mode:
 *   - \code
	void spi_master_init(SPI_EXAMPLE);
\endcode
 * -# Set up an SPI device:
 *   - \code void spi_master_setup_device(SPI_EXAMPLE, &SPI_DEVICE_EXAMPLE,
	        SPI_MODE_0, SPI_EXAMPLE_BAUDRATE, 0); \endcode
 *   - \note The returned device descriptor structure must be passed to the driver
 *      whenever that device should be used as current slave device.
 * -# Enable SPI module:
 *   - \code spi_enable(SPI_EXAMPLE); \endcode
 */
#endif /* SPI_H_INCLUDED */
