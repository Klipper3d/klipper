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

#ifndef ADC2_H_INCLUDED
#define ADC2_H_INCLUDED

#include "compiler.h"
#include "status_codes.h"

#if (SAM4N)
#define TEMP_SENSOR
#define SLEEP_MODE_ADC    SLEEPMGR_SLEEP_WFI
#endif

#if (SAMG)
#define NO_TEMP_SENSOR
#define SLEEP_MODE_ADC    SLEEPMGR_ACTIVE
#endif

/** Write Protect Key */
#ifndef ADC_WPMR_WPKEY_PASSWD
#define ADC_WPMR_WPKEY_PASSWD    (0x414443u << 8)
#endif

/** Definitions for ADC resolution */
enum adc_resolution {
#if SAMG55
	ADC_12_BITS = ADC_EMR_OSR_NO_AVERAGE,     /* ADC 12-bit resolution */
	ADC_13_BITS = ADC_EMR_OSR_OSR4,           /* ADC 13-bit resolution */
	ADC_14_BITS = ADC_EMR_OSR_OSR16,          /* ADC 14-bit resolution */
	ADC_15_BITS = ADC_EMR_OSR_OSR64,          /* ADC 15-bit resolution */
	ADC_16_BITS = ADC_EMR_OSR_OSR256,         /* ADC 16-bit resolution */
#else
	ADC_8_BITS = ADC_MR_LOWRES_BITS_8,        /* ADC 8-bit resolution */
	ADC_10_BITS = ADC_MR_LOWRES_BITS_10,      /* ADC 10-bit resolution */
	ADC_11_BITS = ADC_EMR_OSR_OSR4,           /* ADC 11-bit resolution */
	ADC_12_BITS = ADC_EMR_OSR_OSR16           /* ADC 12-bit resolution */
#endif
};

/** Definitions for ADC power mode */
enum adc_power_mode {
	/* ADC core on and reference voltage circuitry on */
	ADC_POWER_MODE_0 = 0,
	/* ADC core off and reference voltage circuitry off */
	ADC_POWER_MODE_1
};

/** Definitions for ADC trigger */
enum adc_trigger {
	/* Starting a conversion is only possible by software. */
	ADC_TRIG_SW = ADC_MR_TRGEN_DIS,
	/* External trigger */
	ADC_TRIG_EXT = ADC_MR_TRGSEL_ADC_TRIG0 | ADC_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 0 */
	ADC_TRIG_TIO_CH_0 = ADC_MR_TRGSEL_ADC_TRIG1 | ADC_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 1 */
	ADC_TRIG_TIO_CH_1 = ADC_MR_TRGSEL_ADC_TRIG2 | ADC_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 2 */
	ADC_TRIG_TIO_CH_2 = ADC_MR_TRGSEL_ADC_TRIG3 | ADC_MR_TRGEN,
#if (SAMG)
	/* RTCOUT0 */
	ADC_TRIG_RTC_0 = ADC_MR_TRGSEL_ADC_TRIG4 | ADC_MR_TRGEN,
	/* RTTINC */
	ADC_TRIG_RTT = ADC_MR_TRGSEL_ADC_TRIG5 | ADC_MR_TRGEN,
#endif
	/* Freerun mode conversion. */
	ADC_TRIG_FREERUN = 0xFF
};

/** Definitions for ADC channel number */
enum adc_channel_num {
	ADC_CHANNEL_0 = 0,
	ADC_CHANNEL_1,
	ADC_CHANNEL_2,
	ADC_CHANNEL_3,
	ADC_CHANNEL_4,
	ADC_CHANNEL_5,
	ADC_CHANNEL_6,
	ADC_CHANNEL_7,
#if (SAM4N)
	ADC_CHANNEL_8,
	ADC_CHANNEL_9,
	ADC_CHANNEL_10,
	ADC_CHANNEL_11,
	ADC_CHANNEL_12,
	ADC_CHANNEL_13,
	ADC_CHANNEL_14,
	ADC_CHANNEL_15,
#endif
#ifdef TEMP_SENSOR
	ADC_TEMPERATURE_SENSOR,
#endif
	ADC_CHANNEL_ALL = 0xFFFF
};

