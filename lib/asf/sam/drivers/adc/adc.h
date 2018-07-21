/**
 * \file
 *
 * \brief Analog-to-Digital Converter (ADC/ADC12B) driver for SAM.
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

#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/* The max ADC clock freq definition*/
#if !SAM4C && !SAM4CP && !SAM4CM
#define ADC_FREQ_MAX   20000000
#else
#define ADC_FREQ_MAX   16000000
#endif
/* The min ADC clock freq definition*/
#define ADC_FREQ_MIN    1000000
/* The normal ADC startup time*/
#define ADC_STARTUP_NORM     40
/* The fast ADC startup time*/
#define ADC_STARTUP_FAST     12

/* Definitions for ADC resolution */
#if SAM3S || SAM4S || SAM3XA
enum adc_resolution_t {
	ADC_10_BITS = ADC_MR_LOWRES_BITS_10,    /* ADC 10-bit resolution */
	ADC_12_BITS = ADC_MR_LOWRES_BITS_12     /* ADC 12-bit resolution */
};
#elif SAM3N
enum adc_resolution_t {
	ADC_8_BITS = ADC_MR_LOWRES_BITS_8,      /* ADC 8-bit resolution */
	ADC_10_BITS = ADC_MR_LOWRES_BITS_10     /* ADC 10-bit resolution */
};
#elif SAM3U
enum adc_resolution_t {
	ADC_8_BITS = ADC_MR_LOWRES_BITS_8,      /* ADC 8-bit resolution */
	ADC_10_BITS = ADC12B_MR_LOWRES_BITS_10, /* ADC 10-bit resolution */
	ADC_12_BITS = ADC12B_MR_LOWRES_BITS_12  /* ADC 12-bit resolution */
};
#elif SAM4C || SAM4CP || SAM4CM
enum adc_resolution_t {
	ADC_8_BITS  = ADC_MR_LOWRES_BITS_8,  /* ADC 8-bit resolution */
	ADC_10_BITS = ADC_MR_LOWRES_BITS_10, /* ADC 10-bit resolution */
	ADC_11_BITS = ADC_EMR_OSR_OSR4,      /* ADC 11-bit resolution */
	ADC_12_BITS = ADC_EMR_OSR_OSR16      /* ADC 12-bit resolution */
};
#endif

/* Definitions for ADC trigger */
enum adc_trigger_t {
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
#if SAM4C || SAM4CP || SAM4CM
	/* TIO Output of the Timer Counter Channel 3 */
	ADC_TRIG_TIO_CH_3 = ADC_MR_TRGSEL_ADC_TRIG3 | ADC_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 4 */
	ADC_TRIG_TIO_CH_4 = ADC_MR_TRGSEL_ADC_TRIG4 | ADC_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 5 */
	ADC_TRIG_TIO_CH_5 = ADC_MR_TRGSEL_ADC_TRIG5 | ADC_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 6 */
	ADC_TRIG_TIO_CH_6 = ADC_MR_TRGSEL_ADC_TRIG6 | ADC_MR_TRGEN,
#endif
#if SAM3S || SAM4S || SAM3XA || SAM3U
	/* PWM Event Line 0 */
	ADC_TRIG_PWM_EVENT_LINE_0 = ADC_MR_TRGSEL_ADC_TRIG4 | ADC_MR_TRGEN,
	/* PWM Event Line 1 */
	ADC_TRIG_PWM_EVENT_LINE_1 = ADC_MR_TRGSEL_ADC_TRIG5 | ADC_MR_TRGEN
#endif
};

#if SAM3U
/* Definitions for ADC trigger */
enum adc12b_trigger_t {
	/* Starting a conversion is only possible by software. */
	ADC12B_TRIG_SW = ADC12B_MR_TRGEN_DIS,
	/* External trigger */
	ADC12B_TRIG_EXT = ADC12B_MR_TRGSEL_ADC_TRIG0 | ADC12B_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 0 */
	ADC12B_TRIG_TIO_CH_0 = ADC12B_MR_TRGSEL_ADC_TRIG1 | ADC12B_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 1 */
	ADC12B_TRIG_TIO_CH_1 = ADC12B_MR_TRGSEL_ADC_TRIG2 | ADC12B_MR_TRGEN,
	/* TIO Output of the Timer Counter Channel 2 */
	ADC12B_TRIG_TIO_CH_2 = ADC12B_MR_TRGSEL_ADC_TRIG3 | ADC12B_MR_TRGEN,
	/* PWM Event Line 0 */
	ADC12B_TRIG_PWM_EVENT_LINE_0 = ADC12B_MR_TRGSEL_ADC_TRIG4 | ADC12B_MR_TRGEN,
	/* PWM Event Line 1 */
	ADC12B_TRIG_PWM_EVENT_LINE_1 = ADC12B_MR_TRGSEL_ADC_TRIG5 | ADC12B_MR_TRGEN
};
#endif

