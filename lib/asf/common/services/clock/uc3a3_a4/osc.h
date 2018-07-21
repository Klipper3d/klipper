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
#ifndef CHIP_OSC_H_INCLUDED
#define CHIP_OSC_H_INCLUDED

#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \weakgroup osc_group
 * @{
 */

//! \name Oscillator identifiers
//@{
#define OSC_ID_OSC0             0    //!< External Oscillator 0
#define OSC_ID_OSC1             1    //!< External Oscillator 1
#define OSC_ID_OSC32            2    //!< External 32 kHz oscillator
//@}

//! \name OSC0/OSC1 mode values
//@{
//! External clock connected to XIN
#define OSC_MODE_EXTERNAL       AVR32_PM_MODE_EXT_CLOCK
//! Crystal connected to XIN/XOUT. Use oscillator gain G0 (400 kHz to 900 kHz)
#define OSC_MODE_XTAL_G0        AVR32_PM_MODE_CRYSTAL_G0
//! Crystal connected to XIN/XOUT. Use oscillator gain G1 (900 kHz to 3 MHz)
#define OSC_MODE_XTAL_G1        AVR32_PM_MODE_CRYSTAL_G1
//! Crystal connected to XIN/XOUT. Use oscillator gain G2 (3 MHz to 8 MHz)
#define OSC_MODE_XTAL_G2        AVR32_PM_MODE_CRYSTAL_G2
//! Crystal connected to XIN/XOUT. Use oscillator gain G3 (8 MHz and higher)
#define OSC_MODE_XTAL_G3        AVR32_PM_MODE_CRYSTAL_G3
//@}

//! \name OSC32 mode values
//@{
//! External clock connected to XIN32
#define OSC32_MODE_EXTERNAL     AVR32_PM_OSCCTRL32_MODE_EXT_CLOCK
//! Crystal connected to XIN32/XOUT32. Use automatic gain control
#define OSC32_MODE_XTAL         AVR32_PM_OSCCTRL32_MODE_CRYSTAL
//@}

//! \name OSC0/OSC1 startup values
//@{
//! 0 cycles
#define OSC_STARTUP_0           AVR32_PM_OSCCTRL0_STARTUP_0_RCOSC
//! 64 cycles (560 us)
#define OSC_STARTUP_64          AVR32_PM_OSCCTRL0_STARTUP_64_RCOSC
//! 128 cycles (1.1 ms)
#define OSC_STARTUP_128         AVR32_PM_OSCCTRL0_STARTUP_128_RCOSC
//! 2048 cycles (18 ms)
#define OSC_STARTUP_2048        AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC
//! 4096 cycles (36 ms)
#define OSC_STARTUP_4096        AVR32_PM_OSCCTRL0_STARTUP_4096_RCOSC
//! 8192 cycles (71 ms)
#define OSC_STARTUP_8192        AVR32_PM_OSCCTRL0_STARTUP_8192_RCOSC
//! 16384 cycles (142 ms)
#define OSC_STARTUP_16384       AVR32_PM_OSCCTRL0_STARTUP_16384_RCOSC
//@}

//! \name OSC32 startup values
//@{
//! 0 cycles
#define OSC32_STARTUP_0         AVR32_PM_OSCCTRL32_STARTUP_0_RCOSC
//! 128 cycles (1.1 ms)
#define OSC32_STARTUP_128       AVR32_PM_OSCCTRL32_STARTUP_128_RCOSC
//! 8192 cycles (72.3 ms)
#define OSC32_STARTUP_8192      AVR32_PM_OSCCTRL32_STARTUP_8192_RCOSC
//! 16384 cycles (143 ms)
#define OSC32_STARTUP_16384     AVR32_PM_OSCCTRL32_STARTUP_16384_RCOSC
//! 65536 cycles (570 ms)
#define OSC32_STARTUP_65536     AVR32_PM_OSCCTRL32_STARTUP_65536_RCOSC
//! 131072 cycles (1.1 s)
#define OSC32_STARTUP_131072    AVR32_PM_OSCCTRL32_STARTUP_131072_RCOSC
//! 262144 cycles (2.3 s)
#define OSC32_STARTUP_262144    AVR32_PM_OSCCTRL32_STARTUP_262144_RCOSC
//! 524288 cycles (4.6 s)
#define OSC32_STARTUP_524288    AVR32_PM_OSCCTRL32_STARTUP_524288_RCOSC
//@}

/**
 * \def OSC0_STARTUP_TIMEOUT
 * \brief Number of slow clock cycles to wait for OSC0 to start
 *
 * This is the number of slow clock cycles corresponding to
 * OSC0_STARTUP_VALUE with an additional 25% safety margin. If the
 * oscillator isn't running when this timeout has expired, it is assumed
 * to have failed to start.
 */
/**
 * \def OSC0_MODE_VALUE
 * \brief Board-dependent value written to the MODE bitfield of
 * PM_OSCCTRL(0)
 */
/**
 * \def OSC0_STARTUP_VALUE
 * \brief Board-dependent value written to the STARTUP bitfield of
 * PM_OSCCTRL(0)
 */

#if defined(BOARD_OSC0_STARTUP_US)
# if BOARD_OSC0_STARTUP_US == 0
#  define OSC0_STARTUP_VALUE    OSC_STARTUP_0
#  define OSC0_STARTUP_TIMEOUT  8
# elif BOARD_OSC0_STARTUP_US <= 560
#  define OSC0_STARTUP_VALUE    OSC_STARTUP_64
#  define OSC0_STARTUP_TIMEOUT  80
# elif BOARD_OSC0_STARTUP_US <= 1100
#  define OSC0_STARTUP_VALUE    OSC_STARTUP_128
#  define OSC0_STARTUP_TIMEOUT  160
# elif BOARD_OSC0_STARTUP_US <= 18000
#  define OSC0_STARTUP_VALUE    OSC_STARTUP_2048
#  define OSC0_STARTUP_TIMEOUT  2560
# elif BOARD_OSC0_STARTUP_US <= 36000
#  define OSC0_STARTUP_VALUE    OSC_STARTUP_4096
#  define OSC0_STARTUP_TIMEOUT  5120
# elif BOARD_OSC0_STARTUP_US <= 71000
#  define OSC0_STARTUP_VALUE    OSC_STARTUP_8192
#  define OSC0_STARTUP_TIMEOUT  10240
# elif BOARD_OSC0_STARTUP_US <= 142000
#  define OSC0_STARTUP_VALUE    OSC_STARTUP_16384
#  define OSC0_STARTUP_TIMEOUT  20480
# else
#  error BOARD_OSC0_STARTUP_US is too high
# endif
# if BOARD_OSC0_IS_XTAL == true
#  if BOARD_OSC0_HZ < 900000
#   define OSC0_MODE_VALUE      OSC_MODE_XTAL_G0
#  elif BOARD_OSC0_HZ < 3000000
#   define OSC0_MODE_VALUE      OSC_MODE_XTAL_G1
#  elif BOARD_OSC0_HZ < 8000000
#   define OSC0_MODE_VALUE      OSC_MODE_XTAL_G2
#  else
#   define OSC0_MODE_VALUE      OSC_MODE_XTAL_G3
#  endif
# else
#  define OSC0_MODE_VALUE       OSC_MODE_EXTERNAL
# endif
#else
# ifdef BOARD_OSC0_HZ
#  error BOARD_OSC0_STARTUP_US must be defined by the board code
# endif
# ifdef __DOXYGEN__
#  define OSC0_STARTUP_VALUE     UNDEFINED
#  define OSC0_STARTUP_TIMEOUT   UNDEFINED
#  define OSC0_MODE_VALUE        UNDEFINED
# endif
#endif

/**
 * \def OSC1_STARTUP_VALUE
 * \brief Board-dependent value written to the STARTUP bitfield of
 * PM_OSCCTRL(1)
 */
/**
 * \def OSC1_STARTUP_TIMEOUT
 * \brief Number of slow clock cycles to wait for OSC1 to start
 *
 * This is the number of slow clock cycles corresponding to
 * OSC1_STARTUP_VALUE with an additional 25% safety margin. If the
 * oscillator isn't running when this timeout has expired, it is assumed
 * to have failed to start.
 */
/**
 * \def OSC1_MODE_VALUE
 * \brief Board-dependent value written to the MODE bitfield of
 * PM_OSCCTRL(1)
 */
#if defined(BOARD_OSC1_STARTUP_US)
# if BOARD_OSC1_STARTUP_US == 0
#  define OSC1_STARTUP_VALUE    OSC_STARTUP_0
#  define OSC1_STARTUP_TIMEOUT  8
# elif BOARD_OSC1_STARTUP_US <= 560
#  define OSC1_STARTUP_VALUE    OSC_STARTUP_64
#  define OSC1_STARTUP_TIMEOUT  80
# elif BOARD_OSC1_STARTUP_US <= 1100
#  define OSC1_STARTUP_VALUE    OSC_STARTUP_128
#  define OSC1_STARTUP_TIMEOUT  160
# elif BOARD_OSC1_STARTUP_US <= 18000
#  define OSC1_STARTUP_VALUE    OSC_STARTUP_2048
#  define OSC1_STARTUP_TIMEOUT  2560
# elif BOARD_OSC1_STARTUP_US <= 36000
#  define OSC1_STARTUP_VALUE    OSC_STARTUP_4096
#  define OSC1_STARTUP_TIMEOUT  5120
# elif BOARD_OSC1_STARTUP_US <= 71000
#  define OSC1_STARTUP_VALUE    OSC_STARTUP_8192
#  define OSC1_STARTUP_TIMEOUT  10240
# elif BOARD_OSC1_STARTUP_US <= 142000
#  define OSC1_STARTUP_VALUE    OSC_STARTUP_16384
#  define OSC1_STARTUP_TIMEOUT  20480
# else
#  error BOARD_OSC1_STARTUP_US is too high
# endif
# ifdef BOARD_OSC1_IS_XTAL
#  if BOARD_OSC1_HZ < 900000
#   define OSC1_MODE_VALUE      OSC_MODE_XTAL_G0
#  elif BOARD_OSC1_HZ < 3000000
#   define OSC1_MODE_VALUE      OSC_MODE_XTAL_G1
#  elif BOARD_OSC1_HZ < 8000000
#   define OSC1_MODE_VALUE      OSC_MODE_XTAL_G2
#  else
#   define OSC1_MODE_VALUE      OSC_MODE_XTAL_G3
#  endif
# else
#  define OSC1_MODE_VALUE       OSC_MODE_EXTERNAL
# endif
#else
# ifdef __DOXYGEN__
#  define OSC1_STARTUP_VALUE     UNDEFINED
#  define OSC1_STARTUP_TIMEOUT   UNDEFINED
#  define OSC1_MODE_VALUE        UNDEFINED
# endif
#endif

#if defined(BOARD_OSC32_STARTUP_US)
# if BOARD_OSC32_STARTUP_US == 0
#  define OSC32_STARTUP_VALUE    OSC32_STARTUP_0
# elif BOARD_OSC32_STARTUP_US <= 1100
#  define OSC32_STARTUP_VALUE    OSC32_STARTUP_128
# elif BOARD_OSC32_STARTUP_US <= 72300
#  define OSC32_STARTUP_VALUE    OSC32_STARTUP_8192
# elif BOARD_OSC32_STARTUP_US <= 143000
#  define OSC32_STARTUP_VALUE    OSC32_STARTUP_16384
# elif BOARD_OSC32_STARTUP_US <= 570000
#  define OSC32_STARTUP_VALUE    OSC32_STARTUP_65536
# elif BOARD_OSC32_STARTUP_US <= 1100000
#  define OSC32_STARTUP_VALUE    OSC32_STARTUP_131072
# elif BOARD_OSC32_STARTUP_US <= 2300000
#  define OSC32_STARTUP_VALUE    OSC32_STARTUP_262144
# elif BOARD_OSC32_STARTUP_US <= 4600000
#  define OSC32_STARTUP_VALUE    OSC32_STARTUP_524288
# else
#  error BOARD_OSC32_STARTUP_US is too high
# endif
# if BOARD_OSC32_IS_XTAL == true
#  define OSC32_MODE_VALUE       OSC32_MODE_XTAL
# else
#  define OSC32_MODE_VALUE       OSC32_MODE_EXTERNAL
# endif
#else
# if defined(BOARD_OSC32_HZ)
#  error BOARD_OSC32_STARTUP_US must be defined by the board code
# endif
# ifdef __DOXYGEN__
#  define OSC32_STARTUP_VALUE     UNDEFINED
#  define OSC32_STARTUP_TIMEOUT   UNDEFINED
#  define OSC32_MODE_VALUE        UNDEFINED
# endif
#endif

/**
 * \name Board-specific configuration parameters
 * The following definitions must be provided by the board code for all
 * working oscillators on the board.
 */
//@{
/**
 * \def BOARD_OSC0_HZ
 * \brief Clock frequency of OSC0 in Hz
 */
/**
 * \def BOARD_OSC0_STARTUP_US
 * \brief Startup time of OSC0 in microseconds
 */
/**
 * \def BOARD_OSC0_IS_XTAL
 * \brief OSC0 uses a crystal, not an external clock
 */
/**
 * \def BOARD_OSC1_HZ
 * \brief Clock frequency of OSC1 in Hz
 */
/**
 * \def BOARD_OSC1_STARTUP_US
 * \brief Startup time of OSC1 in microseconds
 */
/**
 * \def BOARD_OSC1_IS_XTAL
 * \brief OSC1 uses a crystal, not an external clock
 */
/**
 * \def BOARD_OSC32_HZ
 * \brief Clock frequency of OSC32 in Hz
 */
/**
 * \def BOARD_OSC32_STARTUP_US
 * \brief Startup time of OSC32 in microseconds
 */
/**
 * \def BOARD_OSC32_IS_XTAL
 * \brief OSC32 uses a crystal, not an external clock
 */
#if !defined(BOARD_OSC0_HZ)
# ifdef __DOXYGEN__
#  define BOARD_OSC0_HZ            UNDEFINED
# endif
#endif
#if !defined(BOARD_OSC0_STARTUP_US)
# ifdef __DOXYGEN__
#  define BOARD_OSC0_STARTUP_US    UNDEFINED
# endif
#endif
#if !defined(BOARD_OSC0_IS_XTAL)
# ifdef __DOXYGEN__
#  define BOARD_OSC0_IS_XTAL       UNDEFINED
# endif
#endif
#if !defined(BOARD_OSC1_HZ)
# ifdef __DOXYGEN__
#  define BOARD_OSC1_HZ            UNDEFINED
# endif
#endif
#if !defined(BOARD_OSC1_STARTUP_US)
# ifdef __DOXYGEN__
#  define BOARD_OSC1_STARTUP_US    UNDEFINED
# endif
#endif
#if !defined(BOARD_OSC1_IS_XTAL)
# ifdef __DOXYGEN__
#  define BOARD_OSC1_IS_XTAL       UNDEFINED
# endif
#endif
#if !defined(BOARD_OSC32_HZ)
# ifdef __DOXYGEN__
#  define BOARD_OSC32_HZ           UNDEFINED
# endif
#endif
#if !defined(BOARD_OSC32_STARTUP_US)
# ifdef __DOXYGEN__
#  define BOARD_OSC32_STARTUP_US   UNDEFINED
# endif
#endif
#if !defined(BOARD_OSC32_IS_XTAL)
# ifdef __DOXYGEN__
#  define BOARD_OSC32_IS_XTAL      UNDEFINED
# endif
#endif
/**
 * \name Slow clock frequency limits
 * The slow clock is an internal RC oscillator whose frequency may drift
 * a bit as a result of temperature changes. These definitions provide
 * upper and lower limits which may be used to calculate upper and lower
 * limits of timeouts, derived clock frequencies, etc.
 */
//@{
//! Nominal frequency of the slow clock in Hz
#define OSC_SLOW_NOMINAL_HZ     AVR32_PM_RCOSC_FREQUENCY
//! Minimum frequency of the slow clock in Hz
#define OSC_SLOW_MIN_HZ         100000
//! Maximum frequency of the slow clock in Hz
#define OSC_SLOW_MAX_HZ         120000
//@}

#ifndef __ASSEMBLY__

#include <stdbool.h>
#include <stdint.h>
#include <avr32/io.h>

static inline void osc_enable(uint8_t id)
{
	irqflags_t flags;
	uint32_t   oscctrl;

	flags = cpu_irq_save();

	switch (id) {
#ifdef BOARD_OSC0_HZ
	case OSC_ID_OSC0:
		oscctrl = OSC0_STARTUP_VALUE <<
				AVR32_PM_OSCCTRL0_STARTUP_OFFSET;
		oscctrl |= OSC0_MODE_VALUE << AVR32_PM_OSCCTRL0_MODE_OFFSET;
		AVR32_PM.oscctrl0 = oscctrl;
		AVR32_PM.mcctrl |= 1U << AVR32_PM_MCCTRL_OSC0EN;
		break;
#endif

#ifdef BOARD_OSC1_HZ
	case OSC_ID_OSC1:
		oscctrl = OSC1_STARTUP_VALUE <<
				AVR32_PM_OSCCTRL1_STARTUP_OFFSET;
		oscctrl |= OSC1_MODE_VALUE << AVR32_PM_OSCCTRL1_MODE_OFFSET;
		AVR32_PM.oscctrl1 = oscctrl;
		AVR32_PM.mcctrl |= 1U << AVR32_PM_MCCTRL_OSC1EN;
		break;
#endif

#ifdef BOARD_OSC32_HZ
	case OSC_ID_OSC32:
		oscctrl = OSC32_STARTUP_VALUE
				<< AVR32_PM_OSCCTRL32_STARTUP_OFFSET;
		oscctrl |= OSC32_MODE_VALUE << AVR32_PM_OSCCTRL32_MODE_OFFSET;
		oscctrl |= 1 << AVR32_PM_OSCCTRL32_OSC32EN_OFFSET;
		AVR32_PM.oscctrl32 = oscctrl;
		break;
#endif

	default:
		/* unhandled_case(id); */
		break;
	}

	cpu_irq_restore(flags);
}

static inline void osc_disable(uint8_t id)
{
	irqflags_t flags;

	flags = cpu_irq_save();

	switch (id) {
#ifdef BOARD_OSC0_HZ
	case OSC_ID_OSC0:
		AVR32_PM.mcctrl &= ~(1U << AVR32_PM_MCCTRL_OSC0EN);
		break;
#endif

#ifdef BOARD_OSC1_HZ
	case OSC_ID_OSC1:
		AVR32_PM.mcctrl &= ~(1U << AVR32_PM_MCCTRL_OSC1EN);
		break;
#endif

#ifdef BOARD_OSC32_HZ
	case OSC_ID_OSC32:
		AVR32_PM.oscctrl32 = 0;
		break;
#endif

	default:
		/* unhandled_case(id); */
		break;
	}
	cpu_irq_restore(flags);
}

static inline bool osc_is_ready(uint8_t id)
{
	switch (id) {
#ifdef BOARD_OSC0_HZ
	case OSC_ID_OSC0:
		return !!(AVR32_PM.poscsr & (1U << AVR32_PM_POSCSR_OSC0RDY));
#endif

#ifdef BOARD_OSC1_HZ
	case OSC_ID_OSC1:
		return !!(AVR32_PM.poscsr & (1U << AVR32_PM_POSCSR_OSC1RDY));
#endif

#ifdef BOARD_OSC32_HZ
	case OSC_ID_OSC32:
		return !!(AVR32_PM.poscsr & (1U << AVR32_PM_POSCSR_OSC32RDY));
#endif

	default:
		/* unhandled_case(id); */
		return false;
	}
}

static inline uint32_t osc_get_rate(uint8_t id)
{
	switch (id) {
#ifdef BOARD_OSC0_HZ
	case OSC_ID_OSC0:
		return BOARD_OSC0_HZ;
#endif

#ifdef BOARD_OSC1_HZ
	case OSC_ID_OSC1:
		return BOARD_OSC1_HZ;
#endif

#ifdef BOARD_OSC32_HZ
	case OSC_ID_OSC32:
		return BOARD_OSC32_HZ;
#endif
	default:
		/* unhandled_case(id); */
		return 0;
	}
}

#endif /* !__ASSEMBLY__ */

//! @}

#ifdef __cplusplus
}
#endif

#endif /* CHIP_OSC_H_INCLUDED */