/** Definitions for ADC Start Up Time */
enum adc_startup_time {
	ADC_STARTUP_TIME_0 = ADC_MR_STARTUP_SUT0,
	ADC_STARTUP_TIME_1 = ADC_MR_STARTUP_SUT8,
	ADC_STARTUP_TIME_2 = ADC_MR_STARTUP_SUT16,
	ADC_STARTUP_TIME_3 = ADC_MR_STARTUP_SUT24,
	ADC_STARTUP_TIME_4 = ADC_MR_STARTUP_SUT64,
	ADC_STARTUP_TIME_5 = ADC_MR_STARTUP_SUT80,
	ADC_STARTUP_TIME_6 = ADC_MR_STARTUP_SUT96,
	ADC_STARTUP_TIME_7 = ADC_MR_STARTUP_SUT112,
	ADC_STARTUP_TIME_8 = ADC_MR_STARTUP_SUT512,
	ADC_STARTUP_TIME_9 = ADC_MR_STARTUP_SUT576,
	ADC_STARTUP_TIME_10 = ADC_MR_STARTUP_SUT640,
	ADC_STARTUP_TIME_11 = ADC_MR_STARTUP_SUT704,
	ADC_STARTUP_TIME_12 = ADC_MR_STARTUP_SUT768,
	ADC_STARTUP_TIME_13 = ADC_MR_STARTUP_SUT832,
	ADC_STARTUP_TIME_14 = ADC_MR_STARTUP_SUT896,
	ADC_STARTUP_TIME_15 = ADC_MR_STARTUP_SUT960
};

/** Definitions for Comparison Mode */
enum adc_cmp_mode {
	ADC_CMP_MODE_0 = ADC_EMR_CMPMODE_LOW,
	ADC_CMP_MODE_1 = ADC_EMR_CMPMODE_HIGH,
	ADC_CMP_MODE_2 = ADC_EMR_CMPMODE_IN,
	ADC_CMP_MODE_3 = ADC_EMR_CMPMODE_OUT
};

#ifdef TEMP_SENSOR
/** Definitions for Temperature Comparison Mode */
enum adc_temp_cmp_mode {
	ADC_TEMP_CMP_MODE_0 = ADC_TEMPMR_TEMPCMPMOD_LOW,
	ADC_TEMP_CMP_MODE_1 = ADC_TEMPMR_TEMPCMPMOD_HIGH,
	ADC_TEMP_CMP_MODE_2 = ADC_TEMPMR_TEMPCMPMOD_IN,
	ADC_TEMP_CMP_MODE_3 = ADC_TEMPMR_TEMPCMPMOD_OUT
};
#endif
#if (SAMG)
/** Definitions for Last Channel Specific Measurement Comparison Mode */
enum adc_last_channel_cmp_mode {
	ADC_LAST_CHANNEL_CMP_MODE_0 = ADC_LCTMR_CMPMOD_LOW,
	ADC_LAST_CHANNEL_CMP_MODE_1 = ADC_LCTMR_CMPMOD_HIGH,
	ADC_LAST_CHANNEL_CMP_MODE_2 = ADC_LCTMR_CMPMOD_IN,
	ADC_LAST_CHANNEL_CMP_MODE_3 = ADC_LCTMR_CMPMOD_OUT
};
#endif

/** Definitions for Reference Voltage Selection */
enum adc_refer_voltage_source {
	ADC_REFER_VOL_EXTERNAL = 0,
	ADC_REFER_VOL_STUCK_AT_MIN,
	ADC_REFER_VOL_VDDANA,
	ADC_REFER_VOL_IRVS
};

/**
 * \brief ADC Enhanced configuration structure.
 *
 * Configuration structure for a ADC Enhanced instance.
 * This structure could be initialized by the \ref ADC_get_config_defaults()
 * function before being modified by the user application.
 */