#if SAM3S || SAM4S ||  SAM3N || SAM3XA || SAM4C || SAM4CP || SAM4CM
/* Definitions for ADC channel number */
enum adc_channel_num_t {
	ADC_CHANNEL_0  = 0,
	ADC_CHANNEL_1  = 1,
	ADC_CHANNEL_2  = 2,
	ADC_CHANNEL_3  = 3,
	ADC_CHANNEL_4  = 4,
	ADC_CHANNEL_5  = 5,
	ADC_CHANNEL_6  = 6,
#if SAM4C || SAM4CP || SAM4CM
	ADC_TEMPERATURE_SENSOR = 7,
#else
	ADC_CHANNEL_7  = 7,
	ADC_CHANNEL_8  = 8,
	ADC_CHANNEL_9  = 9,
	ADC_CHANNEL_10 = 10,
	ADC_CHANNEL_11 = 11,
	ADC_CHANNEL_12 = 12,
	ADC_CHANNEL_13 = 13,
	ADC_CHANNEL_14 = 14,
	ADC_TEMPERATURE_SENSOR = 15,
#endif
};
#elif SAM3U
/* Definitions for ADC channel number */
enum adc_channel_num_t {
	ADC_CHANNEL_0  = 0,
	ADC_CHANNEL_1  = 1,
	ADC_CHANNEL_2  = 2,
	ADC_CHANNEL_3  = 3,
	ADC_CHANNEL_4  = 4,
	ADC_CHANNEL_5  = 5,
	ADC_CHANNEL_6  = 6,
	ADC_CHANNEL_7  = 7,
};
#endif
#if !SAM4C && !SAM4CP && !SAM4CM
/* Definitions for ADC gain value */
enum adc_gainvalue_t {
	ADC_GAINVALUE_0 = 0,
	ADC_GAINVALUE_1 = 1,
	ADC_GAINVALUE_2 = 2,
	ADC_GAINVALUE_3 = 3
};
#endif
/* Definitions for ADC analog settling time */
#if SAM3S || SAM4S ||  SAM3XA
enum adc_settling_time_t {
	ADC_SETTLING_TIME_0 = ADC_MR_SETTLING_AST3,
	ADC_SETTLING_TIME_1 = ADC_MR_SETTLING_AST5,
	ADC_SETTLING_TIME_2 = ADC_MR_SETTLING_AST9,
	ADC_SETTLING_TIME_3 = ADC_MR_SETTLING_AST17
};
#endif

#if SAM3S || SAM4S ||  SAM3N || SAM3XA || SAM4C || SAM4CP || SAM4CM
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
#endif

#if SAM3S || SAM4S ||  SAM3N || SAM3XA || SAM4C || SAM4CP || SAM4CM
uint32_t adc_init(Adc *p_adc, const uint32_t ul_mck,
		const uint32_t ul_adc_clock, const enum adc_startup_time startup);
void adc_configure_trigger(Adc *p_adc, const enum adc_trigger_t trigger,
		const uint8_t uc_freerun);
void adc_configure_sequence(Adc *p_adc, const enum adc_channel_num_t ch_list[],
		const uint8_t uc_num);
void adc_enable_tag(Adc *p_adc);
void adc_disable_tag(Adc *p_adc);
enum adc_channel_num_t adc_get_tag(const Adc *p_adc);
void adc_start_sequencer(Adc *p_adc);
void adc_stop_sequencer(Adc *p_adc);
void adc_set_comparison_mode(Adc *p_adc, const uint8_t uc_mode);
#if SAM4C || SAM4CP || SAM4CM
void adc_set_comparison_filter(Adc *p_adc, uint8_t filter);
#endif
uint32_t adc_get_comparison_mode(const Adc *p_adc);
void adc_set_comparison_window(Adc *p_adc, const uint16_t us_low_threshold,
		const uint16_t us_high_threshold);
