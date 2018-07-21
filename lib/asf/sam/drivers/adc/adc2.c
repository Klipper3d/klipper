/**
 * \file
 *
 * \brief ADC Controller driver.
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
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

#include "adc2.h"
#include "sleepmgr.h"
#include "status_codes.h"
#include "sysclk.h"
#include "pmc.h"

/**
 * \defgroup sam_drivers_adc2_group Analog-to-Digital Controller
 *
 * See \ref sam_adc2_quickstart.
 *
 * Driver for the Analog-to-Digital Controller. This driver provides access to
 * the main features of the ADC controller.
 *
 * @{
 */

/* The number of channel in channel sequence1 register */
#define ADC_SEQ1_CHANNEL_NUM                (8UL)

/* The number of ADC interrupt source */
#define ADC_NUM_OF_INTERRUPT_SOURCE         (24UL)


const uint32_t adc_interrupt_mask[ADC_NUM_OF_INTERRUPT_SOURCE] = {
	ADC_ISR_EOC0, ADC_ISR_EOC1, ADC_ISR_EOC2, ADC_ISR_EOC3,
	ADC_ISR_EOC4, ADC_ISR_EOC5, ADC_ISR_EOC6, ADC_ISR_EOC7,
#if (SAM4N)
	ADC_ISR_EOC8, ADC_ISR_EOC9, ADC_ISR_EOC10, ADC_ISR_EOC11,
	ADC_ISR_EOC12, ADC_ISR_EOC13, ADC_ISR_EOC14, ADC_ISR_EOC15,
	ADC_ISR_EOC16,
#endif
#ifdef TEMP_SENSOR
	ADC_ISR_TEMPCHG,
#endif
	ADC_ISR_EOCAL, ADC_ISR_DRDY,
	ADC_ISR_GOVRE, ADC_ISR_COMPE, ADC_ISR_ENDRX, ADC_ISR_RXBUFF,
};

adc_callback_t adc_callback_pointer[ADC_NUM_OF_INTERRUPT_SOURCE];

/**
 * \brief Get the ADC default configurations.
 *
 * Use to initialize the configuration structure to known default values. This
 * function should be called at the start of any ADC initiation.
 *
 * The default configuration is as follows:
 * - 10-bit resolution
 * - ADC clock frequency is 6MHz
 * - Start Up Time is 64 periods ADC clock
 * - Tracking Time is 3 periods of ADC clock
 * - Transfer Period field shall be programmed with 2 as datasheet said
 * - The controller converts channels in a simple numeric order
 * - Appends the channel number to the conversion result in ADC_LCDR register
 * - Only a Single Trigger is required to get an averaged value
 *
 * \param cfg Pointer to configuration structure to be initiated.
 */
void adc_get_config_defaults(struct adc_config *const cfg)
{
	/* Sanity check argument. */
	Assert(cfg);

#if SAMG55
	cfg->resolution = ADC_12_BITS;
#else
	cfg->resolution = ADC_10_BITS;
#endif
	cfg->mck = sysclk_get_cpu_hz();
	cfg->adc_clock = 6000000UL;
	cfg->startup_time = ADC_STARTUP_TIME_4;
	cfg->tracktim = 2;
	cfg->transfer = 2;
	cfg->useq = false;
	cfg->tag = false;
	cfg->aste = false;
}

/**
 * \internal
 * \brief Configure the ADC Module.
 *
 * \param adc  Base address of the ADC
 * \param config   Configuration for the ADC
 */
static void adc_set_config(Adc *const adc, struct adc_config *config)
{
	uint32_t reg = 0;

	reg =  (config->useq ? ADC_MR_USEQ_REG_ORDER : 0) |
			ADC_MR_PRESCAL(config->mck /
			(2 * config->adc_clock) - 1) |
			ADC_MR_TRACKTIM(config->tracktim) |
			ADC_MR_TRANSFER(config->transfer) |
			(config->startup_time);

	adc->ADC_MR = reg;

	adc->ADC_EMR = (config->tag ? ADC_EMR_TAG : 0) |
			(config->aste ? ADC_EMR_ASTE_SINGLE_TRIG_AVERAGE : 0);

	adc_set_resolution(adc, config->resolution);
}

#ifdef TEMP_SENSOR
/**
 * \brief Get the ADC Temperature Sensor default configurations.
 *
 * Use to initialize the configuration structure to known default values.
 *
 * The default configuration is as follows:
 * - Generates an event when the converted data is in the comparison window
 * - The window range is 0xFF ~ 0xFFF
 *
 * \param cfg Pointer to temperature sensor configuration structure
 *        to be initiated.
 */
void adc_temp_sensor_get_config_defaults(
		struct adc_temp_sensor_config *const cfg)
{
	/*Sanity check argument. */
	Assert(cfg);

	cfg->tempon = true;
	cfg->mode = ADC_TEMP_CMP_MODE_2;
	cfg->low_threshold = 0xFF;
	cfg->high_threshold = 0xFFF;
}