struct adc_config {
	/** Resolution */
	enum adc_resolution resolution;
	/** Master Clock */
	uint32_t mck;
	/** ADC Clock */
	uint32_t adc_clock;
	/** Start Up Time */
	enum adc_startup_time startup_time;
	/** Tracking Time = (tracktim+1) / ADC clock */
	uint8_t tracktim;
	/** Transfer Period */
	uint8_t transfer;
	/** Use Sequence Enable */
	bool useq;
	/** TAG of ADC_LDCR register */
	bool tag;
	/** Averaging on Single Trigger Event */
	bool aste;
};

#ifdef TEMP_SENSOR
/** ADC Temperature Sensor configuration structure.*/
struct adc_temp_sensor_config {
	/** Temperature Sensor On */
	bool tempon;
	/** Temperature Comparison Mode */
	enum adc_temp_cmp_mode mode;
	/** Temperature Low Threshold */
	uint16_t low_threshold;
	/** Temperature High Threshold */
	uint16_t high_threshold;
};
#endif

#if (SAMG)
/** ADC Last Channel Specific Measurement configuration structure.*/
struct adc_last_channel_config {
	/** Specific Measurement On */
	bool dual_trig_on;
	/** Specific Measurement Comparison Mode */
	enum adc_last_channel_cmp_mode mode;
	/** Specific Measurement Low Threshold */
	uint16_t low_threshold;
	/** Specific Measurement High Threshold */
	uint16_t high_threshold;
};
#endif

/** ADC interrupt source type */
enum adc_interrupt_source {
	ADC_INTERRUPT_EOC_0 = 0,
	ADC_INTERRUPT_EOC_1,
	ADC_INTERRUPT_EOC_2,
	ADC_INTERRUPT_EOC_3,
	ADC_INTERRUPT_EOC_4,
	ADC_INTERRUPT_EOC_5,
	ADC_INTERRUPT_EOC_6,
	ADC_INTERRUPT_EOC_7,
#if (SAM4N)
	ADC_INTERRUPT_EOC_8,
	ADC_INTERRUPT_EOC_9,
	ADC_INTERRUPT_EOC_10,
	ADC_INTERRUPT_EOC_11,
	ADC_INTERRUPT_EOC_12,
	ADC_INTERRUPT_EOC_13,
	ADC_INTERRUPT_EOC_14,
	ADC_INTERRUPT_EOC_15,
	ADC_INTERRUPT_EOC_16,
#endif
#ifdef TEMP_SENSOR
	ADC_INTERRUPT_TEMP_CHANGE,
#endif
	ADC_INTERRUPT_END_CAL,
	ADC_INTERRUPT_DATA_READY,
	ADC_INTERRUPT_OVERRUN_ERROR,
	ADC_INTERRUPT_COMP_ERROR,
	ADC_INTERRUPT_END_RXBUF,
	ADC_INTERRUPT_RXBUF_FULL,
	ADC_INTERRUPT_ALL = 0xFFFFFFFF
};

typedef void (*adc_callback_t)(void);

void adc_get_config_defaults(struct adc_config *const cfg);
enum status_code adc_init(Adc *const adc, struct adc_config *const config);

#ifdef TEMP_SENSOR
void adc_temp_sensor_get_config_defaults(
		struct adc_temp_sensor_config *const cfg);
void adc_temp_sensor_set_config(Adc *const adc,
		struct adc_temp_sensor_config *config);
#endif
#if (SAMG)
void adc_last_channel_get_config_defaults(
		struct adc_last_channel_config *const cfg);
void adc_last_channel_set_config(Adc *const adc,
		struct adc_last_channel_config *config);
#endif

void adc_configure_sequence(Adc *const adc,
		const enum adc_channel_num ch_list[], const uint8_t uc_num);
void adc_enable(void);
void adc_disable(void);
void adc_set_callback(Adc *const adc, enum adc_interrupt_source source,
		adc_callback_t callback, uint8_t irq_level);

