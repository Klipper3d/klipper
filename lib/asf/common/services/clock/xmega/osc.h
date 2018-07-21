/**
 * \file
 *
 * \brief Chip-specific oscillator management functions
 *
 * Copyright (c) 2010-2018 Microchip Technology Inc. and its subsidiaries.
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
#ifndef XMEGA_OSC_H_INCLUDED
#define XMEGA_OSC_H_INCLUDED

#include <compiler.h>
#include <board.h>

/**
 * \weakgroup osc_group
 *
 * \section osc_group_errata Errata
 *   - Auto-calibration does not work on XMEGA A1 revision H and
 *     earlier.
 * @{
 */

//! \name Oscillator identifiers
//@{
//! 2 MHz Internal RC Oscillator
#define OSC_ID_RC2MHZ         OSC_RC2MEN_bm
//! 32 MHz Internal RC Oscillator
#define OSC_ID_RC32MHZ        OSC_RC32MEN_bm
//! 32 KHz Internal RC Oscillator
#define OSC_ID_RC32KHZ        OSC_RC32KEN_bm
//! External Oscillator
#define OSC_ID_XOSC           OSC_XOSCEN_bm
#if XMEGA_E
//! 8 MHz Internal RC Oscillator
# define OSC_ID_RC8MHZ        OSC_RC8MEN_bm
#endif

/**
 * \brief Reference from USB Start Of Frame
 * \note This cannot be enabled or disabled, but can be used as a reference for
 * the autocalibration (DFLL).
 */
#define OSC_ID_USBSOF         0xff
//@}

//! \name External oscillator types
//@{
#define XOSC_TYPE_EXTERNAL        0      //!< External clock signal
#define XOSC_TYPE_32KHZ           2      //!< 32.768 kHz resonator on TOSC
#define XOSC_TYPE_XTAL            3      //!< 0.4 to 16 MHz resonator on XTAL
//@}

/**
 * \def CONFIG_XOSC_32KHZ_LPM
 * \brief Define for enabling Low Power Mode for 32 kHz external oscillator.
 */
#ifdef __DOXYGEN__
# define CONFIG_XOSC_32KHZ_LPM
#endif /* __DOXYGEN__ */

/**
 * \def CONFIG_XOSC_STARTUP
 * \brief Board-dependent value that determines the number of start-up cycles
 * for external resonators, based on BOARD_XOSC_STARTUP_US. This is written to
 * the two MSB of the XOSCSEL field of OSC.XOSCCTRL.
 *
 * \note This is automatically computed from BOARD_XOSC_HZ and
 * BOARD_XOSC_STARTUP_US if it is not manually set.
 */

//! \name XTAL resonator start-up cycles
//@{
#define XOSC_STARTUP_256      0   //!< 256 cycle start-up time
#define XOSC_STARTUP_1024     1   //!< 1 k cycle start-up time
#define XOSC_STARTUP_16384    2   //!< 16 k cycle start-up time
//@}

/**
 * \def CONFIG_XOSC_RANGE
 * \brief Board-dependent value that sets the frequency range of the external
 * oscillator. This is written to the FRQRANGE field of OSC.XOSCCTRL.
 *
 * \note This is automatically computed from BOARD_XOSC_HZ if it is not manually
 * set.
 */

//! \name XTAL resonator frequency range
//@{
//! 0.4 to 2 MHz frequency range
#define XOSC_RANGE_04TO2      OSC_FRQRANGE_04TO2_gc
//! 2 to 9 MHz frequency range
#define XOSC_RANGE_2TO9       OSC_FRQRANGE_2TO9_gc
//! 9 to 12 MHz frequency range
#define XOSC_RANGE_9TO12      OSC_FRQRANGE_9TO12_gc
//! 12 to 16 MHz frequency range
#define XOSC_RANGE_12TO16     OSC_FRQRANGE_12TO16_gc
//@}

/**
 * \def XOSC_STARTUP_TIMEOUT
 * \brief Number of us to wait for XOSC to start
 *
 * This is the number of slow clock cycles corresponding to
 * OSC0_STARTUP_VALUE with an additional 25% safety margin. If the
 * oscillator isn't running when this timeout has expired, it is assumed
 * to have failed to start.
 */