void adc_set_comparison_channel(Adc *p_adc,
		const enum adc_channel_num_t channel);
void adc_set_writeprotect(Adc *p_adc, const uint32_t ul_enable);
uint32_t adc_get_writeprotect_status(const Adc *p_adc);
void adc_check(Adc* p_adc, const uint32_t ul_mck);
uint32_t adc_get_overrun_status(const Adc *p_adc);
#elif SAM3U
uint32_t adc_init(Adc * p_adc, const uint32_t ul_mck, const uint32_t ul_adc_clock,
		const uint32_t ul_startuptime);
void adc_configure_trigger(Adc *p_adc, const enum adc_trigger_t trigger);
void adc_configure_power_save(Adc *p_adc, const uint8_t uc_sleep);
#endif

#if SAM3S8 || SAM4S || SAM3N || SAM3SD8
void adc_configure_power_save(Adc *p_adc, const uint8_t uc_sleep,
		const uint8_t uc_fwup);
#elif SAM3U || SAM4C || SAM4CP || SAM4CM
void adc_configure_power_save(Adc *p_adc, const uint8_t uc_sleep);
#endif
void adc_set_resolution(Adc *p_adc, const enum adc_resolution_t resolution);
void adc_start(Adc *p_adc);
void adc_reset(Adc *p_adc);
void adc_enable_channel(Adc *p_adc, const enum adc_channel_num_t adc_ch);
void adc_disable_channel(Adc *p_adc, const enum adc_channel_num_t adc_ch);
void adc_enable_all_channel(Adc *p_adc);
void adc_disable_all_channel(Adc *p_adc);
uint32_t adc_get_channel_status(const Adc *p_adc, const enum adc_channel_num_t adc_ch);
uint32_t adc_get_channel_value(const Adc *p_adc,const enum adc_channel_num_t adc_ch);
uint32_t adc_get_latest_value(const Adc *p_adc);
uint32_t adc_get_actual_adc_clock(const Adc *p_adc, const uint32_t ul_mck);
void adc_enable_interrupt(Adc *p_adc, const uint32_t ul_source);
void adc_disable_interrupt(Adc *p_adc, const uint32_t ul_source);
uint32_t adc_get_status(const Adc *p_adc);
uint32_t adc_get_interrupt_mask(const Adc *p_adc);
Pdc *adc_get_pdc_base(const Adc *p_adc);

#if SAM3S || SAM4S || SAM3XA
void adc_configure_timing(Adc *p_adc, const uint8_t uc_tracking,
		const enum adc_settling_time_t settling, const uint8_t uc_transfer);
void adc_enable_anch( Adc *p_adc );
void adc_disable_anch( Adc *p_adc );
void adc_enable_channel_differential_input(Adc *p_adc, const enum adc_channel_num_t channel);
void adc_disable_channel_differential_input(Adc *p_adc, const enum adc_channel_num_t channel);
void adc_enable_channel_input_offset(Adc *p_adc, const enum adc_channel_num_t channel);
void adc_disable_channel_input_offset(Adc *p_adc, const enum adc_channel_num_t channel);
void adc_set_channel_input_gain(Adc *p_adc, const enum adc_channel_num_t channel,
		const enum adc_gainvalue_t uc_gain);
void adc_set_bias_current(Adc *p_adc, const uint8_t uc_ibctl);
#elif SAM3N || SAM4C || SAM4CP || SAM4CM
void adc_configure_timing(Adc *p_adc, const uint8_t uc_tracking);
#elif SAM3U
void adc_configure_timing(Adc *p_adc, const uint32_t ul_sh);
#endif

#if SAM3S || SAM4S || SAM3XA || SAM4C || SAM4CP || SAM4CM
void adc_enable_ts(Adc *p_adc);
void adc_disable_ts(Adc *p_adc);
#if SAM4C || SAM4CP || SAM4CM
/** Definitions for Temperature Comparison Mode */
enum adc_temp_cmp_mode {
	ADC_TEMP_CMP_MODE_0 = ADC_TEMPMR_TEMPCMPMOD_LOW,
	ADC_TEMP_CMP_MODE_1 = ADC_TEMPMR_TEMPCMPMOD_HIGH,
	ADC_TEMP_CMP_MODE_2 = ADC_TEMPMR_TEMPCMPMOD_IN,
	ADC_TEMP_CMP_MODE_3 = ADC_TEMPMR_TEMPCMPMOD_OUT
};
void adc_configure_ts_comparison(Adc *p_adc, enum adc_temp_cmp_mode mode,
		uint16_t low_threshold, uint16_t high_threshold);
