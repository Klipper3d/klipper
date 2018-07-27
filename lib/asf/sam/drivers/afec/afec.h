/**
 * \file
 *
 * \brief Analog-Front-End Controller driver for SAM.
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

#ifndef AFEC_H_INCLUDED
#define AFEC_H_INCLUDED

#include "compiler.h"
#include "status_codes.h"

#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
/** Definitions for AFEC resolution */
enum afec_resolution {
	AFEC_12_BITS = AFEC_EMR_RES_NO_AVERAGE,    /* AFEC 12-bit resolution */
	AFEC_13_BITS = AFEC_EMR_RES_OSR4,          /* AFEC 13-bit resolution */
	AFEC_14_BITS = AFEC_EMR_RES_OSR16,         /* AFEC 14-bit resolution */
	AFEC_15_BITS = AFEC_EMR_RES_OSR64,         /* AFEC 15-bit resolution */
	AFEC_16_BITS = AFEC_EMR_RES_OSR256         /* AFEC 16-bit resolution */
};
#else
/** Definitions for AFEC resolution */
enum afec_resolution {
	AFEC_10_BITS = AFEC_EMR_RES_LOW_RES,       /* AFEC 10-bit resolution */
	AFEC_12_BITS = AFEC_EMR_RES_NO_AVERAGE,    /* AFEC 12-bit resolution */
	AFEC_13_BITS = AFEC_EMR_RES_OSR4,          /* AFEC 13-bit resolution */
	AFEC_14_BITS = AFEC_EMR_RES_OSR16,         /* AFEC 14-bit resolution */
	AFEC_15_BITS = AFEC_EMR_RES_OSR64,         /* AFEC 15-bit resolution */
	AFEC_16_BITS = AFEC_EMR_RES_OSR256         /* AFEC 16-bit resolution */
};
#endif
/** Definitions for AFEC power mode */
enum afec_power_mode {
	/* AFEC core on and reference voltage circuitry on */
	AFEC_POWER_MODE_0 = 0,
	/* AFEC core off and reference voltage circuitry on */
	AFEC_POWER_MODE_1,
	/* AFEC core off and reference voltage circuitry off */
	AFEC_POWER_MODE_2
};

/** Definitions for AFEC trigger */
enum afec_trigger {
	/* Starting a conversion is only possible by software. */
	AFEC_TRIG_SW = AFEC_MR_TRGEN_DIS,
	/* External trigger */
	AFEC_TRIG_EXT = AFEC_MR_TRGSEL_AFEC_TRIG0 | AFEC_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 0 */
	AFEC_TRIG_TIO_CH_0 = AFEC_MR_TRGSEL_AFEC_TRIG1 | AFEC_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 1 */
	AFEC_TRIG_TIO_CH_1 = AFEC_MR_TRGSEL_AFEC_TRIG2 | AFEC_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 2 */
	AFEC_TRIG_TIO_CH_2 = AFEC_MR_TRGSEL_AFEC_TRIG3 | AFEC_MR_TRGEN,
	/* PWM Event Line 0 */
	AFEC_TRIG_PWM_EVENT_LINE_0 = AFEC_MR_TRGSEL_AFEC_TRIG4 | AFEC_MR_TRGEN,
	/* PWM Event Line 1 */
	AFEC_TRIG_PWM_EVENT_LINE_1 = AFEC_MR_TRGSEL_AFEC_TRIG5 | AFEC_MR_TRGEN,
#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
	/*Analog Comparator*/
	AFEC_TRIG_ANALOG_COMPARATOR = AFEC_MR_TRGSEL_AFEC_TRIG6 | AFEC_MR_TRGEN,
#endif	
	/* Freerun mode conversion. */
	AFEC_TRIG_FREERUN = 0xFF,
};