// If application intends to use XOSC.
#ifdef BOARD_XOSC_HZ
// Get start-up config for XOSC, if not manually set.
# ifndef CONFIG_XOSC_STARTUP
#  ifndef BOARD_XOSC_STARTUP_US
#   error BOARD_XOSC_STARTUP_US must be configured.
#  else
//! \internal Number of start-up cycles for the board's XOSC.
#   define BOARD_XOSC_STARTUP_CYCLES \
		(BOARD_XOSC_HZ / 1000000 * BOARD_XOSC_STARTUP_US)

#   if (BOARD_XOSC_TYPE == XOSC_TYPE_XTAL)
#    if (BOARD_XOSC_STARTUP_CYCLES > 16384)
#     error BOARD_XOSC_STARTUP_US is too high for current BOARD_XOSC_HZ.

#    elif (BOARD_XOSC_STARTUP_CYCLES > 1024)
#     define CONFIG_XOSC_STARTUP    XOSC_STARTUP_16384
#     define XOSC_STARTUP_TIMEOUT   (16384*(1000000/BOARD_XOSC_HZ))

#    elif (BOARD_XOSC_STARTUP_CYCLES > 256)
#     define CONFIG_XOSC_STARTUP    XOSC_STARTUP_1024
#     define XOSC_STARTUP_TIMEOUT   (1024*(1000000/BOARD_XOSC_HZ))

#    else
#     define CONFIG_XOSC_STARTUP    XOSC_STARTUP_256
#     define XOSC_STARTUP_TIMEOUT   (256*(1000000/BOARD_XOSC_HZ))
#    endif
#   else /* BOARD_XOSC_TYPE == XOSC_TYPE_XTAL */
#    define CONFIG_XOSC_STARTUP     0
#   endif
#  endif /* BOARD_XOSC_STARTUP_US */
# endif /* CONFIG_XOSC_STARTUP */

// Get frequency range setting for XOSC, if not manually set.
# ifndef CONFIG_XOSC_RANGE
#  if (BOARD_XOSC_TYPE == XOSC_TYPE_XTAL)
#   if (BOARD_XOSC_HZ < 400000)
#    error BOARD_XOSC_HZ is below minimum frequency of 400 kHz.

#   elif (BOARD_XOSC_HZ < 2000000)
#    define CONFIG_XOSC_RANGE    XOSC_RANGE_04TO2

#   elif (BOARD_XOSC_HZ < 9000000)
#    define CONFIG_XOSC_RANGE    XOSC_RANGE_2TO9

#   elif (BOARD_XOSC_HZ < 12000000)
#    define CONFIG_XOSC_RANGE    XOSC_RANGE_9TO12

#   elif (BOARD_XOSC_HZ <= 16000000)
#    define CONFIG_XOSC_RANGE    XOSC_RANGE_12TO16

#   else
#    error BOARD_XOSC_HZ is above maximum frequency of 16 MHz.
#   endif
#  else /* BOARD_XOSC_TYPE == XOSC_TYPE_XTAL */
#   define CONFIG_XOSC_RANGE     0
#  endif
# endif /* CONFIG_XOSC_RANGE */
#endif /* BOARD_XOSC_HZ */

#ifndef __ASSEMBLY__

/**
 * \internal
 * \brief Enable internal oscillator \a id
 *
 * Do not call this function directly. Use osc_enable() instead.
 */
static inline void osc_enable_internal(uint8_t id)
{
	irqflags_t flags;

	Assert(id != OSC_ID_USBSOF);

	flags = cpu_irq_save();
	OSC.CTRL |= id;
#if (XMEGA_E && CONFIG_SYSCLK_RC8MHZ_LPM)
	if(id == OSC_ID_RC8MHZ) {
		OSC.CTRL |= OSC_RC8MLPM_bm;
	}
#endif
	cpu_irq_restore(flags);
}

#if defined(BOARD_XOSC_HZ) || defined(__DOXYGEN__)

/**
 * \internal
 * \brief Enable external oscillator \a id
 *
 * Do not call this function directly. Use osc_enable() instead. Also
 * note that this function is only available if the board actually has
 * an external oscillator crystal.
 */