#endif
#endif

#if  SAM3S8 || SAM3SD8 || SAM4S
void adc_set_calibmode(Adc *p_adc);
#endif

#if SAM4C || SAM4CP | SAM4CM
void adc_set_averaging_trigger(Adc *p_adc, bool multi);
enum adc_internal_ref_voltage {
	ADC_INTERNAL_REF_2426MV = 0,
	ADC_INTERNAL_REF_2305MV,
	ADC_INTERNAL_REF_2184MV,
	ADC_INTERNAL_REF_2063MV,
	ADC_INTERNAL_REF_1941MV,
	ADC_INTERNAL_REF_1820MV,
	ADC_INTERNAL_REF_1699MV,
	ADC_INTERNAL_REF_1578MV,
	ADC_INTERNAL_REF_3396MV,
	ADC_INTERNAL_REF_3275MV,
	ADC_INTERNAL_REF_3154MV,
	ADC_INTERNAL_REF_3032MV,
	ADC_INTERNAL_REF_2911MV,
	ADC_INTERNAL_REF_2790MV,
	ADC_INTERNAL_REF_2699MV,
	ADC_INTERNAL_REF_2547MV,
};
struct adc_internal_ref {
	bool adc_internal_ref_change_enable;
	enum adc_internal_ref_voltage volt;
	bool adc_force_internal_ref;
	bool adc_internal_ref_on;
};
enum status_code adc_set_internal_reference_voltage(Adc *p_adc,
		struct adc_internal_ref *ref);
#endif

#if SAM3U
uint32_t adc12b_init(Adc12b *p_adc, const uint32_t ul_mck, const uint32_t ul_adc_clock,
		const uint32_t ul_startuptime, const uint32_t ul_offmode_startuptime);
void adc12b_set_resolution(Adc12b *p_adc, const enum adc_resolution_t resolution);
void adc12b_configure_trigger(Adc12b *p_adc, const enum adc12b_trigger_t trigger);
void adc12b_configure_power_save(Adc12b *p_adc, const uint8_t uc_sleep,
		const uint8_t uc_offmode);
void adc12b_configure_timing(Adc12b *p_adc, const uint32_t ul_sh);
void adc12b_start(Adc12b *p_adc);
void adc12b_reset(Adc12b *p_adc);
void adc12b_enable_channel(Adc12b *p_adc, const enum adc_channel_num_t adc_ch);
void adc12b_disable_channel(Adc12b *p_adc, const enum adc_channel_num_t adc_ch);
void adc12b_enable_all_channel(Adc12b *p_adc);
void adc12b_disable_all_channel(Adc12b *p_adc);
uint32_t adc12b_get_channel_status(const Adc12b *p_adc,const enum adc_channel_num_t adc_ch);
uint32_t adc12b_get_channel_value(const Adc12b *p_adc, const enum adc_channel_num_t adc_ch);
uint32_t adc12b_get_latest_value(const Adc12b *p_adc);
void adc12b_enable_differential_input(Adc12b *p_adc);
void adc12b_disable_differential_input(Adc12b *p_adc);
void adc12b_enable_input_offset(Adc12b *p_adc);
void adc12b_disable_input_offset(Adc12b *p_adc);
void adc12b_set_input_gain(Adc12b *p_adc, const enum adc_gainvalue_t uc_gain);
uint32_t adc12b_get_actual_adc_clock(const Adc12b *p_adc, const uint32_t ul_mck);
void adc12b_enable_interrupt(Adc12b *p_adc, const uint32_t ul_source);
void adc12b_disable_interrupt(Adc12b *p_adc, const uint32_t ul_source);
uint32_t adc12b_get_interrupt_mask(const Adc12b *p_adc);
uint32_t adc12b_get_status(const Adc12b *p_adc);
void adc12b_set_bias_current(Adc12b *p_adc, const uint8_t uc_ibctl);
Pdc *adc12b_get_pdc_base(const Adc12b *p_adc);
#endif

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \page sam_adc_quickstart Quickstart guide for SAM ADC driver
 *
 * This is the quickstart guide for the \ref adc_group "SAM ADC driver",
 * with step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section adc_basic_use_case Basic use case
 * In this basic use case, the ADC module and single channel are configured for:
 * - 12-bit, unsigned conversions
 * - Internal bandgap as 3.3 V reference
 * - ADC clock rate of at most 6.4 MHz and maximum sample rate is 1 MHz
 * - Software triggering of conversions
 * - Interrupt-based conversion handling
 * - Single channel measurement
 * - ADC_CHANNEL_5 as positive input
 *
 * \subsection sam_adc_quickstart_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (Sysclock)"
 * -# \ref pmc_group "Power Management Controller (PMC)"
 *
 * \section adc_basic_use_case_setup Setup steps
 * \subsection adc_basic_use_case_setup_code Example code
 * Add to application C-file:
 * \code
	   void ADC_IrqHandler(void)
	   {
	       // Check the ADC conversion status
	       if ((adc_get_status(ADC) & ADC_ISR_DRDY) == ADC_ISR_DRDY)
	       {
	       // Get latest digital data value from ADC and can be used by application
	           uint32_t result = adc_get_latest_value(ADC);
	       }
	   }
	   void adc_setup(void)
	   {
	       adc_init(ADC, sysclk_get_main_hz(), ADC_CLOCK, 8);

	       adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);

	       adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_12);

	       adc_enable_channel(ADC, ADC_CHANNEL_5);

	       adc_enable_interrupt(ADC, ADC_IER_DRDY);

	       adc_configure_trigger(ADC, ADC_TRIG_SW, 0);
	   }
