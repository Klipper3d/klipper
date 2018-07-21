/**
 * \file
 *
 * \brief Analog-Front-End Controller driver for SAM.
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
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

#include "afec.h"
#include "sleepmgr.h"
#include "status_codes.h"
#include "sysclk.h"
#include "pmc.h"

/**
 * \defgroup sam_drivers_afec_group Analog-Front-End Controller
 *
 * See \ref sam_afec_quickstart.
 *
 * Driver for the Analog-Front-End Controller. This driver provides access to
 * the main features of the AFEC controller.
 *
 * @{
 */
#if defined(AFEC1)
#define NUM_OF_AFEC    (2UL)
#else
#define NUM_OF_AFEC    (1UL)
#endif

/* The gap between bit EOC15 and DRDY in interrupt register */
#if defined __SAM4E8C__  || defined __SAM4E16C__ || defined __SAM4E8CB__ || defined __SAM4E16CB__
#define AFEC_INTERRUPT_GAP1                  (17UL)
#elif defined __SAM4E8E__  || defined __SAM4E16E__
#define AFEC_INTERRUPT_GAP1                  (8UL)
#elif (SAMV71 || SAMV70 || SAME70 || SAMS70)
/* The gap between bit EOC11 and DRDY in interrupt register */
#define AFEC_INTERRUPT_GAP1                  (12UL)
#endif

#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
/* The gap between bit COMPE and TEMPCHG in interrupt register */
#define AFEC_INTERRUPT_GAP2                  (3UL)
#else
/* The gap between bit RXBUFF and TEMPCHG in interrupt register */
#define AFEC_INTERRUPT_GAP2                  (1UL)
#endif

/* The number of channel in channel sequence1 register */
#define AFEC_SEQ1_CHANNEL_NUM                (8UL)

#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
/* The interrupt source number of temperature sensor */
#define AFEC_TEMP_INT_SOURCE_NUM             (11UL)
#else
/* The interrupt source number of temperature sensor */
#define AFEC_TEMP_INT_SOURCE_NUM             (15UL)
#endif

afec_callback_t afec_callback_pointer[NUM_OF_AFEC][_AFEC_NUM_OF_INTERRUPT_SOURCE];


/**
 * \internal
 * \brief Get AFEC instance number.
 *
 * \param afec  Base address of the AFEC
 *
 * \return   AFEC instance number
 */
static uint32_t afec_find_inst_num(Afec *const afec)
{
#if defined(AFEC1)
	if (afec == AFEC1) {
		return 1;
	}
#endif
#if defined(AFEC0)
	if (afec == AFEC0) {
		return 0;
	}
#endif
	return 0;
}

/**
 * \internal
 * \brief Get AFEC Peripheral ID.
 *
 * \param afec  Base address of the AFEC
 *
 * \return   AFEC Peripheral ID
 */
static uint32_t afec_find_pid(Afec *const afec)
{
#if defined(ID_AFEC1)
	if (afec == AFEC1) {
		return ID_AFEC1;
	}
#endif
#if defined(ID_AFEC0)
	if (afec == AFEC0) {
		return ID_AFEC0;
	}
#endif
	return ID_AFEC0;
}

/**
 * \internal
 * \brief Configure the AFEC Module.
 *
 * \param afec  Base address of the AFEC
 * \param config   Configuration for the AFEC
 */
static void afec_set_config(Afec *const afec, struct afec_config *config)
{
	uint32_t reg = 0;

	reg = (config->useq ? AFEC_MR_USEQ_REG_ORDER : 0) |
		#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
			AFEC_MR_PRESCAL((config->mck / config->afec_clock )- 1) |
			AFEC_MR_ONE |
		#else
			(config->anach ? AFEC_MR_ANACH_ALLOWED : 0) |
			AFEC_MR_PRESCAL(config->mck / (2 * config->afec_clock) - 1) |
			(config->settling_time) |		
		#endif
			AFEC_MR_TRACKTIM(config->tracktim) |
			AFEC_MR_TRANSFER(config->transfer) |
			(config->startup_time);

	afec->AFEC_MR = reg;

	afec->AFEC_EMR = (config->tag ? AFEC_EMR_TAG : 0) |
			(config->resolution) |
			(config->stm ? AFEC_EMR_STM : 0);
  #if (SAMV71 || SAMV70 || SAME70 || SAMS70)
	afec->AFEC_ACR = AFEC_ACR_IBCTL(config->ibctl) | AFEC_ACR_PGA0EN | AFEC_ACR_PGA1EN;
  #else
    afec->AFEC_ACR = AFEC_ACR_IBCTL(config->ibctl);
  #endif
}