/**
 * \brief Configure the ADC temperature sensor.
 *
 * \param adc  Base address of the ADC
 * \param config   Configuration for the ADC temperature sensor
 */
void adc_temp_sensor_set_config(Adc *const adc,
		struct adc_temp_sensor_config *config)
{
	uint32_t reg = 0;

	reg = ((config->tempon) ? ADC_TEMPMR_TEMPON : 0) | (config->mode);
	adc->ADC_TEMPMR = reg;

	adc->ADC_TEMPCWR = ADC_TEMPCWR_TLOWTHRES(config->low_threshold) |
			ADC_TEMPCWR_THIGHTHRES(config->high_threshold);
}
#endif

#if (SAMG)
/**
 * \brief Get the Last Channel Specific Measurement default configurations.
 *
 * Use to initialize the configuration structure to known default values.
 *
 * The default configuration is as follows:
 * - Generates an event when the converted data is in the comparison window
 * - The window range is 0xFF ~ 0xFFF
 *
 * \param cfg Pointer to last channel configuration structure
 *        to be initiated.
 */
void adc_last_channel_get_config_defaults(
		struct adc_last_channel_config *const cfg)
{
	/*Sanity check argument. */
	Assert(cfg);

	cfg->dual_trig_on = true;
	cfg->mode = ADC_LAST_CHANNEL_CMP_MODE_2;
	cfg->low_threshold = 0xFF;
	cfg->high_threshold = 0xFFF;
}

/**
 * \brief Configure the ADC Last Channel Specific Measurement.
 *
 * \param adc  Base address of the ADC
 * \param config   Configuration for the last channel
 */
void adc_last_channel_set_config(Adc *const adc,
		struct adc_last_channel_config *config)
{
	uint32_t reg = 0;

	reg = ((config->dual_trig_on) ? ADC_LCTMR_DUALTRIG : 0) | (config->mode);
	adc->ADC_LCTMR = reg;

	adc->ADC_LCCWR = ADC_LCCWR_LOWTHRES(config->low_threshold) |
			ADC_LCCWR_HIGHTHRES(config->high_threshold);
}
#endif

/**
 * \brief Initialize the ADC Module.
 *
 * \param adc  Base address of the ADC
 * \param config   Configuration for the ADC
 *
 * \retval STATUS_OK  Initialization is finished.
 * \retval STATUS_ERR_BUSY  Initialization failed.
 */
enum status_code adc_init(Adc *const adc, struct adc_config *config)
{
	Assert(adc);
	Assert(config);

	if ((adc_get_interrupt_status(adc) & ADC_ISR_DRDY) == ADC_ISR_DRDY) {
		return STATUS_ERR_BUSY;
	}

	/* Reset and configure the ADC module */
	adc->ADC_CR = ADC_CR_SWRST;
	adc_set_config(adc, config);

	uint32_t i;
	for (i = 0; i < ADC_NUM_OF_INTERRUPT_SOURCE; i++) {
		adc_callback_pointer[i] = 0;
	}

	return STATUS_OK;
}

/**
 * \brief Configure conversion resolution.
 *
 * \param adc  Base address of the ADC.
 * \param res Conversion resolution.
 *
 */
void adc_set_resolution(Adc *const adc,
		const enum adc_resolution res)
{
#if SAMG55
	adc->ADC_EMR |= res;
#else
	if (res == ADC_11_BITS || res == ADC_12_BITS) {
		adc->ADC_MR &= ~ADC_MR_LOWRES;
		adc->ADC_EMR |= res;
	} else {
		adc->ADC_MR |= res;
		adc->ADC_EMR &= ~ADC_EMR_OSR_Msk;
	}
#endif
}

/**
 * \brief Configure comparison mode.
 *
 * \param adc  Base address of the ADC.
 * \param mode Comparison mode.
 * \param channel Comparison Selected Channel.
 * \param cmp_filter Compare Event Filtering.
 */
void adc_set_comparison_mode(Adc *const adc,
		const enum adc_cmp_mode mode,
		const enum adc_channel_num channel, uint8_t cmp_filter)
{
	if (channel != ADC_CHANNEL_ALL) {
		adc_ch_sanity_check(adc, channel);
	}

	uint32_t reg;

	reg = adc->ADC_EMR;

	reg &= ~(ADC_EMR_CMPSEL_Msk |
			ADC_EMR_CMPMODE_Msk |
			ADC_EMR_CMPFILTER_Msk);
	reg |= mode |
			((channel == ADC_CHANNEL_ALL) ? ADC_EMR_CMPALL
			: ADC_EMR_CMPSEL(channel)) |
			ADC_EMR_CMPFILTER(cmp_filter);

	adc->ADC_EMR = reg;
}

/**
 * \brief Configure ADC power mode.
 *
 * \param adc  Base address of the ADC.
 * \param mode   ADC power mode value.
 */
void adc_set_power_mode(Adc *const adc,
		const enum adc_power_mode mode)
{
	uint32_t reg;

	reg = adc->ADC_MR;

	switch (mode) {
	case ADC_POWER_MODE_0:
		reg |= ADC_MR_SLEEP_NORMAL;
		break;

	case ADC_POWER_MODE_1:
		reg |= ADC_MR_SLEEP_SLEEP;
		break;
	}

	adc->ADC_MR = reg;
}