\endcode
 *
 * \subsection adc_basic_use_case_setup_flow Workflow
 * -# Define the interrupt service handler in the application:
 *   - \code
	void ADC_IrqHandler(void)
	{
	    //Check the ADC conversion status
	    if ((adc_get_status(ADC) & ADC_ISR_DRDY) == ADC_ISR_DRDY)
	    {
	    //Get latest digital data value from ADC and can be used by application
	        uint32_t result = adc_get_latest_value(ADC);
	    }
	}
\endcode
 *   - \note Get ADC status and check if the conversion is finished. If done, read the last ADC result data.
 * -# Initialize the given ADC with the specified ADC clock and startup time:
 *   - \code adc_init(ADC, sysclk_get_main_hz(), ADC_CLOCK, 8); \endcode
 *   - \note The ADC clock range is between master clock / 2 and master clock / 512.
 * The function sysclk_get_main_hz() is used to get the master clock frequency while ADC_CLOCK gives the ADC clock frequency.
 * -# Configure ADC timing:
 *   - \code adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1); \endcode
 *   - \note Tracking Time = (0 + 1) * ADC Clock period
 * Settling Time =  ADC_SETTLING_TIME_3 * ADC Clock period
 * Transfer Time = (1 * 2 + 3) * ADC Clock period
 * -# Set the ADC resolution.
 *   - \code adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_12); \endcode
 *   - \note The resolution value can be set to 10 bits or 12 bits.
 * -# Enable the specified ADC channel:
 *   - \code adc_enable_channel(ADC, ADC_CHANNEL_5); \endcode
 * -# Enable ADC interrupts:
 *   - \code adc_enable_interrupt(ADC, ADC_IER_DRDY); \endcode
 * -# Configure software conversion trigger:
 *   - \code adc_configure_trigger(ADC, ADC_TRIG_SW, 0); \endcode
 *
 * \section adc_basic_use_case_usage Usage steps
 * \subsection adc_basic_use_case_usage_code Example code
 * Add to, e.g., main loop in application C-file:
 * \code
	adc_start(ADC);
\endcode
 *
 * \subsection adc_basic_use_case_usage_flow Workflow
 * -# Start ADC conversion on channel:
 *   - \code adc_start(ADC); \endcode
 *
 * \section adc_use_cases Advanced use cases
 * For more advanced use of the ADC driver, see the following use cases:
 * - \subpage adc_use_case_1 : 12-bits unsigned, comparison event happen and interrupt
 * driven
 */