/**
 * \brief Configure the AFEC channel.
 *
 * \param afec  Base address of the AFEC
 * \param channel The channel number
 * \param config   Configuration for the AFEC channel
 */
void afec_ch_set_config(Afec *const afec, const enum afec_channel_num channel,
		struct afec_ch_config *config)
{
	afec_ch_sanity_check(afec, channel);
	uint32_t reg = 0;

	reg = afec->AFEC_DIFFR;
	reg &= ~(0x1u << channel);
	reg |= (config->diff) ? (0x1u << channel) : 0;
	afec->AFEC_DIFFR = reg;

	reg = afec->AFEC_CGR;
	reg &= ~(0x03u << (2 * channel));
	reg |= (config->gain) << (2 * channel);
	afec->AFEC_CGR = reg;
}

/**
 * \brief Configure the AFEC temperature sensor.
 *
 * \param afec  Base address of the AFEC
 * \param config   Configuration for the AFEC temperature sensor
 */
void afec_temp_sensor_set_config(Afec *const afec,
		struct afec_temp_sensor_config *config)
{
	Assert(afec == AFEC0);

	uint32_t reg = 0;

	reg = ((config->rctc) ? AFEC_TEMPMR_RTCT : 0) | (config->mode);
	afec->AFEC_TEMPMR = reg;

	afec->AFEC_TEMPCWR = AFEC_TEMPCWR_TLOWTHRES(config->low_threshold) |
			AFEC_TEMPCWR_THIGHTHRES(config->high_threshold);
			
}

/**
 * \brief Get the AFEC default configurations.
 *
 * Use to initialize the configuration structure to known default values. This
 * function should be called at the start of any AFEC initiation.
 *
 * The default configuration is as follows:
 * - 12 -bit resolution
 * - AFEC clock frequency is 6MHz
 * - Start Up Time is 64 periods AFEC clock, for SAMV71 is 24 periods AFEC clock
 * - Analog Settling Time is 3 periods of AFEC clock
 * - Tracking Time is 3 periods of AFEC clock
 * - Transfer Period is 5 periods AFEC clock,for SAMV71 is 7 periods AFEC clock
 * - Allows different analog settings for each channel
 * - The controller converts channels in a simple numeric order
 * - Appends the channel number to the conversion result in AFE_LDCR register
 * - Only a Single Trigger is required to get an averaged value
 * - AFE Bias Current Control value is 1
 *
 * \param cfg Pointer to configuration structure to be initiated.
 */
void afec_get_config_defaults(struct afec_config *const cfg)
{
	/* Sanity check argument. */
	Assert(cfg);

	cfg->resolution = AFEC_12_BITS;
	cfg->mck = sysclk_get_cpu_hz();
		cfg->afec_clock = 6000000UL;
		cfg->startup_time = AFEC_STARTUP_TIME_4;
	#if !(SAMV71 || SAMV70 || SAME70 || SAMS70)
		cfg->settling_time = AFEC_SETTLING_TIME_0;
	#endif	
		cfg->tracktim = 2;
		cfg->transfer = 1;
		cfg->anach = true;
		cfg->useq = false;
		cfg->tag = true;
		cfg->stm = true;
		cfg->ibctl = 1;
}