#if defined __SAM4E8C__  || defined __SAM4E16C__ || defined __SAM4E8CB__  || defined __SAM4E16CB__
/** Definitions for AFEC channel number */
enum afec_channel_num {
	AFEC_CHANNEL_0 = 0,
	AFEC_CHANNEL_1,
	AFEC_CHANNEL_2,
	AFEC_CHANNEL_3,
	AFEC_CHANNEL_4,
	AFEC_CHANNEL_5,
	AFEC_TEMPERATURE_SENSOR = 15,
	AFEC_CHANNEL_ALL = 0x803F,
} ;
#elif defined __SAM4E8E__  || defined __SAM4E16E__
/** Definitions for AFEC channel number */
enum afec_channel_num {
	AFEC_CHANNEL_0 = 0,
	AFEC_CHANNEL_1,
	AFEC_CHANNEL_2,
	AFEC_CHANNEL_3,
	AFEC_CHANNEL_4,
	AFEC_CHANNEL_5,
	AFEC_CHANNEL_6,
	AFEC_CHANNEL_7,
	AFEC_CHANNEL_8,
	AFEC_CHANNEL_9,
	AFEC_CHANNEL_10,
	AFEC_CHANNEL_11,
	AFEC_CHANNEL_12,
	AFEC_CHANNEL_13,
	AFEC_CHANNEL_14,
	AFEC_TEMPERATURE_SENSOR,
	AFEC_CHANNEL_ALL = 0xFFFF,
} ;
#elif (SAMV71 || SAMV70 || SAME70 || SAMS70)
/** Definitions for AFEC channel number */
enum afec_channel_num {
	AFEC_CHANNEL_0 = 0,
	AFEC_CHANNEL_1,
	AFEC_CHANNEL_2,
	AFEC_CHANNEL_3,
	AFEC_CHANNEL_4,
	AFEC_CHANNEL_5,
	AFEC_CHANNEL_6,
	AFEC_CHANNEL_7,
	AFEC_CHANNEL_8,
	AFEC_CHANNEL_9,
	AFEC_CHANNEL_10,
	AFEC_TEMPERATURE_SENSOR,
	AFEC_CHANNEL_ALL = 0x0FFF,
} ;

#define NB_CH_AFE0  (12UL)
#define NB_CH_AFE1  (12UL)
#endif

#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
enum afec_sample_hold_mode {
	/*Single Sample-and-Hold mode*/
	AFEC_SAMPLE_HOLD_MODE_0,
	/*Dual Sample-and-Hold mode*/
	AFEC_SAMPLE_HOLD_MODE_1,
};
#endif

/** Definitions for AFEC gain value */
enum afec_gainvalue {
	AFEC_GAINVALUE_0 = 0,
	AFEC_GAINVALUE_1 = 1,
	AFEC_GAINVALUE_2 = 2,
	AFEC_GAINVALUE_3 = 3
};

/** Definitions for AFEC Start Up Time */
enum afec_startup_time {
	AFEC_STARTUP_TIME_0 = AFEC_MR_STARTUP_SUT0,
	AFEC_STARTUP_TIME_1 = AFEC_MR_STARTUP_SUT8,
	AFEC_STARTUP_TIME_2 = AFEC_MR_STARTUP_SUT16,
	AFEC_STARTUP_TIME_3 = AFEC_MR_STARTUP_SUT24,
	AFEC_STARTUP_TIME_4 = AFEC_MR_STARTUP_SUT64,
	AFEC_STARTUP_TIME_5 = AFEC_MR_STARTUP_SUT80,
	AFEC_STARTUP_TIME_6 = AFEC_MR_STARTUP_SUT96,
	AFEC_STARTUP_TIME_7 = AFEC_MR_STARTUP_SUT112,
	AFEC_STARTUP_TIME_8 = AFEC_MR_STARTUP_SUT512,
	AFEC_STARTUP_TIME_9 = AFEC_MR_STARTUP_SUT576,
	AFEC_STARTUP_TIME_10 = AFEC_MR_STARTUP_SUT640,
	AFEC_STARTUP_TIME_11 = AFEC_MR_STARTUP_SUT704,
	AFEC_STARTUP_TIME_12 = AFEC_MR_STARTUP_SUT768,
	AFEC_STARTUP_TIME_13 = AFEC_MR_STARTUP_SUT832,
	AFEC_STARTUP_TIME_14 = AFEC_MR_STARTUP_SUT896,
	AFEC_STARTUP_TIME_15 = AFEC_MR_STARTUP_SUT960
};

#if SAM4E
/** Definitions for AFEC analog settling time */
enum afec_settling_time {
	AFEC_SETTLING_TIME_0 = AFEC_MR_SETTLING_AST3,
	AFEC_SETTLING_TIME_1 = AFEC_MR_SETTLING_AST5,
	AFEC_SETTLING_TIME_2 = AFEC_MR_SETTLING_AST9,
	AFEC_SETTLING_TIME_3 = AFEC_MR_SETTLING_AST17
};
#endif