static inline void osc_enable_external(uint8_t id)
{
	irqflags_t flags;

	Assert(id == OSC_ID_XOSC);

#ifndef CONFIG_XOSC_32KHZ_LPM
#  if (XMEGA_E && (BOARD_XOSC_TYPE == XOSC_TYPE_EXTERNAL) && defined(CONFIG_XOSC_EXTERNAL_PC4))
	OSC.XOSCCTRL = OSC_XOSCSEL4_bm;
#  else
	OSC.XOSCCTRL = BOARD_XOSC_TYPE | (CONFIG_XOSC_STARTUP << 2) |
			CONFIG_XOSC_RANGE;
#  endif
#else
	OSC.XOSCCTRL = BOARD_XOSC_TYPE | (CONFIG_XOSC_STARTUP << 2) |
			CONFIG_XOSC_RANGE | OSC_X32KLPM_bm;
#endif /* CONFIG_XOSC_32KHZ_LPM */

	flags = cpu_irq_save();
	OSC.CTRL |= id;
	cpu_irq_restore(flags);
}
#else

static inline void osc_enable_external(uint8_t id)
{
	Assert(false); // No external oscillator on the selected board
}
#endif

static inline void osc_disable(uint8_t id)
{
	irqflags_t flags;

	Assert(id != OSC_ID_USBSOF);

	flags = cpu_irq_save();
	OSC.CTRL &= ~id;
	cpu_irq_restore(flags);
}

static inline void osc_enable(uint8_t id)
{
	if (id != OSC_ID_XOSC) {
		osc_enable_internal(id);
	} else {
		osc_enable_external(id);
	}
}

static inline bool osc_is_ready(uint8_t id)
{
	Assert(id != OSC_ID_USBSOF);

	return OSC.STATUS & id;
}

//! \name XMEGA-Specific Oscillator Features
//@{

/**
 * \brief Enable DFLL-based automatic calibration of an internal
 * oscillator.
 *
 * The XMEGA features two Digital Frequency Locked Loops (DFLLs) which
 * can be used to improve the accuracy of the 2 MHz and 32 MHz internal
 * RC oscillators. The DFLL compares the oscillator frequency with a
 * more accurate reference clock to do automatic run-time calibration of
 * the oscillator.
 *
 * This function enables auto-calibration for either the 2 MHz or 32 MHz
 * internal oscillator using either the 32.768 kHz calibrated internal
 * oscillator or an external crystal oscillator as a reference. If the
 * latter option is used, the crystal must be connected to the TOSC pins
 * and run at 32.768 kHz.
 *
 * \param id The ID of the oscillator for which to enable
 * auto-calibration:
 * \arg \c OSC_ID_RC2MHZ or \c OSC_ID_RC32MHZ.
 * \param ref_id The ID of the oscillator to use as a reference:
 * \arg \c OSC_ID_RC32KHZ or \c OSC_ID_XOSC for internal or external 32 kHz
 * reference, respectively.
 * \arg \c OSC_ID_USBSOF for 32 MHz only when USB is available and running.
 */