/**
 * \brief Get the AFEC channel default configurations.
 *
 * Use to initialize the configuration structure to known default values.
 *
 * The default configuration is as follows:
 * - Single Ended Mode
 * - Gain value is 1
 *
 * \param cfg Pointer to channel configuration structure to be initiated.
 */
void afec_ch_get_config_defaults(struct afec_ch_config *const cfg)
{
	/*Sanity check argument. */
	Assert(cfg);

	cfg->diff = false;
   	cfg->gain = AFEC_GAINVALUE_1;

}

/**
 * \brief Get the AFEC Temperature Sensor default configurations.
 *
 * Use to initialize the configuration structure to known default values.
 *
 * The default configuration is as follows:
 * - The temperature sensor measure is not triggered by RTC event
 * - Generates an event when the converted data is in the comparison window
 * - The window range is 0xFF ~ 0xFFF
 *
 * \param cfg Pointer to temperature sensor configuration structure
 *        to be initiated.
 */
void afec_temp_sensor_get_config_defaults(
		struct afec_temp_sensor_config *const cfg)
{
	/*Sanity check argument. */
	Assert(cfg);

	cfg->rctc = false;
	cfg->mode= AFEC_TEMP_CMP_MODE_2;
	cfg->low_threshold= 0xFF;
	cfg->high_threshold= 0xFFF;
}

/**
 * \brief Initialize the AFEC Module.
 *
 * \param afec  Base address of the AFEC
 * \param config   Configuration for the AFEC
 *
 * \retval STATUS_OK  Initialization is finished.
 * \retval STATUS_ERR_BUSY  Initialization failed.
 */
enum status_code afec_init(Afec *const afec, struct afec_config *config)
{
	Assert(afec);
	Assert(config);

	if ((afec_get_interrupt_status(afec) & AFEC_ISR_DRDY) == AFEC_ISR_DRDY) {
		return STATUS_ERR_BUSY;
	}

	/* Reset and configure the AFEC module */
	afec->AFEC_CR = AFEC_CR_SWRST;
	afec_set_config(afec, config);

	uint32_t i;
	if(afec == AFEC0) {
		for (i = 0; i < _AFEC_NUM_OF_INTERRUPT_SOURCE; i++){
			afec_callback_pointer[0][i] = 0;
		}
	}
	if(afec == AFEC1) {
		for (i = 0; i < _AFEC_NUM_OF_INTERRUPT_SOURCE; i++){
			afec_callback_pointer[1][i] = 0;
		}
	}

	return STATUS_OK;
}

/**
 * \brief Configure comparison mode.
 *
 * \param afec  Base address of the AFEC.
 * \param mode Comparison mode.
 * \param channel Comparison Selected Channel.
 * \param cmp_filter Compare Event Filtering.
 */
void afec_set_comparison_mode(Afec *const afec,
		const enum afec_cmp_mode mode,
		const enum afec_channel_num channel, uint8_t cmp_filter)
{
	if (channel != AFEC_CHANNEL_ALL) {
		afec_ch_sanity_check(afec, channel);
	}

	uint32_t reg;

	reg = afec->AFEC_EMR;

	reg &= ~(AFEC_EMR_CMPSEL_Msk |
			AFEC_EMR_CMPMODE_Msk |
			AFEC_EMR_CMPFILTER_Msk);
	reg |= mode |
			((channel == AFEC_CHANNEL_ALL) ? AFEC_EMR_CMPALL
			: AFEC_EMR_CMPSEL(channel)) |
			AFEC_EMR_CMPFILTER(cmp_filter);

	afec->AFEC_EMR = reg;
}

/**
 * \brief Configure AFEC power mode.
 *
 * \param afec  Base address of the AFEC.
 * \param mode   AFEC power mode value.
 */
void afec_set_power_mode(Afec *const afec,
		const enum afec_power_mode mode)
{
	uint32_t reg;

	reg = afec->AFEC_MR;

	switch(mode) {
		case AFEC_POWER_MODE_0:
			reg &= ~(AFEC_MR_FWUP_ON | AFEC_MR_SLEEP_SLEEP);
			break;
		case AFEC_POWER_MODE_1:
			reg |= AFEC_MR_FWUP_ON;
			break;
		case AFEC_POWER_MODE_2:
			reg |= AFEC_MR_SLEEP_SLEEP;
			reg &= ~AFEC_MR_FWUP_ON;
			break;
	}

	afec->AFEC_MR = reg;
}