/** Definitions for Comparison Mode */
enum afec_cmp_mode {
	AFEC_CMP_MODE_0 = AFEC_EMR_CMPMODE_LOW,
	AFEC_CMP_MODE_1 = AFEC_EMR_CMPMODE_HIGH,
	AFEC_CMP_MODE_2 = AFEC_EMR_CMPMODE_IN,
	AFEC_CMP_MODE_3 = AFEC_EMR_CMPMODE_OUT
};

/** Definitions for Temperature Comparison Mode */
enum afec_temp_cmp_mode {
	AFEC_TEMP_CMP_MODE_0 = AFEC_TEMPMR_TEMPCMPMOD_LOW,
	AFEC_TEMP_CMP_MODE_1 = AFEC_TEMPMR_TEMPCMPMOD_HIGH,
	AFEC_TEMP_CMP_MODE_2 = AFEC_TEMPMR_TEMPCMPMOD_IN,
	AFEC_TEMP_CMP_MODE_3 = AFEC_TEMPMR_TEMPCMPMOD_OUT
};

/**
 * \brief Analog-Front-End Controller configuration structure.
 *
 * Configuration structure for a Analog-Front-End Controller instance.
 * This structure could be initialized by the \ref afec_get_config_defaults()
 * function before being modified by the user application.
 */
struct afec_config {
	/** Resolution */
	enum afec_resolution resolution;
	/** Master Clock */
	uint32_t mck;
	/** AFEC Clock */
	uint32_t afec_clock;
	/** Start Up Time */
	enum afec_startup_time startup_time;
	#if  SAM4E
		/** Analog Settling Time = (settling_time + 1) / AFEC clock */
		enum afec_settling_time settling_time;
	#endif
	/** Tracking Time = tracktim / AFEC clock */
	uint8_t tracktim;
	/** Transfer Period = (transfer * 2 + 3) / AFEC clock */
	uint8_t transfer;
	/** Analog Change */
	bool anach;
	/** Use Sequence Enable */
	bool useq;
	/** TAG of AFE_LDCR register */
	bool tag;
	/** Single Trigger Mode */
	bool stm;
	/** AFE Bias Current Control */
	uint8_t ibctl;
};

/** AFEC channel configuration structure.*/
struct afec_ch_config {
	/** Differential Mode */
	bool diff;
	/** Gain Value */
	enum afec_gainvalue gain;
};

/** AFEC Temperature Sensor configuration structure.*/
struct afec_temp_sensor_config {
	/** RTC Trigger mode */
	bool rctc;
	/** Temperature Comparison Mode */
	enum afec_temp_cmp_mode mode;
	/** Temperature Low Threshold */
	uint16_t low_threshold;
	/** Temperature High Threshold */
	uint16_t high_threshold;
};

