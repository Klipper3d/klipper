/**
 * \file
 *
 * \brief SAM Timer Counter (TC) driver.
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

#include <assert.h>
#include "tc.h"

/// @cond
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#ifndef TC_WPMR_WPKEY_PASSWD
#define TC_WPMR_WPKEY_PASSWD TC_WPMR_WPKEY((uint32_t)0x54494D)
#endif

/**
 * \brief Configure TC for timer, waveform generation, or capture.
 *
 * \param[in,out] p_tc   Module hardware register base address pointer
 * \param[in] ul_channel Channel to configure
 * \param[in] ul_mode    Control mode register bitmask value to set
 *
 * \note For more information regarding <i>ul_mode</i> configuration refer to
 * the section entitled "Channel Mode Register: Capture Mode" and/or section
 * "Waveform Operating Mode" in the device-specific datasheet.
 *
 * \note If the TC is configured for waveform generation then the external event
 * selection (EEVT) should only be set to TC_CMR_EEVT_TIOB, or the
 * equivalent value of 0, if it really is the intention to use TIOB as an
 * external event trigger. This is because this setting forces TIOB to be
 * an input, even if the external event trigger has not been enabled with
 * TC_CMR_ENETRG, and thus prevents normal operation of TIOB.
 */
void tc_init(
		Tc *p_tc,
		uint32_t ul_channel,
		uint32_t ul_mode)
{
	TcChannel *tc_channel;

	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));
	tc_channel = p_tc->TC_CHANNEL + ul_channel;

	/*  Disable TC clock. */
	tc_channel->TC_CCR = TC_CCR_CLKDIS;

	/*  Disable interrupts. */
	tc_channel->TC_IDR = 0xFFFFFFFF;

	/*  Clear status register. */
	tc_channel->TC_SR;

	/*  Set mode. */
	tc_channel->TC_CMR = ul_mode;
}

/**
 * \brief Asserts a SYNC signal to generate a software trigger on
 * all channels.
 *
 * \param[out] p_tc Module hardware register base address pointer
 *
 */
void tc_sync_trigger(
		Tc *p_tc)
{
	/* Validate inputs. */
	Assert(p_tc);
	
	p_tc->TC_BCR = TC_BCR_SYNC;
}

/**
 * \brief Configure the TC Block mode.
 *
 * \note The function tc_init() must be called prior to this one.
 *
 * \param[out] p_tc        Module hardware register base address pointer
 * \param[in] ul_blockmode Block mode register value to set
 *
 * \note For more information regarding <i>ul_blockmode</i> configuration refer to
 * the section  entitled "TC Block Mode Register" in the device-specific datasheet.
 */
void tc_set_block_mode(
		Tc *p_tc,
		uint32_t ul_blockmode)
{
	/* Validate inputs. */
	Assert(p_tc);
	
	p_tc->TC_BMR = ul_blockmode;
}

#if (!SAM3U) || defined(__DOXYGEN__)

/**
 * \brief Configure TC for 2-bit Gray Counter for Stepper Motor.
 * \note The function tc_init() must be called prior to this one.
 *
 * \note This function is not available on SAM3U devices.
 *
 * \param[out] p_tc          Module hardware register base address pointer
 * \param[in] ul_channel     Channel to configure
 * \param[in] ul_steppermode Stepper motor mode register value to set
 *
 * \return 0 for OK.
 */
uint32_t tc_init_2bit_gray(
		Tc *p_tc,
		uint32_t ul_channel,
		uint32_t ul_steppermode)
{
	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));

	p_tc->TC_CHANNEL[ul_channel].TC_SMMR = ul_steppermode;
	return 0;
}

#endif /* (!SAM3U) || defined(__DOXYGEN__) */

/**
 * \brief Start the TC clock on the specified channel.
 *
 * \param[out] p_tc      Module hardware register base address pointer
 * \param[in] ul_channel Channel to configure
 */
void tc_start(
		Tc *p_tc,
		uint32_t ul_channel)
{
	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));

	p_tc->TC_CHANNEL[ul_channel].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

/**
 * \brief Stop the TC clock on the specified channel.
 *
 * \param[out] p_tc      Module hardware register base address pointer
 * \param[in] ul_channel Channel to configure
 */
void tc_stop(
		Tc *p_tc,
		uint32_t ul_channel)
{
	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));

	p_tc->TC_CHANNEL[ul_channel].TC_CCR = TC_CCR_CLKDIS;
}