/**
 * \page adc_use_case_1 Advanced use cases
 * In this use case the ADC module and one channel are configured for:
 * - 12-bit, unsigned conversions
 * - Internal bandgap as 3.3 V reference
 * - ADC clock rate of at most 6.4 MHz and maximum sample rate is 1 MHz
 * - Software triggering of conversions
 * - Comparison event happen and interrupt handling
 * - Single channel measurement
 * - ADC_CHANNEL_5 as positive input
 *
 * \section adc_use_case_1_setup Setup steps
 * \subsection adc_use_case_1_setup_code Example code
 * Add to application C-file:
 * \code
	void ADC_IrqHandler(void)
	{
	    // Check the ADC conversion status
	    if ((adc_get_status(ADC) & ADC_ISR_COMPE) == ADC_ISR_COMPE)
	    {
	        // Get comparison mode of ADC
	        uint32_t ul_mode = adc_get_comparison_mode(ADC);
	        // Get latest digital data value from ADC and can be used by application
	        uint16_t us_adc = adc_get_channel_value(ADC, ADC_CHANNEL_5);
	    }
	}
\endcode
 *
 * \code
	   void adc_setup(void)
	   {
	       adc_init(ADC, sysclk_get_main_hz(), ADC_CLOCK, 8);

	       adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);

	       adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_12);

	       adc_enable_channel(ADC, ADC_CHANNEL_5);

	       adc_set_comparison_channel(ADC, ADC_CHANNEL_5);
	       adc_set_comparison_mode(ADC, ADC_EMR_CMPMODE_IN);
	       adc_set_comparison_window(ADC, MAX_DIGITAL, 0);

	       adc_enable_interrupt(ADC, ADC_IER_COMPE);

	       adc_configure_trigger(ADC, ADC_TRIG_TIO_CH_0, 0);
	   }
\endcode
 *
 * \subsection adc_basic_use_case_setup_flow Workflow
 * -# Define the interrupt service handler in the application:
 *   - \code
	void ADC_IrqHandler(void)
	{
	    // Check the ADC conversion status
	    if ((adc_get_status(ADC) & ADC_ISR_COMPE) == ADC_ISR_COMPE)
	    {
	        // Get comparison mode of ADC
	        uint32_t ul_mode = adc_get_comparison_mode(ADC);
	        // Get latest digital data value from ADC and can be used by application
	        uint16_t us_adc = adc_get_channel_value(ADC, ADC_CHANNEL_5);
	    }
	}
\endcode
 *   - \note Get ADC status and check if comparison event occurred. If occurred, read the ADC channel value and comparison mode.
 * -# Initialize the given ADC with the specified ADC clock and startup time:
 *   - \code adc_init(ADC, sysclk_get_main_hz(), ADC_CLOCK, 10); \endcode
 *   - \note The ADC clock range is between master clock/2 and master clock/512.
 * The function sysclk_get_main_hz() is used to get the master clock frequency while ADC_CLOCK gives the ADC clock frequency.
 * -# Configure ADC timing:
 *   - \code adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1); \endcode
 *   - \note Tracking Time = (0 + 1) * ADC Clock period
 * Settling Time =  ADC_SETTLING_TIME_3 * ADC Clock period
 * Transfer Time = (1 * 2 + 3) * ADC Clock period
 * -# Set the ADC resolution.
 *   - \code adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_12); \endcode
 *   - \note The resolution value can be set to 10 bits or 12 bits.
 * -# Enable the specified ADC channel:
 *   - \code adc_enable_channel(ADC, ADC_CHANNEL_5); \endcode
 * -# Set the comparison ADC channel, mode and window:
 *   - \code adc_set_comparison_channel(ADC, ADC_CHANNEL_5);
	adc_set_comparison_mode(ADC, ADC_EMR_CMPMODE_IN);
	adc_set_comparison_window(ADC, us_high_threshold, us_low_threshold); \endcode
 *   - \note The high and low threshold of comparison window can be set by the user.
 * An event will be generated whenever the converted data is in the comparison window.
 * -# Enable ADC interrupts:
 *   - \code adc_enable_interrupt(ADC, ADC_IER_COMPE); \endcode
 * -# Configure software conversion trigger:
 *   - \code adc_configure_trigger(ADC, ADC_TRIG_SW, 0); \endcode
 *
 * \section adc_use_case_1_usage Usage steps
 * \subsection adc_use_case_1_usage_code Example code
 * Add to, e.g., main loop in application C-file:
 * \code
	adc_start(ADC);
\endcode
 *
 * \subsection adc_use_case_1_usage_flow Workflow
 * -# Start ADC conversion on the configured channels:
 *   - \code adc_start(ADC); \endcode
 */
#endif /* ADC_H_INCLUDED */