/**
 * \internal
 * \brief ADC channel sanity check
 *
 * \param adc  Base address of the ADC.
 * \param channel  Adc channel number.
 *
 */
static inline void adc_ch_sanity_check(Adc *const adc,
		const enum adc_channel_num channel)
{
	if (adc == ADC) {
		Assert((channel < NB_CH_ADC)
#ifdef TEMP_SENSOR
				||(channel == ADC_TEMPERATURE_SENSOR)
#endif
				);
	}

	UNUSED(channel);
}

#if (SAMG)
#if SAMG55
/**
 * \brief Configure ADC clock to mck.
 *
 * \param adc  Base address of the ADC.
 *
 */
static inline void adc_select_clock_source_mck(Adc *const adc)
{
	uint32_t reg;

	reg = adc->ADC_EMR;

	reg &= ~ADC_EMR_SRCCLK_PMC_PCK;

	adc->ADC_EMR = reg;
}

/**
 * \brief Configure ADC clock to pck.
 *
 * \param adc  Base address of the ADC.
 *
 */
static inline void adc_select_clock_source_pck(Adc *const adc)
{
	uint32_t reg;

	reg = adc->ADC_EMR;

	reg |= ADC_EMR_SRCCLK_PMC_PCK;

	adc->ADC_EMR = reg;
}

#else
/**
 * \brief Configure ADC clock to MCK.
 *
 * \param adc  Base address of the ADC.
 *
 */
static inline void adc_set_clock_mck(Adc *const adc)
{
	uint32_t reg;

	reg = adc->ADC_EMR;

	reg |= ADC_MR_DIV1;

	adc->ADC_MR = reg;
}

/**
 * \brief Configure ADC clock to MCK/3.
 *
 * \param adc  Base address of the ADC.
 *
 */
static inline void adc_set_clock_mck_div3(Adc *const adc)
{
	uint32_t reg;

	reg = adc->ADC_MR;

	reg &= ~ADC_MR_DIV1;
	reg |= ADC_MR_DIV3;

	adc->ADC_MR = reg;
}
#endif
#endif

/**
 * \brief Configure conversion trigger and free run mode.
 *
 * \param adc  Base address of the ADC.
 * \param trigger Conversion trigger.
 *
 */
static inline void adc_set_trigger(Adc *const adc,
		const enum adc_trigger trigger)
{
	uint32_t reg;

	reg = adc->ADC_MR;

	if (trigger == ADC_TRIG_FREERUN) {
		reg |= ADC_MR_FREERUN_ON;
	} else {
		reg &= ~(ADC_MR_TRGSEL_Msk | ADC_MR_TRGEN | ADC_MR_FREERUN_ON);
		reg |= trigger;
	}

	adc->ADC_MR = reg;
}

void adc_set_resolution(Adc *const adc,
		const enum adc_resolution res);

void adc_set_comparison_mode(Adc *const adc,
		const enum adc_cmp_mode mode,
		const enum adc_channel_num channel,
		uint8_t cmp_filter);

/**
 * \brief Get comparison mode.
 *
 * \param adc  Base address of the ADC.
 *
 * \retval Compare mode value.
 */
static inline enum adc_cmp_mode adc_get_comparison_mode(Adc *const adc)
{
	return (enum adc_cmp_mode)(adc->ADC_EMR & ADC_EMR_CMPMODE_Msk);
}

/**
 * \brief Configure ADC compare window.
 *
 * \param adc  Base address of the ADC.
 * \param us_low_threshold Low threshold of compare window.
 * \param us_high_threshold High threshold of compare window.
 */
static inline void adc_set_comparison_window(Adc *const adc,
		const uint16_t us_low_threshold,
		const uint16_t us_high_threshold)
{
	adc->ADC_CWR = ADC_CWR_LOWTHRES(us_low_threshold) |
			ADC_CWR_HIGHTHRES(us_high_threshold);
}

/**
 * \brief Enable or disable write protection of ADC registers.
 *
 * \param adc  Base address of the ADC.
 * \param is_enable 1 to enable, 0 to disable.
 */