#if defined __SAM4E8C__  || defined __SAM4E16C__ || defined __SAM4E8CB__  || defined __SAM4E16CB__
/** AFEC interrupt source type */
enum afec_interrupt_source {
	AFEC_INTERRUPT_EOC_0 = 0,
	AFEC_INTERRUPT_EOC_1,
	AFEC_INTERRUPT_EOC_2,
	AFEC_INTERRUPT_EOC_3,
	AFEC_INTERRUPT_EOC_4,
	AFEC_INTERRUPT_EOC_5,
	AFEC_INTERRUPT_EOC_15,
	AFEC_INTERRUPT_DATA_READY,
	AFEC_INTERRUPT_OVERRUN_ERROR,
	AFEC_INTERRUPT_COMP_ERROR,
	AFEC_INTERRUPT_END_RXBUF,
	AFEC_INTERRUPT_RXBUF_FULL,
	AFEC_INTERRUPT_TEMP_CHANGE,
	AFEC_INTERRUPT_END_CAL,
	_AFEC_NUM_OF_INTERRUPT_SOURCE,
	AFEC_INTERRUPT_ALL = 0xDF00803F,
};
#elif defined __SAM4E8E__  || defined __SAM4E16E__
/** AFEC interrupt source type */
enum afec_interrupt_source {
	AFEC_INTERRUPT_EOC_0 = 0,
	AFEC_INTERRUPT_EOC_1,
	AFEC_INTERRUPT_EOC_2,
	AFEC_INTERRUPT_EOC_3,
	AFEC_INTERRUPT_EOC_4,
	AFEC_INTERRUPT_EOC_5,
	AFEC_INTERRUPT_EOC_6,
	AFEC_INTERRUPT_EOC_7,
	AFEC_INTERRUPT_EOC_8,
	AFEC_INTERRUPT_EOC_9,
	AFEC_INTERRUPT_EOC_10,
	AFEC_INTERRUPT_EOC_11,
	AFEC_INTERRUPT_EOC_12,
	AFEC_INTERRUPT_EOC_13,
	AFEC_INTERRUPT_EOC_14,
	AFEC_INTERRUPT_EOC_15,
	AFEC_INTERRUPT_DATA_READY,
	AFEC_INTERRUPT_OVERRUN_ERROR,
	AFEC_INTERRUPT_COMP_ERROR,
	AFEC_INTERRUPT_END_RXBUF,
	AFEC_INTERRUPT_RXBUF_FULL,
	AFEC_INTERRUPT_TEMP_CHANGE,
	AFEC_INTERRUPT_END_CAL,
	_AFEC_NUM_OF_INTERRUPT_SOURCE,
	AFEC_INTERRUPT_ALL = 0xDF00FFFF,
};
#elif (SAMV71 || SAMV70 || SAME70 || SAMS70)
/** AFEC interrupt source type */
enum afec_interrupt_source {
	AFEC_INTERRUPT_EOC_0 = 0,
	AFEC_INTERRUPT_EOC_1,
	AFEC_INTERRUPT_EOC_2,
	AFEC_INTERRUPT_EOC_3,
	AFEC_INTERRUPT_EOC_4,
	AFEC_INTERRUPT_EOC_5,
	AFEC_INTERRUPT_EOC_6,
	AFEC_INTERRUPT_EOC_7,
	AFEC_INTERRUPT_EOC_8,
	AFEC_INTERRUPT_EOC_9,
	AFEC_INTERRUPT_EOC_10,
	AFEC_INTERRUPT_EOC_11,
	AFEC_INTERRUPT_DATA_READY,
	AFEC_INTERRUPT_OVERRUN_ERROR,
	AFEC_INTERRUPT_COMP_ERROR,
	AFEC_INTERRUPT_TEMP_CHANGE,
	_AFEC_NUM_OF_INTERRUPT_SOURCE,
	AFEC_INTERRUPT_ALL = 0x47000FFF,
};
#endif

typedef void (*afec_callback_t)(void);

void afec_get_config_defaults(struct afec_config *const cfg);
void afec_ch_get_config_defaults(struct afec_ch_config *const cfg);
void afec_temp_sensor_get_config_defaults(
		struct afec_temp_sensor_config *const cfg);
enum status_code afec_init(Afec *const afec, struct afec_config *const config);
void afec_temp_sensor_set_config(Afec *const afec,
		struct afec_temp_sensor_config *config);
void afec_ch_set_config(Afec *const afec, const enum afec_channel_num channel,
		struct afec_ch_config *config);
void afec_configure_sequence(Afec *const afec,
		const enum afec_channel_num ch_list[], const uint8_t uc_num);
void afec_enable(Afec *const afec);
void afec_disable(Afec *const afec);
void afec_set_callback(Afec *const afec, enum afec_interrupt_source source,
		afec_callback_t callback, uint8_t irq_level);

#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
void afec_configure_auto_error_correction(Afec *const afec,
		const enum afec_channel_num channel,int16_t offsetcorr, uint16_t gaincorr);

uint32_t afec_get_correction_value(Afec *const afec,
	   const enum afec_channel_num afec_ch);
void afec_set_sample_hold_mode(Afec *const afec,
		const enum afec_channel_num channel,const enum afec_sample_hold_mode mode);

#endif
/**
 * \internal
 * \brief AFEC channel sanity check
 *
 * \param afec  Base address of the AFEC.
 * \param channel  AFEC channel number.
 *
 */
static inline void afec_ch_sanity_check(Afec *const afec,
		const enum afec_channel_num channel)
{
	if (afec == AFEC0) {
	#if defined __SAM4E8C__  || defined __SAM4E16C__ || defined __SAM4E8CB__  || defined __SAM4E16CB__
		Assert((channel < NB_CH_AFE0) || (channel == AFEC_TEMPERATURE_SENSOR));
	#elif defined __SAM4E8E__  || defined __SAM4E16E__
		Assert(channel < NB_CH_AFE0);
	#elif (SAMV71 || SAMV70 || SAME70 || SAMS70)
		Assert(channel < NB_CH_AFE0);
	#endif
	} else if (afec == AFEC1) {
		Assert(channel < NB_CH_AFE1);
	}
	UNUSED(channel);
}