/**
 * \brief Read the counter value on the specified channel.
 *
 * \param[in] p_tc       Module hardware register base address pointer
 * \param[in] ul_channel Channel to read
 *
 * \return The counter value.
 */
uint32_t tc_read_cv(
		Tc *p_tc,
		uint32_t ul_channel)
{
	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));

	return p_tc->TC_CHANNEL[ul_channel].TC_CV;
}

/**
 * \brief Read TC Register A (RA) on the specified channel.
 *
 * \param[in] p_tc       Module hardware register base address pointer
 * \param[in] ul_channel Channel to read
 *
 * \return The TC Register A (RA) value.
 */
uint32_t tc_read_ra(
		Tc *p_tc,
		uint32_t ul_channel)
{
	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));

	return p_tc->TC_CHANNEL[ul_channel].TC_RA;
}

/**
 * \brief Read TC Register B (RB) on the specified channel.
 *
 * \param[in] p_tc       Module hardware register base address pointer
 * \param[in] ul_channel Channel to read
 *
 * \return The TC Register B (RB) value.
 */
uint32_t tc_read_rb(
		Tc *p_tc,
		uint32_t ul_channel)
{
	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));

	return p_tc->TC_CHANNEL[ul_channel].TC_RB;
}

/**
 * \brief Read TC Register C (RC) on the specified channel.
 *
 * \param[in] p_tc       Module hardware register base address pointer
 * \param[in] ul_channel Channel to read
 *
 * \return The Register C (RC) value.
 */
uint32_t tc_read_rc(
		Tc *p_tc,
		uint32_t ul_channel)
{
	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));

	return p_tc->TC_CHANNEL[ul_channel].TC_RC;
}

/**
 * \brief Write to TC Register A (RA) on the specified channel.
 *
 * \param[out] p_tc      Module hardware register base address pointer
 * \param[in] ul_channel Channel to write
 * \param[in] ul_value   Value to write
 */
void tc_write_ra(
		Tc *p_tc,
		uint32_t ul_channel,
		uint32_t ul_value)
{
	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));

	p_tc->TC_CHANNEL[ul_channel].TC_RA = ul_value;
}

/**
 * \brief Write to TC Register B (RB) on the specified channel.
 *
 * \param[out] p_tc      Module hardware register base address pointer
 * \param[in] ul_channel Channel to write
 * \param[in] ul_value   Value to write
 */
void tc_write_rb(
		Tc *p_tc,
		uint32_t ul_channel,
		uint32_t ul_value)
{
	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));

	p_tc->TC_CHANNEL[ul_channel].TC_RB = ul_value;
}

/**
 * \brief Write to TC Register C (RC) on the selected channel.
 *
 * \param[out] p_tc      Module hardware register base address pointer
 * \param[in] ul_channel Channel to write
 * \param[in] ul_value   Value to write
 */
void tc_write_rc(
		Tc *p_tc,
		uint32_t ul_channel,
		uint32_t ul_value)
{
	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));

	p_tc->TC_CHANNEL[ul_channel].TC_RC = ul_value;
}

/**
 * \brief Enable the TC interrupts on the specified channel.
 *
 * \param[in,out] p_tc   Module hardware register base address pointer
 * \param[in] ul_channel Channel to configure
 * \param[in] ul_sources Bitmask of interrupt sources
 *
 * Where the input parameter <i>ul_sources</i> can be one or more of the following:
 * <table>
 * <tr>
 *    <th>Parameter Value</th>
 *    <th>Description</th>
 * </tr>
 *     <tr><td>TC_IER_COVFS</td><td>Enables the Counter Overflow Interrupt</td></tr>
 *     <tr><td>TC_IER_LOVRS</td><td>Enables the Load Overrun Interrupt</td></tr>
 *     <tr><td>TC_IER_CPAS</td><td>Enables the RA Compare Interrupt</td></tr>
 *     <tr><td>TC_IER_CPBS</td><td>Enables the RB Compare Interrupt</td></tr>
 *     <tr><td>TC_IER_CPCS</td><td>Enables the RC Compare Interrupt</td></tr>
 *     <tr><td>TC_IER_LDRAS</td><td>Enables the RA Load Interrupt</td></tr>
 *     <tr><td>TC_IER_LDRBS</td><td>Enables the RB Load Interrupt</td></tr>
 *     <tr><td>TC_IER_ETRGS</td><td>Enables the External Trigger Interrupt</td></tr>
 * </table>
 */