static inline void adc_set_writeprotect(Adc *const adc,
		const bool is_enable)
{
	if (is_enable) {
		adc->ADC_WPMR = ADC_WPMR_WPEN | ADC_WPMR_WPKEY_PASSWD;
	} else {
		adc->ADC_WPMR = ADC_WPMR_WPKEY_PASSWD;
	}
}

/**
 * \brief Indicate write protect status.
 *
 * \param adc  Base address of the ADC.
 *
 * \return 0 if no write protect violation occurred, or 16-bit write protect
 * violation source.
 */
static inline uint32_t adc_get_writeprotect_status(Adc *const adc)
{
	uint32_t reg_value;

	reg_value = adc->ADC_WPSR;
	if (reg_value & ADC_WPSR_WPVS) {
		return (reg_value & ADC_WPSR_WPVSRC_Msk) >> ADC_WPSR_WPVSRC_Pos;
	} else {
		return 0;
	}
}

/**
 * \brief Get ADC overrun error status.
 *
 * \param adc  Base address of the ADC.
 *
 * \return ADC overrun error status.
 */
static inline uint32_t adc_get_overrun_status(Adc *const adc)
{
	return adc->ADC_OVER;
}

/**
 * \brief Set ADC averaging on single trigger event
 *
 * \param adc Base address of the ADC.
 */
static inline void adc_average_on_single_trigger(Adc *const adc)
{
	adc->ADC_EMR |= ADC_EMR_ASTE_SINGLE_TRIG_AVERAGE;
}

/**
 * \brief Set ADC averaging on serval trigger events
 *
 * \param adc Base address of the ADC.
 */
static inline void adc_average_on_multi_trigger(Adc *const adc)
{
	adc->ADC_EMR &= ~ADC_EMR_ASTE_SINGLE_TRIG_AVERAGE;
}

/**
 * \brief Start analog-to-digital conversion.
 *
 * \note If one of the hardware event is selected as ADC trigger,
 * this function can NOT start analog to digital conversion.
 *
 * \param adc  Base address of the ADC.
 */
static inline void adc_start_software_conversion(Adc *const adc)
{
	adc->ADC_CR = ADC_CR_START;
}

void adc_set_power_mode(Adc *const adc,
		const enum adc_power_mode mode);

/**
 * \brief Enable the specified ADC channel.
 *
 * \param adc  Base address of the ADC.
 * \param adc_ch Adc channel number.
 */
static inline void adc_channel_enable(Adc *const adc,
		const enum adc_channel_num adc_ch)
{
	if (adc_ch != ADC_CHANNEL_ALL) {
		adc_ch_sanity_check(adc, adc_ch);
	}

	adc->ADC_CHER = (adc_ch == ADC_CHANNEL_ALL) ?
			ADC_CHANNEL_ALL : 1 << adc_ch;
}

/**
 * \brief Disable the specified ADC channel.
 *
 * \param adc  Base address of the ADC.
 * \param adc_ch Adc channel number.
 */
static inline void adc_channel_disable(Adc *const adc,
		const enum adc_channel_num adc_ch)
{
	if (adc_ch != ADC_CHANNEL_ALL) {
		adc_ch_sanity_check(adc, adc_ch);
	}

	adc->ADC_CHDR = (adc_ch == ADC_CHANNEL_ALL) ?
			ADC_CHANNEL_ALL : 1 << adc_ch;
}

/**
 * \brief Get the ADC channel status.
 *
 * \param adc  Base address of the ADC.
 * \param adc_ch Adc channel number.
 *
 * \retval 1 if channel is enabled.
 * \retval 0 if channel is disabled.
 */
static inline uint32_t adc_channel_get_status(Adc *const adc,
		const enum adc_channel_num adc_ch)
{
	adc_ch_sanity_check(adc, adc_ch);

	return adc->ADC_CHSR & (1 << adc_ch);
}

/**
 * \brief Read the Converted Data of the selected channel.
 *
 * \param adc  Base address of the ADC.
 * \param adc_ch Adc channel number.
 *
 * \return ADC converted value of the selected channel.
 */