/**
 * \brief Configure conversion trigger and free run mode.
 *
 * \param afec  Base address of the AFEC.
 * \param trigger Conversion trigger.
 *
 */
static inline void afec_set_trigger(Afec *const afec,
		const enum afec_trigger trigger)
{
	uint32_t reg;

	reg = afec->AFEC_MR;

	if (trigger == AFEC_TRIG_FREERUN) {
		reg |= AFEC_MR_FREERUN_ON;
	} else {
		reg &= ~(AFEC_MR_TRGSEL_Msk | AFEC_MR_TRGEN | AFEC_MR_FREERUN_ON);
		reg |= trigger;
	}

	afec->AFEC_MR = reg;
}

/**
 * \brief Configure conversion resolution.
 *
 * \param afec  Base address of the AFEC.
 * \param res Conversion resolution.
 *
 */
static inline void afec_set_resolution(Afec *const afec,
		const enum afec_resolution res)
{
	uint32_t reg;

	reg = afec->AFEC_EMR;

	reg &= ~AFEC_EMR_RES_Msk;
	reg |= res;

	afec->AFEC_EMR = reg;
}

void afec_set_comparison_mode(Afec *const afec,
		const enum afec_cmp_mode mode, const enum afec_channel_num channel,
		uint8_t cmp_filter);

/**
 * \brief Get comparison mode.
 *
 * \param afec  Base address of the AFEC.
 *
 * \retval Compare mode value.
 */
static inline enum afec_cmp_mode afec_get_comparison_mode(Afec *const afec)
{
	return (enum afec_cmp_mode)(afec->AFEC_EMR & AFEC_EMR_CMPMODE_Msk);
}

/**
 * \brief Configure AFEC compare window.
 *
 * \param afec  Base address of the AFEC.
 * \param us_low_threshold Low threshold of compare window.
 * \param us_high_threshold High threshold of compare window.
 */
static inline void afec_set_comparison_window(Afec *const afec,
		const uint16_t us_low_threshold, const uint16_t us_high_threshold)
{
	afec->AFEC_CWR = AFEC_CWR_LOWTHRES(us_low_threshold) |
			AFEC_CWR_HIGHTHRES(us_high_threshold);
}

/**
 * \brief Enable or disable write protection of AFEC registers.
 *
 * \param afec  Base address of the AFEC.
 * \param is_enable 1 to enable, 0 to disable.
 */
static inline void afec_set_writeprotect(Afec *const afec,
		const bool is_enable)
{
	if (is_enable) {
		afec->AFEC_WPMR = AFEC_WPMR_WPEN | AFEC_WPMR_WPKEY_PASSWD;
	} else {
		afec->AFEC_WPMR = AFEC_WPMR_WPKEY_PASSWD;
	}
}

/**
 * \brief Indicate write protect status.
 *
 * \param afec  Base address of the AFEC.
 *
 * \return 0 if no write protect violation occurred, or 16-bit write protect
 * violation source.
 */
static inline uint32_t afec_get_writeprotect_status(Afec *const afec)
{
	uint32_t reg_value;

	reg_value = afec->AFEC_WPSR;
	if (reg_value & AFEC_WPSR_WPVS) {
		return (reg_value & AFEC_WPSR_WPVSRC_Msk) >> AFEC_WPSR_WPVSRC_Pos;
	} else {
		return 0;
	}
}

/**
 * \brief Get AFEC overrun error status.
 *
 * \param afec  Base address of the AFEC.
 *
 * \return AFEC overrun error status.
 */
static inline uint32_t afec_get_overrun_status(Afec *const afec)
{
	return afec->AFEC_OVER;
}

/**
 * \brief Start analog-to-digital conversion.
 *
 * \note If one of the hardware event is selected as AFEC trigger,
 * this function can NOT start analog to digital conversion.
 *
 * \param afec  Base address of the AFEC.
 */
static inline void afec_start_software_conversion(Afec *const afec)
{
	afec->AFEC_CR = AFEC_CR_START;
}

void afec_set_power_mode(Afec *const afec,
		const enum afec_power_mode mode);

/**
 * \brief Enable the specified AFEC channel.
 *
 * \param afec  Base address of the AFEC.
 * \param afec_ch AFEC channel number.
 */