void tc_enable_interrupt(
		Tc *p_tc,
		uint32_t ul_channel,
		uint32_t ul_sources)
{
	TcChannel *tc_channel;

	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));
	tc_channel = p_tc->TC_CHANNEL + ul_channel;
	tc_channel->TC_IER = ul_sources;
}

/**
 * \brief Disable TC interrupts on the specified channel.
 *
 * \param[in,out] p_tc   Module hardware register base address pointer
 * \param[in] ul_channel Channel to configure
 * \param[in] ul_sources A bitmask of Interrupt sources
 *
 * Where the input parameter <i>ul_sources</i> can be one or more of the following:
 * <table>
 * <tr>
 *    <th>Parameter Value</th>
 *    <th>Description</th>
 * </tr>
 *     <tr><td>TC_IDR_COVFS</td><td>Disables the Counter Overflow Interrupt</td></tr>
 *     <tr><td>TC_IDR_LOVRS</td><td>Disables the Load Overrun Interrupt</td></tr>
 *     <tr><td>TC_IDR_CPAS</td><td>Disables the RA Compare Interrupt</td></tr>
 *     <tr><td>TC_IDR_CPBS</td><td>Disables the RB Compare Interrupt</td></tr>
 *     <tr><td>TC_IDR_CPCS</td><td>Disables the RC Compare Interrupt</td></tr>
 *     <tr><td>TC_IDR_LDRAS</td><td>Disables the RA Load Interrupt</td></tr>
 *     <tr><td>TC_IDR_LDRBS</td><td>Disables the RB Load Interrupt</td></tr>
 *     <tr><td>TC_IDR_ETRGS</td><td>Disables the External Trigger Interrupt</td></tr>
 * </table>
 */
void tc_disable_interrupt(
		Tc *p_tc,
		uint32_t ul_channel,
		uint32_t ul_sources)
{
	TcChannel *tc_channel;

	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));
	tc_channel = p_tc->TC_CHANNEL + ul_channel;
	tc_channel->TC_IDR = ul_sources;
}

/**
 * \brief Read the TC interrupt mask for the specified channel.
 *
 * \param[in] p_tc       Module hardware register base address pointer
 * \param[in] ul_channel Channel to read
 *
 * \return The TC interrupt mask value.
 */
uint32_t tc_get_interrupt_mask(
		Tc *p_tc,
		uint32_t ul_channel)
{
	TcChannel *tc_channel;

	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));
	tc_channel = p_tc->TC_CHANNEL + ul_channel;
	return tc_channel->TC_IMR;
}

/**
 * \brief Get the current status for the specified TC channel.
 *
 * \param[in] p_tc       Module hardware register base address pointer
 * \param[in] ul_channel Channel number
 *
 * \return The current TC status.
 */
uint32_t tc_get_status(
		Tc *p_tc,
		uint32_t ul_channel)
{
	TcChannel *tc_channel;

	/* Validate inputs. */
	Assert(p_tc);
	Assert(ul_channel <
			(sizeof(p_tc->TC_CHANNEL) / sizeof(p_tc->TC_CHANNEL[0])));
			
	tc_channel = p_tc->TC_CHANNEL + ul_channel;
	return tc_channel->TC_SR;
}

/* TC divisor used to find the lowest acceptable timer frequency */
#define TC_DIV_FACTOR 65536

#if (!SAM4L) && !defined(__DOXYGEN__)

#ifndef FREQ_SLOW_CLOCK_EXT
#define FREQ_SLOW_CLOCK_EXT 32768 /* External slow clock frequency (hz) */
#endif

/**
 * \brief Find the best MCK divisor.
 *
 * Finds the best MCK divisor given the timer frequency and MCK. The result
 * is guaranteed to satisfy the following equation:
 * \code (MCK / (DIV * 65536)) <= freq <= (MCK / DIV) \endcode
 * With DIV being the lowest possible value, to maximize timing adjust resolution.
 *
 * \param[in] ul_freq     Desired timer frequency
 * \param[in] ul_mck      Master clock frequency
 * \param[out] p_uldiv    Divisor value
 * \param[out] p_ultcclks TCCLKS field value for divisor
 * \param[in] ul_boardmck Board clock frequency
 *
 * \return The divisor found status.
 * \retval 0 No suitable divisor was found
 * \retval 1 A divisor was found
 */