/**
 * \brief Set callback for ADC
 *
 * \param adc  Base address of the ADC
 * \param source    Interrupt source
 * \param callback  Callback function pointer
 * \param irq_level Interrupt level
 */
void adc_set_callback(Adc *const adc, enum adc_interrupt_source source,
		adc_callback_t callback, uint8_t irq_level)
{
	Assert(adc);
	Assert(callback);

	adc_callback_pointer[source] = callback;
	irq_register_handler(ADC_IRQn, irq_level);

	/* Enable the specified interrupt source */
	adc_enable_interrupt(adc, source);
}

/**
 * \brief Enable ADC interrupts.
 *
 * \param adc  Base address of the ADC.
 * \param interrupt_source Interrupts to be enabled.
 */
void adc_enable_interrupt(Adc *const adc,
		enum adc_interrupt_source interrupt_source)
{
	if (interrupt_source == ADC_INTERRUPT_ALL) {
		adc->ADC_IER = ADC_INTERRUPT_ALL;
		return;
	} else {
		adc->ADC_IER = adc_interrupt_mask[interrupt_source];
	}
}

/**
 * \brief Disable ADC interrupts.
 *
 * \param adc  Base address of the ADC.
 * \param interrupt_source Interrupts to be disabled.
 */
void adc_disable_interrupt(Adc *const adc,
		enum adc_interrupt_source interrupt_source)
{
	if (interrupt_source == ADC_INTERRUPT_ALL) {
		adc->ADC_IDR = ADC_INTERRUPT_ALL;
		return;
	} else {
		adc->ADC_IDR = adc_interrupt_mask[interrupt_source];
	}
}

/**
 * \internal
 * \brief Common ADC interrupt handler
 *
 * The optional callback used by the interrupt handler is set by the
 * adc_set_callback() function.
 *
 * \param inst_num ADC instance number to handle interrupt for
 * \param source   Interrupt source number
 */
static void adc_interrupt(enum adc_interrupt_source source)
{
	if (adc_callback_pointer[source]) {
		adc_callback_pointer[source]();
	}
}

/**
 * \internal
 * \brief Call the callback function if the corresponding interrupt is asserted
 *
 * \param adc  Base address of the ADC.
 */
static void adc_process_callback(Adc *const adc)
{
	volatile uint32_t status;
	uint32_t cnt;

	status = adc_get_interrupt_status(adc);

	for (cnt = 0; cnt < ADC_NUM_OF_INTERRUPT_SOURCE; cnt++) {
		if (status & adc_interrupt_mask[cnt]) {
			adc_interrupt((enum adc_interrupt_source)cnt);
		}
	}
}

/**
 * \brief Interrupt handler for ADC.
 */
void ADC_Handler(void)
{
	adc_process_callback(ADC);
}

/**
 * \brief Enable ADC Module.
 *
 */
void adc_enable(void)
{
	/* Enable peripheral clock. */
	pmc_enable_periph_clk(ID_ADC);
	sleepmgr_lock_mode(SLEEP_MODE_ADC);
}

/**
 * \brief Disable ADC Module.
 *
 */
void adc_disable(void)
{
	/* Disable peripheral clock. */
	pmc_disable_periph_clk(ID_ADC);
	sleepmgr_unlock_mode(SLEEP_MODE_ADC);
}

/**
 * \brief Configure conversion sequence.
 *
 * \param adc  Base address of the ADC.
 * \param ch_list Channel sequence list.
 * \param uc_num Number of channels in the list.
 */
void adc_configure_sequence(Adc *const adc,
		const enum adc_channel_num ch_list[], uint8_t uc_num)
{
	uint8_t uc_counter;

	/* Set user sequence mode */
	adc->ADC_MR |= ADC_MR_USEQ_REG_ORDER;
#if (SAM4N)
	adc->ADC_SEQR1 = 0;
	adc->ADC_SEQR2 = 0;
#endif
#if (SAMG)
	adc->ADC_SEQR1 = 0;
#endif

	if (uc_num <= ADC_SEQ1_CHANNEL_NUM) {
		for (uc_counter = 0; uc_counter < uc_num; uc_counter++) {
#if (SAM4N || SAMG)
			adc->ADC_SEQR1
#endif
				|= ch_list[uc_counter] << (4 * uc_counter);
		}
	} else {
		for (uc_counter = 0; uc_counter <= ADC_SEQ1_CHANNEL_NUM;
				uc_counter++) {
#if (SAM4N || SAMG)
			adc->ADC_SEQR1
#endif
				|= ch_list[uc_counter] << (4 * uc_counter);
		}
		for (uc_counter = 0; uc_counter < uc_num - ADC_SEQ1_CHANNEL_NUM;
				uc_counter++) {
#if (SAM4N)
			adc->ADC_SEQR2 |= ch_list[8 + uc_counter] << (4 * uc_counter);
#endif
		}
	}
}

//@}
