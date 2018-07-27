/**
 * \file
 *
 * \brief Chip-specific oscillator management functions.
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

#ifndef CHIP_OSC_H_INCLUDED
#define CHIP_OSC_H_INCLUDED

#include "board.h"
#include "pmc.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/*
 * Below BOARD_XXX macros are related to the specific board, and
 * should be defined by the board code, otherwise default value are used.
 */
#if !defined(BOARD_FREQ_SLCK_XTAL)
#  warning The board slow clock xtal frequency has not been defined.
#  define BOARD_FREQ_SLCK_XTAL      (32768UL)
#endif

#if !defined(BOARD_FREQ_SLCK_BYPASS)
#  warning The board slow clock bypass frequency has not been defined.
#  define BOARD_FREQ_SLCK_BYPASS    (32768UL)
#endif

#if !defined(BOARD_FREQ_MAINCK_XTAL)
#  warning The board main clock xtal frequency has not been defined.
#  define BOARD_FREQ_MAINCK_XTAL    (12000000UL)
#endif

#if !defined(BOARD_FREQ_MAINCK_BYPASS)
#  warning The board main clock bypass frequency has not been defined.
#  define BOARD_FREQ_MAINCK_BYPASS  (12000000UL)
#endif

#if !defined(BOARD_OSC_STARTUP_US)
#  warning The board main clock xtal startup time has not been defined.
#  define BOARD_OSC_STARTUP_US      (15625UL)
#endif

/**
 * \weakgroup osc_group
 * @{
 */

//! \name Oscillator identifiers
//@{
#define OSC_SLCK_32K_RC             0    //!< Internal 32kHz RC oscillator.
#define OSC_SLCK_32K_XTAL           1    //!< External 32kHz crystal oscillator.
#define OSC_SLCK_32K_BYPASS         2    //!< External 32kHz bypass oscillator.
#define OSC_MAINCK_4M_RC            3    //!< Internal 4MHz RC oscillator.
#define OSC_MAINCK_8M_RC            4    //!< Internal 8MHz RC oscillator.
#define OSC_MAINCK_12M_RC           5    //!< Internal 12MHz RC oscillator.
#define OSC_MAINCK_XTAL             6    //!< External crystal oscillator.
#define OSC_MAINCK_BYPASS           7    //!< External bypass oscillator.
//@}

//! \name Oscillator clock speed in hertz
//@{
#define OSC_SLCK_32K_RC_HZ          CHIP_FREQ_SLCK_RC               //!< Internal 32kHz RC oscillator.
#define OSC_SLCK_32K_XTAL_HZ        BOARD_FREQ_SLCK_XTAL            //!< External 32kHz crystal oscillator.
#define OSC_SLCK_32K_BYPASS_HZ      BOARD_FREQ_SLCK_BYPASS          //!< External 32kHz bypass oscillator.
#define OSC_MAINCK_4M_RC_HZ         CHIP_FREQ_MAINCK_RC_4MHZ        //!< Internal 4MHz RC oscillator.
#define OSC_MAINCK_8M_RC_HZ         CHIP_FREQ_MAINCK_RC_8MHZ        //!< Internal 8MHz RC oscillator.
#define OSC_MAINCK_12M_RC_HZ        CHIP_FREQ_MAINCK_RC_12MHZ       //!< Internal 12MHz RC oscillator.
#define OSC_MAINCK_XTAL_HZ          BOARD_FREQ_MAINCK_XTAL          //!< External crystal oscillator.
#define OSC_MAINCK_BYPASS_HZ        BOARD_FREQ_MAINCK_BYPASS        //!< External bypass oscillator.
//@}

static inline void osc_enable(uint32_t ul_id)
{
	switch (ul_id) {
	case OSC_SLCK_32K_RC:
		break;

	case OSC_SLCK_32K_XTAL:
		pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);
		break;

	case OSC_SLCK_32K_BYPASS:
		pmc_switch_sclk_to_32kxtal(PMC_OSC_BYPASS);
		break;


	case OSC_MAINCK_4M_RC:
		pmc_osc_enable_fastrc(CKGR_MOR_MOSCRCF_4_MHz);
		break;

	case OSC_MAINCK_8M_RC:
		pmc_osc_enable_fastrc(CKGR_MOR_MOSCRCF_8_MHz);
		break;

	case OSC_MAINCK_12M_RC:
		pmc_osc_enable_fastrc(CKGR_MOR_MOSCRCF_12_MHz);
		break;


	case OSC_MAINCK_XTAL:
		pmc_osc_enable_main_xtal(pmc_us_to_moscxtst(
				BOARD_OSC_STARTUP_US, OSC_SLCK_32K_RC_HZ));
		break;

	case OSC_MAINCK_BYPASS:
		pmc_osc_bypass_main_xtal();
		break;
	}
}

static inline void osc_disable(uint32_t ul_id)
{
	switch (ul_id) {
	case OSC_SLCK_32K_RC:
	case OSC_SLCK_32K_XTAL:
	case OSC_SLCK_32K_BYPASS:
		break;

	case OSC_MAINCK_4M_RC:
	case OSC_MAINCK_8M_RC:
	case OSC_MAINCK_12M_RC:
		pmc_osc_disable_fastrc();
		break;

	case OSC_MAINCK_XTAL:
		pmc_osc_disable_xtal(PMC_OSC_XTAL);
		break;

	case OSC_MAINCK_BYPASS:
		pmc_osc_disable_xtal(PMC_OSC_BYPASS);
		break;
	}
}

static inline bool osc_is_ready(uint32_t ul_id)
{
	switch (ul_id) {
	case OSC_SLCK_32K_RC:
		return 1;

	case OSC_SLCK_32K_XTAL:
	case OSC_SLCK_32K_BYPASS:
		return pmc_osc_is_ready_32kxtal();

	case OSC_MAINCK_4M_RC:
	case OSC_MAINCK_8M_RC:
	case OSC_MAINCK_12M_RC:
		return pmc_osc_is_ready_fastrc();

	case OSC_MAINCK_XTAL:
	case OSC_MAINCK_BYPASS:
		return pmc_osc_is_ready_main_xtal();
	}

	return 0;
}

static inline uint32_t osc_get_rate(uint32_t ul_id)
{
	switch (ul_id) {
	case OSC_SLCK_32K_RC:
		return OSC_SLCK_32K_RC_HZ;

	case OSC_SLCK_32K_XTAL:
		return BOARD_FREQ_SLCK_XTAL;

	case OSC_SLCK_32K_BYPASS:
		return BOARD_FREQ_SLCK_BYPASS;

	case OSC_MAINCK_4M_RC:
		return OSC_MAINCK_4M_RC_HZ;

	case OSC_MAINCK_8M_RC:
		return OSC_MAINCK_8M_RC_HZ;

	case OSC_MAINCK_12M_RC:
		return OSC_MAINCK_12M_RC_HZ;

	case OSC_MAINCK_XTAL:
		return BOARD_FREQ_MAINCK_XTAL;

	case OSC_MAINCK_BYPASS:
		return BOARD_FREQ_MAINCK_BYPASS;
	}

	return 0;
}

//! @}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* CHIP_OSC_H_INCLUDED */