uint32_t tc_find_mck_divisor(
		uint32_t ul_freq,
		uint32_t ul_mck,
		uint32_t *p_uldiv,
		uint32_t *p_ultcclks,
		uint32_t ul_boardmck)
{
	const uint32_t divisors[5] = { 2, 8, 32, 128,
			ul_boardmck / FREQ_SLOW_CLOCK_EXT };
	uint32_t ul_index;
	uint32_t ul_high, ul_low;

	/*  Satisfy frequency bound. */
	for (ul_index = 0;
			ul_index < (sizeof(divisors) / sizeof(divisors[0]));
			ul_index++) {
		ul_high = ul_mck / divisors[ul_index];
		ul_low  = ul_high / TC_DIV_FACTOR;
		if (ul_freq > ul_high) {
			return 0;
		} else if (ul_freq >= ul_low) {
			break;
		}
	}
	if (ul_index >= (sizeof(divisors) / sizeof(divisors[0]))) {
		return 0;
	}

	/*  Store results. */
	if (p_uldiv) {
		*p_uldiv = divisors[ul_index];
	}

	if (p_ultcclks) {
		*p_ultcclks = ul_index;
	}

	return 1;
}

#endif /* (!SAM4L) */

#if (SAM4L) || defined(__DOXYGEN__)
/**
 * \brief Find the best PBA/MCK divisor.
 *
 * <b>For SAM4L devices:</b> Finds the best PBA divisor given the timer
 * frequency and PBA clock. The result is guaranteed to satisfy the following equation:
 * \code  (ul_pbaclk / (2* DIV * 65536)) <= freq <= (ul_pbaclk / (2* DIV)) \endcode
 * with DIV being the lowest possible value, to maximize timing adjust resolution.
 *
 * <b>For non SAM4L devices:</b> Finds the best MCK divisor given the timer frequency
 * and MCK. The result is guaranteed to satisfy the following equation:
 * \code (MCK / (DIV * 65536)) <= freq <= (MCK / DIV) \endcode
 * with DIV being the lowest possible value, to maximize timing adjust resolution.
 *
 * \param[in] ul_freq     Desired timer frequency
 * \param[in] ul_mck      PBA clock frequency
 * \param[out] p_uldiv    Divisor value
 * \param[out] p_ultcclks TCCLKS field value for divisor
 * \param[in] ul_boardmck Board clock frequency (set to 0 for SAM4L devices)
 *
 * \return The divisor found status.
 * \retval 0 No suitable divisor was found
 * \retval 1 A divisor was found
 */
uint32_t tc_find_mck_divisor(
		uint32_t ul_freq,
		uint32_t ul_mck,
		uint32_t *p_uldiv,
		uint32_t *p_ultcclks,
		uint32_t ul_boardmck)
{
	const uint32_t divisors[5] = { 0, 2, 8, 32, 128};
	uint32_t ul_index;
	uint32_t ul_high, ul_low;

	UNUSED(ul_boardmck);

	/*  Satisfy frequency bound. */
	for (ul_index = 1;
			ul_index < (sizeof(divisors) / sizeof(divisors[0]));
			ul_index++) {
		ul_high = ul_mck / divisors[ul_index];
		ul_low  = ul_high / TC_DIV_FACTOR;
		if (ul_freq > ul_high) {
			return 0;
		} else if (ul_freq >= ul_low) {
			break;
		}
	}
	if (ul_index >= (sizeof(divisors) / sizeof(divisors[0]))) {
		return 0;
	}

	/*  Store results. */
	if (p_uldiv) {
		*p_uldiv = divisors[ul_index];
	}

	if (p_ultcclks) {
		*p_ultcclks = ul_index;
	}

	return 1;
}

#endif /* (SAM4L) || defined(__DOXYGEN__) */

#if (!SAM4L && !SAMG) || defined(__DOXYGEN__)

/**
 * \brief Enable TC QDEC interrupts.
 *
 * \note This function is not available on SAM4L or SAMG devices.
 *
 * \param[out] p_tc      Module hardware register base address pointer
 * \param[in] ul_sources A bitmask of QDEC interrupts to be enabled
 *
 * Where the input parameter <i>ul_sources</i> can be one or more of the following:
 * <table>
 * <tr>
 *    <th>Parameter Value</th>
 *    <th>Description</th>
 * </tr>
 *     <tr><td>TC_QIER_IDX</td><td>Enable the rising edge detected on IDX input interrupt</td></tr>
 *     <tr><td>TC_QIER_DIRCHG</td><td>Enable the change in rotation direction detected interrupt</td></tr>
 *     <tr><td>TC_QIER_QERR</td><td>Enable the quadrature error detected on PHA/PHB interrupt</td></tr>
 * </table>
 */