/**
 * \brief Set callback for AFEC
 *
 * \param afec  Base address of the AFEC
 * \param source    Interrupt source
 * \param callback  Callback function pointer
 * \param irq_level Interrupt level
 */
void afec_set_callback(Afec *const afec, enum afec_interrupt_source source,
		afec_callback_t callback, uint8_t irq_level)
{
	Assert(afec);
	Assert(callback);

	uint32_t i = afec_find_inst_num(afec);
	afec_callback_pointer[i][source] = callback;
	if (!i) {
		irq_register_handler(AFEC0_IRQn, irq_level);
	} else if (i == 1) {
		irq_register_handler(AFEC1_IRQn, irq_level);
	}
	/* Enable the specified interrupt source */
	afec_enable_interrupt(afec, source);
}

/**
 * \brief Enable AFEC interrupts.
 *
 * \param afec  Base address of the AFEC.
 * \param interrupt_source Interrupts to be enabled.
 */
void afec_enable_interrupt(Afec *const afec,
		enum afec_interrupt_source interrupt_source)
{
	if (interrupt_source == AFEC_INTERRUPT_ALL) {
		afec->AFEC_IER = AFEC_INTERRUPT_ALL;
		return;
	}

	if (interrupt_source < AFEC_INTERRUPT_DATA_READY) {
	  #if (SAMV71 || SAMV70 || SAME70 || SAMS70)
		if (interrupt_source == AFEC_INTERRUPT_EOC_11) {
			afec->AFEC_IER = 1 << AFEC_TEMP_INT_SOURCE_NUM;
	  #else
		if (interrupt_source == AFEC_INTERRUPT_EOC_15) {
			afec->AFEC_IER = 1 << AFEC_TEMP_INT_SOURCE_NUM;	  
	  #endif 
		} else {
			afec->AFEC_IER = 1 << interrupt_source;
		}
	} else if (interrupt_source < AFEC_INTERRUPT_TEMP_CHANGE) {
		afec->AFEC_IER = 1 << (interrupt_source + AFEC_INTERRUPT_GAP1);
	} else {
		afec->AFEC_IER = 1 << (interrupt_source + AFEC_INTERRUPT_GAP1
				+ AFEC_INTERRUPT_GAP2);
	}
}

/**
 * \brief Disable AFEC interrupts.
 *
 * \param afec  Base address of the AFEC.
 * \param interrupt_source Interrupts to be disabled.
 */
void afec_disable_interrupt(Afec *const afec,
		enum afec_interrupt_source interrupt_source)
{
	if (interrupt_source == AFEC_INTERRUPT_ALL) {
		afec->AFEC_IDR = AFEC_INTERRUPT_ALL;
		return;
	}