static inline void afec_channel_enable(Afec *const afec,
		const enum afec_channel_num afec_ch)
{
	if (afec_ch != AFEC_CHANNEL_ALL) {
		afec_ch_sanity_check(afec, afec_ch);
	}

	afec->AFEC_CHER = (afec_ch == AFEC_CHANNEL_ALL) ?
			AFEC_CHANNEL_ALL : 1 << afec_ch;
}

/**
 * \brief Disable the specified AFEC channel.
 *
 * \param afec  Base address of the AFEC.
 * \param afec_ch AFEC channel number.
 */
static inline void afec_channel_disable(Afec *const afec,
		const enum afec_channel_num afec_ch)
{
	if (afec_ch != AFEC_CHANNEL_ALL) {
		afec_ch_sanity_check(afec, afec_ch);
	}

	afec->AFEC_CHDR = (afec_ch == AFEC_CHANNEL_ALL) ?
			AFEC_CHANNEL_ALL : 1 << afec_ch;
}

/**
 * \brief Get the AFEC channel status.
 *
 * \param afec  Base address of the AFEC.
 * \param afec_ch AFEC channel number.
 *
 * \retval 1 if channel is enabled.
 * \retval 0 if channel is disabled.
 */
static inline uint32_t afec_channel_get_status(Afec *const afec,
		const enum afec_channel_num afec_ch)
{
	afec_ch_sanity_check(afec, afec_ch);

	return afec->AFEC_CHSR & (1 << afec_ch);
}

/**
 * \brief Read the Converted Data of the selected channel.
 *
 * \param afec  Base address of the AFEC.
 * \param afec_ch AFEC channel number.
 *
 * \return AFEC converted value of the selected channel.
 */
static inline uint32_t afec_channel_get_value(Afec *const afec,
		enum afec_channel_num afec_ch)
{
	afec_ch_sanity_check(afec, afec_ch);

	afec->AFEC_CSELR = afec_ch;
	return afec->AFEC_CDR;
}

/**
 * \brief Set analog offset to be used for channel CSEL.
 *
 * \param afec  Base address of the AFEC.
 * \param afec_ch AFEC channel number.
 * \param aoffset  Analog offset value.
 */
static inline void afec_channel_set_analog_offset(Afec *const afec,
		enum afec_channel_num afec_ch, uint16_t aoffset)
{
	afec_ch_sanity_check(afec, afec_ch);

	afec->AFEC_CSELR = afec_ch;
	afec->AFEC_COCR = (aoffset & AFEC_COCR_AOFF_Msk);
}

/**
 * \brief Get the Last Data Converted.
 *
 * \param afec  Base address of the AFEC.
 *
 * \return AFEC latest converted value.
 */
static inline uint32_t afec_get_latest_value(Afec *const afec)
{
	return afec->AFEC_LCDR & AFEC_LCDR_LDATA_Msk;
}

/**
 * \brief Get the Last Converted Channel Number.
 *
 * \param afec  Base address of the AFEC.
 *
 * \return AFEC Last Converted Channel Number.
 */
static inline uint32_t afec_get_latest_chan_num(Afec *const afec)
{
	return (afec->AFEC_LCDR & AFEC_LCDR_CHNB_Msk) >> AFEC_LCDR_CHNB_Pos;
}

void afec_enable_interrupt(Afec *const afec,
		enum afec_interrupt_source interrupt_source);

void afec_disable_interrupt(Afec *const afec,
		enum afec_interrupt_source interrupt_source);

/**
 * \brief Get AFEC interrupt status.
 *
 * \param afec  Base address of the AFEC.
 *
 * \return The interrupt status value.
 */
static inline uint32_t afec_get_interrupt_status(Afec *const afec)
{
	return afec->AFEC_ISR;
}

/**
 * \brief Get AFEC interrupt mask.
 *
 * \param afec  Base address of the AFEC.
 *
 * \return The interrupt mask value.
 */
static inline uint32_t afec_get_interrupt_mask(Afec *const afec)
{
	return afec->AFEC_IMR;
}

#if SAM4E
/**
 * \brief Get PDC registers base address.
 *
 * \param afec  Base address of the AFEC.
 *
 * \return AFEC PDC register base address.
 */