void tc_enable_qdec_interrupt(
		Tc *p_tc,
		uint32_t ul_sources)
{
	/* Validate inputs. */
	Assert(p_tc);
	
	p_tc->TC_QIER = ul_sources;
}

/**
 * \brief Disable TC QDEC interrupts.
 *
 * \note This function is not available on SAM4L or SAMG devices.
 *
 * \param[out] p_tc      Module hardware register base address pointer
 * \param[in] ul_sources A bitmask of QDEC interrupts to be disabled
 *
 * Where the input parameter <i>ul_sources</i> can be one or more of the following:
 * <table>
 * <tr>
 *    <th>Parameter Value</th>
 *    <th>Description</th>
 * </tr>
 *     <tr><td>TC_QIDR_IDX</td><td>Disable the rising edge detected on IDX input interrupt</td></tr>
 *     <tr><td>TC_QIDR_DIRCHG</td><td>Disable the change in rotation direction detected interrupt</td></tr>
 *     <tr><td>TC_QIDR_QERR</td><td>Disable the quadrature error detected on PHA/PHB interrupt</td></tr>
 * </table>
 */
void tc_disable_qdec_interrupt(
		Tc *p_tc,
		uint32_t ul_sources)
{
	/* Validate inputs. */
	Assert(p_tc);
	
	p_tc->TC_QIDR = ul_sources;
}

/**
 * \brief Read TC QDEC interrupt mask.
 *
 * \note This function is not available on SAM4L or SAMG devices.
 *
 * \param[in] p_tc Module hardware register base address pointer
 *
 * \return The QDEC interrupt mask value.
 */
uint32_t tc_get_qdec_interrupt_mask(
		Tc *p_tc)
{
	/* Validate inputs. */
	Assert(p_tc);
	
	return p_tc->TC_QIMR;
}

/**
 * \brief Get current TC QDEC interrupt status.
 *
 * \note This function is not available on SAM4L or SAMG devices.
 *
 * \param[in] p_tc Module hardware register base address pointer
 *
 * \return The TC QDEC interrupt status.
 */
uint32_t tc_get_qdec_interrupt_status(
		Tc *p_tc)
{
	/* Validate inputs. */
	Assert(p_tc);
	
	return p_tc->TC_QISR;
}

#endif /* (!SAM4L && !SAMG) || defined(__DOXYGEN__) */

#if (!SAM3U) || defined(__DOXYGEN__)

/**
 * \brief Enable or disable write protection of TC registers.
 *
 * \note This function is not available on SAM3U devices.
 *
 * \param[out] p_tc     Module hardware register base address pointer
 * \param[in] ul_enable 1 to enable, 0 to disable
 */
void tc_set_writeprotect(
		Tc *p_tc,
		uint32_t ul_enable)
{
	/* Validate inputs. */
	Assert(p_tc);
	
	if (ul_enable) {
		p_tc->TC_WPMR = TC_WPMR_WPKEY_PASSWD | TC_WPMR_WPEN;
	} else {
		p_tc->TC_WPMR = TC_WPMR_WPKEY_PASSWD;
	}
}

#endif /* (!SAM3U) || defined(__DOXYGEN__) */

#if SAM4L || defined(__DOXYGEN__)

/**
 * \brief Indicate TC features.
 *
 * \note This function is only available on SAM4L devices.
 *
 * \param[in] p_tc Module hardware register base address pointer
 *
 * \return The TC FEATURES register contents.
 */
uint32_t tc_get_feature(
		Tc *p_tc)
{
	/* Validate inputs. */
	Assert(p_tc);
	
	return p_tc->TC_FEATURES;
}

/**
 * \brief Indicate TC version.
 *
 * \note This function is only available on SAM4L devices.
 *
 * \param[in] p_tc Module hardware register base address pointer
 *
 * \return The TC VERSION register contents.
 */
uint32_t tc_get_version(
		Tc *p_tc)
{
	/* Validate inputs. */
	Assert(p_tc);
	
	return p_tc->TC_VERSION;
}

#endif /* SAM4L || defined(__DOXYGEN__) */

/// @cond
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