static inline uint32_t adc_channel_get_value(Adc *const adc,
		enum adc_channel_num adc_ch)
{
	adc_ch_sanity_check(adc, adc_ch);

	return adc->ADC_CDR[adc_ch];
}

/**
 * \brief Get the Last Data Converted.
 *
 * \param adc  Base address of the ADC.
 *
 * \return ADC latest converted value.
 */
static inline uint32_t adc_get_latest_value(Adc *const adc)
{
	return adc->ADC_LCDR & ADC_LCDR_LDATA_Msk;
}

/**
 * \brief Get the Last Converted Channel Number.
 *
 * \param adc Base address of the ADC.
 *
 * \return ADC Last Converted Channel Number.
 */
static inline uint32_t adc_get_latest_chan_num(Adc *const adc)
{
#if SAMG55
	return (adc->ADC_LCDR & ADC_LCDR_CHNBOSR_Msk) >> ADC_LCDR_CHNBOSR_Pos;
#else
	return (adc->ADC_LCDR & ADC_LCDR_CHNB_Msk) >> ADC_LCDR_CHNB_Pos;
#endif
}

void adc_enable_interrupt(Adc *const adc,
		enum adc_interrupt_source interrupt_source);

void adc_disable_interrupt(Adc *const adc,
		enum adc_interrupt_source interrupt_source);

/**
 * \brief Get ADC interrupt status.
 *
 * \param adc  Base address of the ADC.
 *
 * \return The interrupt status value.
 */
static inline uint32_t adc_get_interrupt_status(Adc *const adc)
{
	return adc->ADC_ISR;
}

/**
 * \brief Get ADC interrupt mask.
 *
 * \param adc  Base address of the ADC.
 *
 * \return The interrupt mask value.
 */
static inline uint32_t adc_get_interrupt_mask(Adc *const adc)
{
	return adc->ADC_IMR;
}

/**
 * \brief Get PDC registers base address.
 *
 * \param adc  Base address of the ADC.
 *
 * \return Adc Pdc register base address.
 */
static inline Pdc *adc_get_pdc_base(Adc *const adc)
{
	Pdc *p_pdc_base = NULL;

	if (adc == ADC) {
		p_pdc_base = PDC_ADC;
	}

	return p_pdc_base;
}

/**
 * \brief Launch an automatic calibration of the ADC on next sequence.
 *
 * \param adc  Base address of the ADC.
 *
 * \retval STATUS_OK  An automatic calibration is launched.
 * \retval STATUS_ERR_BUSY  Automatic calibration can not be launched because
 *         the ADC is in freerun mode.
 */
static inline enum status_code adc_start_calibration(Adc *const adc)
{
	if ((adc->ADC_MR & ADC_MR_FREERUN) == ADC_MR_FREERUN_ON) {
		return STATUS_ERR_BUSY;
	}

	adc->ADC_CR = ADC_CR_AUTOCAL;
	return STATUS_OK;
}

#if (SAM4N)
/**
 * \brief ADC Reference Voltage Selection
 *
 * \param  adc  Base address of the ADC.
 * \param  adc_ref_src The source selection for ADC reference voltage, 
 * ADC_REFER_VOL_EXTERNAL - the external pin ADVREF defines the voltage reference.
 * ADC_REFER_VOL_STUCK_AT_MIN - the internal reference voltage is stuck at the minimum value
 * ADC_REFER_VOL_VDDANA - the internal voltage reference is forced to VDDANA. Effective only if ONREF is 1.
 * ADC_REFER_VOL_IRVS - the internal reference voltage is defined by field IRVS
 * See the product electrical characteristics for further details.
 * \param  irvs Internal reference volatage selection, only be effective when
 *         adc_ref_src equals to ADC_REFER_VOL_IRVS
 */