static inline Pdc *afec_get_pdc_base(Afec *const afec)
{
	Pdc *p_pdc_base = NULL;

#ifdef PDC_AFEC0
	if (afec == AFEC0) {
		p_pdc_base = PDC_AFEC0;
	}
#endif

#ifdef PDC_AFEC1
	else if (afec == AFEC1) {
		p_pdc_base = PDC_AFEC1;
	}
#endif

	return p_pdc_base;
}

/**
 * \brief Launch an automatic calibration of the AFEC on next sequence.
 *
 * \param afec  Base address of the AFEC.
 *
 * \retval STATUS_OK  An automatic calibration is launched.
 * \retval STATUS_ERR_BUSY  Automatic calibration can not be launched because
 *         the AFEC is in freerun mode.
 */
static inline enum status_code afec_start_calibration(Afec *const afec)
{
	uint32_t reg;

	reg = afec->AFEC_CHSR;
	afec->AFEC_CDOR = reg;

	if ((afec->AFEC_MR & AFEC_MR_FREERUN) == AFEC_MR_FREERUN_ON) {
		return STATUS_ERR_BUSY;
	}
	afec->AFEC_CR = AFEC_CR_AUTOCAL;
	return STATUS_OK;
}
#endif

/**
 * \page sam_afec_quickstart Quickstart guide for SAM AFEC driver
 *
 * This is the quickstart guide for the \ref sam_drivers_afec_group
 * "SAM AFEC driver" with step-by-step instructions on how to configure and use
 * the driver in a selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section afec_basic_use_case Basic use case
 * In this basic use case, the AFEC module and single channel are configured for:
 * - 12 -bit resolution
 * - AFEC clock frequency is 6MHz
 * - Start Up Time is 64 periods AFEC clock
 * - Analog Settling Time is 3 periods of AFEC clock
 * - Tracking Time is 3 periods of AFEC clock
 * - Transfer Period is 5 periods AFEC clock
 * - Allows different analog settings for each channel
 * - The controller converts channels in a simple numeric order
 * - Appends the channel number to the conversion result in AFE_LDCR register
 * - Only a Single Trigger is required to get an averaged value
 * - AFE Bias Current Control value is 1
 * - Software triggering of conversions
 * - Single channel measurement
 * - AFEC_CHANNEL_5 of AFEC0 as input
 *
 * \subsection sam_afec_quickstart_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (Sysclock)"
 *
 * \section afec_basic_use_case_setup Setup steps
 * \subsection afec_basic_use_case_setup_code Example code
 * Add to application C-file:
 * \code
	afec_enable(AFEC0);
	afec_get_config_defaults(&afec_cfg);
	afec_init(AFEC0, &afec_cfg);
	afec_set_trigger(AFEC0, AFEC_TRIG_SW);
	afec_channel_enable(AFEC0, AFEC_CHANNEL_5);
\endcode
 *
 * \subsection afec_basic_use_case_setup_flow Workflow
 * -# Enable AFEC Module:
 *   - \code afec_enable(AFEC0); \endcode
 * -# Get the AFEC default configurations:
 *   - \code afec_get_config_defaults(&afec_cfg); \endcode
 * -# Initialize the AFEC Module:
 *   - \code  afec_init(AFEC0, &afec_cfg); \endcode
 * -# Configure conversion trigger and free run mode:
 *   - \code afec_set_trigger(AFEC0, AFEC_TRIG_SW); \endcode
 * -# Enable Channel:
 *   - \code afec_channel_enable(AFEC0, AFEC_CHANNEL_5); \endcode
 *
 * \section afec_basic_use_case_usage Usage steps
 * \subsection afec_basic_use_case_usage_code Example code
 * Add to, e.g., main loop in application C-file:
 * \code
	afec_start_software_conversion(AFEC0);
	while (afec_get_interrupt_status(AFEC0) & (1 << AFEC_CHANNEL_5));
	uint32_t result = afec_channel_get_value(AFEC0, AFEC_CHANNEL_5);
\endcode
 *
 * \subsection afec_basic_use_case_usage_flow Workflow
 * -# Start AFEC conversion on channel:
 *   - \code afec_start_software_conversion(AFEC0); \endcode
 * -# Wait for the conversion over:
 *   - \code while (afec_get_interrupt_status(AFEC0) & (1 << AFEC_CHANNEL_5));
\endcode
 * -# Get the conversion result:
 *   - \code uint32_t result = afec_channel_get_value(AFEC0, AFEC_CHANNEL_5);
\endcode
 */
#endif /* AFEC_H_INCLUDED */