static inline void osc_enable_autocalibration(uint8_t id, uint8_t ref_id)
{
	irqflags_t flags;

	flags = cpu_irq_save();
	switch (id) {
	case OSC_ID_RC2MHZ:
#if !XMEGA_E
		Assert((ref_id == OSC_ID_RC32KHZ) || (ref_id == OSC_ID_XOSC));
		if (ref_id == OSC_ID_XOSC) {
			osc_enable(OSC_ID_RC32KHZ);
			OSC.DFLLCTRL |= OSC_RC2MCREF_bm;
		} else {
			OSC.DFLLCTRL &= ~(OSC_RC2MCREF_bm);
		}
		DFLLRC2M.CTRL |= DFLL_ENABLE_bm;
#endif
		break;

	case OSC_ID_RC32MHZ:
#if XMEGA_AU || XMEGA_B || XMEGA_C || XMEGA_E
		Assert((ref_id == OSC_ID_RC32KHZ)
				|| (ref_id == OSC_ID_XOSC)
# if !XMEGA_E
				|| (ref_id == OSC_ID_USBSOF)
#endif
				);

		OSC.DFLLCTRL &= ~(OSC_RC32MCREF_gm);

		if (ref_id == OSC_ID_XOSC) {
			osc_enable(OSC_ID_RC32KHZ);
			OSC.DFLLCTRL |= OSC_RC32MCREF_XOSC32K_gc;
		}
		else if (ref_id == OSC_ID_RC32KHZ) {
			OSC.DFLLCTRL |= OSC_RC32MCREF_RC32K_gc;
		}
# if !XMEGA_E
		else if (ref_id == OSC_ID_USBSOF) {
			/*
			 * Calibrate 32MRC at 48MHz using USB SOF
			 * 48MHz / 1kHz = 0xBB80
			 */
			DFLLRC32M.COMP1 = 0x80;
			DFLLRC32M.COMP2 = 0xBB;
			OSC.DFLLCTRL |= OSC_RC32MCREF_USBSOF_gc;
		}
# endif
#else
		Assert((ref_id == OSC_ID_RC32KHZ) ||
				(ref_id == OSC_ID_XOSC));

# if defined(OSC_RC32MCREF_gm)
		OSC.DFLLCTRL &= ~(OSC_RC32MCREF_gm);
# endif

		if (ref_id == OSC_ID_XOSC) {
			osc_enable(OSC_ID_RC32KHZ);
# if defined(OSC_RC32MCREF_gm)
			OSC.DFLLCTRL |= OSC_RC32MCREF_XOSC32K_gc;
# else
			OSC.DFLLCTRL |= OSC_RC32MCREF_bm;
# endif
		}
		else if (ref_id == OSC_ID_RC32KHZ) {
# if defined(OSC_RC32MCREF_gm)
			OSC.DFLLCTRL |= OSC_RC32MCREF_RC32K_gc;
# else
			OSC.DFLLCTRL &= ~(OSC_RC32MCREF_bm);
# endif
		}
#endif

		DFLLRC32M.CTRL |= DFLL_ENABLE_bm;
		break;

	default:
		Assert(false);
		break;
	}
	cpu_irq_restore(flags);
}

/**
 * \brief Disable DFLL-based automatic calibration of an internal
 * oscillator.
 *
 * \see osc_enable_autocalibration
 *
 * \param id The ID of the oscillator for which to disable
 * auto-calibration:
 * \arg \c OSC_ID_RC2MHZ or \c OSC_ID_RC32MHZ.
 */
static inline void osc_disable_autocalibration(uint8_t id)
{
	switch (id) {
	case OSC_ID_RC2MHZ:
#if !XMEGA_E
		DFLLRC2M.CTRL = 0;
#endif
		break;

	case OSC_ID_RC32MHZ:
		DFLLRC32M.CTRL = 0;
		break;

	default:
		Assert(false);
		break;
	}
}

/**
 * \brief Load a specific calibration value for the specified oscillator.
 *
 * \param id The ID of the oscillator for which to disable
 * auto-calibration:
 * \arg \c OSC_ID_RC2MHZ or \c OSC_ID_RC32MHZ.
 * \param calib The specific calibration value required:
 *
 */
static inline void osc_user_calibration(uint8_t id, uint16_t calib)
{
	switch (id) {
	case OSC_ID_RC2MHZ:
#if !XMEGA_E
		DFLLRC2M.CALA=LSB(calib);
		DFLLRC2M.CALB=MSB(calib);
#endif
		break;

	case OSC_ID_RC32MHZ:
		DFLLRC32M.CALA=LSB(calib);
		DFLLRC32M.CALB=MSB(calib);
		break;

#if XMEGA_E
	case OSC_ID_RC8MHZ:
		OSC.RC8MCAL=LSB(calib);
		break;
#endif

	default:
		Assert(false);
		break;
	}
}
//@}

static inline uint32_t osc_get_rate(uint8_t id)
{
	Assert(id != OSC_ID_USBSOF);

	switch (id) {
	case OSC_ID_RC2MHZ:
		return 2000000UL;

	case OSC_ID_RC32MHZ:
#ifdef CONFIG_OSC_RC32_CAL
		return CONFIG_OSC_RC32_CAL;
#else
		return 32000000UL;
#endif

	case OSC_ID_RC32KHZ:
		return 32768UL;

#ifdef BOARD_XOSC_HZ
	case OSC_ID_XOSC:
		return BOARD_XOSC_HZ;
#endif

	default:
		Assert(false);
		return 0;
	}
}

#endif /* __ASSEMBLY__ */

//! @}

#endif /* XMEGA_OSC_H_INCLUDED */