	if (interrupt_source < AFEC_INTERRUPT_DATA_READY) {
	  #if (SAMV71 || SAMV70 || SAME70 || SAMS70)
		if (interrupt_source == AFEC_INTERRUPT_EOC_11) {
			afec->AFEC_IDR = 1 << AFEC_TEMP_INT_SOURCE_NUM;
	  #else
		if (interrupt_source == AFEC_INTERRUPT_EOC_15) {
			afec->AFEC_IDR = 1 << AFEC_TEMP_INT_SOURCE_NUM;	  
	  #endif
		} else {
			afec->AFEC_IDR = 1 << interrupt_source;
		}
	} else if (interrupt_source < AFEC_INTERRUPT_TEMP_CHANGE) {
		afec->AFEC_IDR = 1 << (interrupt_source + AFEC_INTERRUPT_GAP1);
	} else {
		afec->AFEC_IDR = 1 << (interrupt_source + AFEC_INTERRUPT_GAP1
				+ AFEC_INTERRUPT_GAP2);
	}
}

/**
 * \internal
 * \brief Common AFEC interrupt handler
 *
 * The optional callback used by the interrupt handler is set by the
 * afec_set_callback() function.
 *
 * \param inst_num AFEC instance number to handle interrupt for
 * \param source   Interrupt source number
 */
static void afec_interrupt(uint8_t inst_num,
		enum afec_interrupt_source source)
{
	if (afec_callback_pointer[inst_num][source]) {
		afec_callback_pointer[inst_num][source]();
	}
}

/**
 * \internal
 * \brief Call the callback function if the corresponding interrupt is asserted
 *
 * \param afec  Base address of the AFEC.
 */
static void afec_process_callback(Afec *const afec)
{
	volatile uint32_t status;
	uint32_t cnt, inst_num;

	status = afec_get_interrupt_status(afec) & afec_get_interrupt_mask(afec);
	inst_num = afec_find_inst_num(afec);

	for (cnt = 0; cnt < _AFEC_NUM_OF_INTERRUPT_SOURCE; cnt++) {
		if (cnt < AFEC_INTERRUPT_DATA_READY) {
		#if defined __SAM4E8C__  || defined __SAM4E16C__ || defined __SAM4E8CB__  || defined __SAM4E16CB__
			if(cnt == AFEC_INTERRUPT_EOC_15) {
				if (status & (1 << AFEC_TEMP_INT_SOURCE_NUM)) {
					afec_interrupt(inst_num, (enum afec_interrupt_source)cnt);
				}
			} else {
				if (status & (1 << cnt)) {
					afec_interrupt(inst_num, (enum afec_interrupt_source)cnt);
				}
			}
		#elif defined __SAM4E8E__  || defined __SAM4E16E__ || SAMV71 || SAMV70 || SAMS70 || SAME70
			if (status & (1 << cnt)) {
				afec_interrupt(inst_num, (enum afec_interrupt_source)cnt);
			}
		#endif
		} else if (cnt < AFEC_INTERRUPT_TEMP_CHANGE) {
			if (status & (1 << (cnt + AFEC_INTERRUPT_GAP1))) {
				afec_interrupt(inst_num, (enum afec_interrupt_source)cnt);
			}
		} else {
			if (status & (1 << (cnt + AFEC_INTERRUPT_GAP1 + AFEC_INTERRUPT_GAP2))) {
				afec_interrupt(inst_num, (enum afec_interrupt_source)cnt);
			}
		}
	}
}

/**
 * \brief Interrupt handler for AFEC0.
 */
void AFEC0_Handler(void)
{
	afec_process_callback(AFEC0);
}

/**
 * \brief Interrupt handler for AFEC1.
 */
void AFEC1_Handler(void)
{
	afec_process_callback(AFEC1);
}

/**
 * \brief Enable AFEC Module.
 *
 * \param afec  Base address of the AFEC
 */
void afec_enable(Afec *const afec)
{
	Assert(afec);
	uint32_t pid;

	pid = afec_find_pid(afec);
	/* Enable peripheral clock. */
	pmc_enable_periph_clk(pid);
	sleepmgr_lock_mode(SLEEPMGR_SLEEP_WFI);
}

/**
 * \brief Disable AFEC Module.
 *
 * \param afec  Base address of the AFEC
 */
void afec_disable(Afec *const afec)
{
	Assert(afec);
	uint32_t pid;

	pid = afec_find_pid(afec);
	/* Disable peripheral clock. */
	pmc_disable_periph_clk(pid);
	sleepmgr_unlock_mode(SLEEPMGR_SLEEP_WFI);
}

/**
 * \brief Configure conversion sequence.
 *
 * \param afec  Base address of the AFEC.
 * \param ch_list Channel sequence list.
 * \param uc_num Number of channels in the list.
 */
void afec_configure_sequence(Afec *const afec,
		const enum afec_channel_num ch_list[], uint8_t uc_num)
{
	uint8_t uc_counter;

	/* Set user sequence mode */
	afec->AFEC_MR |= AFEC_MR_USEQ_REG_ORDER;
	afec->AFEC_SEQ1R = 0;
	afec->AFEC_SEQ2R = 0;

	if (uc_num < AFEC_SEQ1_CHANNEL_NUM) {
		for (uc_counter = 0; uc_counter < uc_num; uc_counter++) {
			afec->AFEC_SEQ1R |=
					ch_list[uc_counter] << (4 * uc_counter);
		}
	} else {
		for (uc_counter = 0; uc_counter < AFEC_SEQ1_CHANNEL_NUM; uc_counter++) {
			afec->AFEC_SEQ1R |=
					ch_list[uc_counter] << (4 * uc_counter);
		}
		for (uc_counter = 0; uc_counter < uc_num - AFEC_SEQ1_CHANNEL_NUM;
				uc_counter++) {
			afec->AFEC_SEQ2R |=
					ch_list[uc_counter + AFEC_SEQ1_CHANNEL_NUM] << (4 * uc_counter);
		}
	}
}

#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Configure Automatic Error Correction.
 *
 * \param afec  Base address of the AFEC
 * \param channel  Number of channels in the list.
 * \param offsetcorr  the Offset Correction value,signed value
 * \param gaincorr the Gain Correction value.
 */
void afec_configure_auto_error_correction(Afec *const afec,
		const enum afec_channel_num channel, int16_t offsetcorr, uint16_t gaincorr)
{


	if (channel != AFEC_CHANNEL_ALL) {
		afec_ch_sanity_check(afec, channel);
	}

	uint32_t reg = 0;
	reg = afec->AFEC_CECR;
	reg = (channel == AFEC_CHANNEL_ALL)? 0 : ~(0x1u << channel);
	reg |= (channel == AFEC_CHANNEL_ALL)? AFEC_CHANNEL_ALL : (0x1u << channel);
	afec->AFEC_CECR = reg;

	afec->AFEC_COSR = AFEC_COSR_CSEL;
    afec->AFEC_CVR = AFEC_CVR_OFFSETCORR(offsetcorr) | AFEC_CVR_GAINCORR(gaincorr);		 
	
}

/**
 * \brief correct the Converted Data of the selected channel if automatic error correction is enabled.
 *
 * \param afec  Base address of the AFEC.
 * \param afec_ch AFEC channel number.
 *
 * \return AFEC corrected value of the selected channel.
 */
 uint32_t afec_get_correction_value(Afec *const afec,
		const enum afec_channel_num afec_ch)
{	
	uint32_t corrected_data = 0;
	uint32_t converted_data = 0;
	
	afec_ch_sanity_check(afec, afec_ch);

	afec->AFEC_CSELR = afec_ch;
	converted_data = afec->AFEC_CDR;

	corrected_data = (converted_data + (afec->AFEC_CVR & AFEC_CVR_OFFSETCORR_Msk)) * 
			(afec->AFEC_CVR >> AFEC_CVR_GAINCORR_Pos) / 1024u;
	return corrected_data;
	
}

/**
 * \brief Configure sample&hold mode.
 *
 * \param afec  Base address of the AFEC.
 * \param channel   AFEC Channel number.
 * \param mode sample&hold mode.
 */
void afec_set_sample_hold_mode(Afec *const afec,
		const enum afec_channel_num channel, const enum afec_sample_hold_mode mode)
{
	if (channel != AFEC_CHANNEL_ALL) {
		afec_ch_sanity_check(afec, channel);
	}
		
	uint32_t reg = 0;
	reg = afec->AFEC_SHMR;
	if (mode == AFEC_SAMPLE_HOLD_MODE_1) {
		
		reg |= (channel == AFEC_CHANNEL_ALL)? AFEC_CHANNEL_ALL : 0x1u << channel;
	}
	else {
		
		reg = (channel == AFEC_CHANNEL_ALL)? 0 : ~(0x1u << channel);
	}
	afec->AFEC_SHMR = reg;
		
}
#endif

//@}