static inline void adc_ref_vol_sel(Adc *const adc,
		enum adc_refer_voltage_source adc_ref_src,
		uint8_t irvs)
{
	if (ADC_REFER_VOL_EXTERNAL == adc_ref_src) {
		adc->ADC_ACR &= ~ADC_ACR_ONREF_EN;
	} else if (ADC_REFER_VOL_STUCK_AT_MIN == adc_ref_src) {
		adc->ADC_ACR |= ADC_ACR_ONREF_EN;
		adc->ADC_ACR &= ~(ADC_ACR_IRVCE_EN | ADC_ACR_FORCEREF_EN);
	} else if (ADC_REFER_VOL_VDDANA == adc_ref_src) {
		adc->ADC_ACR |= ADC_ACR_ONREF_EN | ADC_ACR_FORCEREF_EN;
	} else if (ADC_REFER_VOL_IRVS == adc_ref_src) {
		adc->ADC_ACR &= ~ADC_ACR_FORCEREF_EN;
		adc->ADC_ACR |= ADC_ACR_ONREF_EN | ADC_ACR_IRVCE_EN |
				(irvs << ADC_ACR_IRVS_Pos);
	}
}
#endif

/**
 * \page sam_adc2_quickstart Quickstart guide for ADC driver
 *
 * This is the quickstart guide for the \ref sam_drivers_adc2_group
 * "ADC2 driver" with step-by-step instructions on how to configure and use
 * the driver in a selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section adc_basic_use_case Basic use case
 * In this basic use case, the ADC module and single channel are configured for:
 * - 10 -bit resolution
 * - ADC clock frequency is 6MHz
 * - Start Up Time is 64 periods ADC clock
 * - Tracking Time is 3 periods of ADC clock
 * - Transfer Period field shall be programmed with 2 as datasheet said
 * - The controller converts channels in a simple numeric order
 * - Appends the channel number to the conversion result in AFE_LDCR register
 * - Single Trigger is optional to get an averaged value
 * - Software triggering of conversions
 * - Single channel measurement
 * - ADC_CHANNEL_1 of ADC as input
 *
 * \subsection sam_adc2_quickstart_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (Sysclock)"
 *
 * \section adc_basic_use_case_setup Setup steps
 * \subsection adc_basic_use_case_setup_code Example code
 * Add to application C-file:
 * \code
	adc_enable();
	adc_get_config_defaults(&adc_cfg);
	adc_init(ADC, &adc_cfg);
	adc_set_trigger(ADC, ADC_TRIG_SW);
	adc_channel_enable(ADC, ADC_CHANNEL_1);
\endcode
 *
 * \subsection adc_basic_use_case_setup_flow Workflow
 * -# Enable ADC Module:
 *   - \code adc_enable(); \endcode
 * -# Get the ADC default configurations:
 *   - \code adc_get_config_defaults(&adc_cfg); \endcode
 * -# Initialize the ADC Module:
 *   - \code adc_init(ADC, &adc_cfg); \endcode
 * -# Configure conversion trigger and free run mode:
 *   - \code adc_set_trigger(ADC, ADC_TRIG_SW); \endcode
 * -# Enable Channel:
 *   - \code adc_channel_enable(ADC, ADC_CHANNEL_1); \endcode
 *
 * \section adc_basic_use_case_usage Usage steps
 * \subsection adc_basic_use_case_usage_code Example code
 * Add to, e.g., main loop in application C-file:
 * \code
	adc_start_software_conversion(ADC);
	while (adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_1));
	uint32_t result = adc_channel_get_value(ADC, ADC_CHANNEL_1);
\endcode
 *
 * \subsection adc_basic_use_case_usage_flow Workflow
 * -# Start ADC conversion on channel:
 *   - \code adc_start_software_conversion(ADC); \endcode
 * -# Wait for the conversion over:
 *   - \code while (adc_get_interrupt_status(ADC) & (1 << ADC_CHANNEL_1));
\endcode
 * -# Get the conversion result:
 *   - \code uint32_t result = adc_channel_get_value(ADC, ADC_CHANNEL_1);
\endcode
 */
#endif /* ADC2_H_INCLUDED */
