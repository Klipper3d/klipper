/**
 * \file
 *
 * \brief Chip-specific oscillator management functions
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
#define OSC_ID_OSC0             0       //!< External Oscillator 0
#define OSC_ID_OSC32            1       //!< External 32 kHz oscillator
#define OSC_ID_RC32K            2       //!< Internal 32 kHz RC oscillator
#define OSC_ID_RC80M            3       //!< Internal 80 MHz RC oscillator
#define OSC_ID_RCFAST           4       //!< Internal 4-8-12 MHz RCFAST oscillator
#define OSC_ID_RC1M             5       //!< Internal 1 MHz RC oscillator
#define OSC_ID_RCSYS            6       //!< Internal System RC oscillator
//@}

//! \name OSC0 mode values
//@{
//! External clock connected to XIN
#define OSC_MODE_EXTERNAL       0
//! Crystal connected to XIN/XOUT
#define OSC_MODE_XTAL           SCIF_OSCCTRL0_MODE
//@}

//! \name OSC32 mode values
//@{
//! External clock connected to XIN32
#define OSC32_MODE_EXTERNAL     BSCIF_OSCCTRL32_MODE(0)
//! Crystal connected to XIN32/XOUT32
#define OSC32_MODE_XTAL         BSCIF_OSCCTRL32_MODE(1)
//! Crystal connected to XIN32/XOUT32 in high current mode
#define OSC32_MODE_XTAL_HC      BSCIF_OSCCTRL32_MODE(4)
//@}

//! \name OSC0 startup values
//@{
//! 0 cycles
#define OSC_STARTUP_0           SCIF_OSCCTRL0_STARTUP(0)
//! 64 cycles (560 us)
#define OSC_STARTUP_64          SCIF_OSCCTRL0_STARTUP(1)
//! 128 cycles (1.1 ms)
#define OSC_STARTUP_128         SCIF_OSCCTRL0_STARTUP(2)
//! 2048 cycles (18 ms)
#define OSC_STARTUP_2048        SCIF_OSCCTRL0_STARTUP(3)
//! 4096 cycles (36 ms)
#define OSC_STARTUP_4096        SCIF_OSCCTRL0_STARTUP(4)
//! 8192 cycles (71 ms)
#define OSC_STARTUP_8192        SCIF_OSCCTRL0_STARTUP(5)
//! 16384 cycles (143 ms)
#define OSC_STARTUP_16384       SCIF_OSCCTRL0_STARTUP(6)
//! 32768 cycles (285 ms)
#define OSC_STARTUP_32768       SCIF_OSCCTRL0_STARTUP(7)
//@}

//! \name OSC32 startup values
//@{
//! 0 cycles
#define OSC32_STARTUP_0         BSCIF_OSCCTRL32_STARTUP(0)
//! 128 cycles (1.1 ms)
#define OSC32_STARTUP_128       BSCIF_OSCCTRL32_STARTUP(1)
//! 8192 cycles (72.3 ms)
#define OSC32_STARTUP_8192      BSCIF_OSCCTRL32_STARTUP(2)
//! 16384 cycles (143 ms)
#define OSC32_STARTUP_16384     BSCIF_OSCCTRL32_STARTUP(3)
//! 65536 cycles (570 ms)
#define OSC32_STARTUP_65536     BSCIF_OSCCTRL32_STARTUP(4)
//! 131072 cycles (1.1 s)
#define OSC32_STARTUP_131072    BSCIF_OSCCTRL32_STARTUP(5)
//! 262144 cycles (2.3 s)
#define OSC32_STARTUP_262144    BSCIF_OSCCTRL32_STARTUP(6)
//! 524288 cycles (4.6 s)
#define OSC32_STARTUP_524288    BSCIF_OSCCTRL32_STARTUP(7)
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
#   if BOARD_OSC0_STARTUP_US == 0
#      define OSC0_STARTUP_VALUE    OSC_STARTUP_0
#      define OSC0_STARTUP_TIMEOUT  8
#   elif BOARD_OSC0_STARTUP_US <= 557
#      define OSC0_STARTUP_VALUE    OSC_STARTUP_64
#      define OSC0_STARTUP_TIMEOUT  80
#   elif BOARD_OSC0_STARTUP_US <= 1100
#      define OSC0_STARTUP_VALUE    OSC_STARTUP_128
#      define OSC0_STARTUP_TIMEOUT  160
#   elif BOARD_OSC0_STARTUP_US <= 18000
#      define OSC0_STARTUP_VALUE    OSC_STARTUP_2048
#      define OSC0_STARTUP_TIMEOUT  2560
#   elif BOARD_OSC0_STARTUP_US <= 36000
#      define OSC0_STARTUP_VALUE    OSC_STARTUP_4096
#      define OSC0_STARTUP_TIMEOUT  5120
#   elif BOARD_OSC0_STARTUP_US <= 71000
#      define OSC0_STARTUP_VALUE    OSC_STARTUP_8192
#      define OSC0_STARTUP_TIMEOUT  10240
#   elif BOARD_OSC0_STARTUP_US <= 143000
#      define OSC0_STARTUP_VALUE    OSC_STARTUP_16384
#      define OSC0_STARTUP_TIMEOUT  20480
#   elif BOARD_OSC0_STARTUP_US <= 285000
#      define OSC0_STARTUP_VALUE    OSC_STARTUP_32768
#      define OSC0_STARTUP_TIMEOUT  40960
#   else
#      error BOARD_OSC0_STARTUP_US is too high
#   endif
#   if BOARD_OSC0_IS_XTAL == true
#      define OSC0_MODE_VALUE       OSC_MODE_XTAL
#      if BOARD_OSC0_HZ < 2000000
#         define OSC0_GAIN_VALUE      SCIF_OSCCTRL0_GAIN(0)
#      elif BOARD_OSC0_HZ < 4000000
#         define OSC0_GAIN_VALUE      SCIF_OSCCTRL0_GAIN(1)
#      elif BOARD_OSC0_HZ < 8000000
#         define OSC0_GAIN_VALUE      SCIF_OSCCTRL0_GAIN(2)
#      elif BOARD_OSC0_HZ < 16000000
#         define OSC0_GAIN_VALUE      SCIF_OSCCTRL0_GAIN(3)
#      else
#         define OSC0_GAIN_VALUE      ((0x1u << 4) | SCIF_OSCCTRL0_GAIN(0))
#      endif
#   else
#      define OSC0_MODE_VALUE       OSC_MODE_EXTERNAL
#   endif
#else
#   if defined(BOARD_OSC0_HZ)
#      error BOARD_OSC0_STARTUP_US must be defined by the board code
#   endif
#   ifdef __DOXYGEN__
#      define OSC0_STARTUP_VALUE     UNDEFINED
#      define OSC0_STARTUP_TIMEOUT   UNDEFINED
#      define OSC0_MODE_VALUE        UNDEFINED
#   endif
#endif

#if defined(BOARD_OSC32_STARTUP_US)
#   if BOARD_OSC32_STARTUP_US == 0
#      define OSC32_STARTUP_VALUE    OSC32_STARTUP_0
#   elif BOARD_OSC32_STARTUP_US   <= 1100
#      define OSC32_STARTUP_VALUE    OSC32_STARTUP_128
#   elif BOARD_OSC32_STARTUP_US   <= 72300
#      define OSC32_STARTUP_VALUE    OSC32_STARTUP_8192
#   elif BOARD_OSC32_STARTUP_US   <= 143000
#      define OSC32_STARTUP_VALUE    OSC32_STARTUP_16384
#   elif BOARD_OSC32_STARTUP_US   <= 570000
#      define OSC32_STARTUP_VALUE    OSC32_STARTUP_65536
#   elif BOARD_OSC32_STARTUP_US   <= 1100000
#      define OSC32_STARTUP_VALUE    OSC32_STARTUP_131072
#   elif BOARD_OSC32_STARTUP_US   <= 2300000
#      define OSC32_STARTUP_VALUE    OSC32_STARTUP_262144
#   elif BOARD_OSC32_STARTUP_US   <= 4600000
#      define OSC32_STARTUP_VALUE    OSC32_STARTUP_524288
#   else
#      error BOARD_OSC32_STARTUP_US is too high
#   endif
#   if BOARD_OSC32_IS_XTAL == true
#      define OSC32_MODE_VALUE       OSC32_MODE_XTAL
#   else
#      define OSC32_MODE_VALUE       OSC32_MODE_EXTERNAL
#   endif
#else
#   if defined(BOARD_OSC32_HZ)
#      error BOARD_OSC32_STARTUP_US must be defined by the board code
#   endif
#   ifdef __DOXYGEN__
#      define OSC32_STARTUP_VALUE     UNDEFINED
#      define OSC32_STARTUP_TIMEOUT   UNDEFINED
#      define OSC32_MODE_VALUE        UNDEFINED
#   endif
#endif

// Use 4 MHz frequency range for RCFAST oscillator if config was empty.
#ifndef CONFIG_RCFAST_FRANGE
#define CONFIG_RCFAST_FRANGE    0
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
/**
 * \def BOARD_OSC32_SELCURR
 * \brief Crystal current selection for OSC32
 *
 * If not defined, the recommended value (300nA) are used.
 */
#ifndef BOARD_OSC32_SELCURR
#   define BOARD_OSC32_SELCURR     BSCIF_OSCCTRL32_SELCURR(10)
#endif

/**
 * \name RC oscillator frequency limits
 * The frequency of the internal RC oscillators may drift a bit as a
 * result of temperature changes. These definitions provide upper and
 * lower limits which may be used to calculate upper and lower limits of
 * timeouts, derived clock frequencies, etc.
 */
//@{
//! Nominal frequency of RCSYS in Hz
#define OSC_RCSYS_NOMINAL_HZ    115000
//! Minimum frequency of RCSYS in Hz
#define OSC_RCSYS_MIN_HZ        100000
//! Maximum frequency of RCSYS in Hz
#define OSC_RCSYS_MAX_HZ        120000

//! Nominal frequency of RC32K in Hz
#define OSC_RC32K_NOMINAL_HZ    32768
//! Minimum frequency of RC32K in Hz
#define OSC_RC32K_MIN_HZ        20000
//! Maximum frequency of RC32K in Hz
#define OSC_RC32K_MAX_HZ        44000

//! Nominal frequency of RC80M in Hz
#define OSC_RC80M_NOMINAL_HZ    80000000

//! Nominal frequency of RCFAST4M in Hz
#define OSC_RCFAST4M_NOMINAL_HZ 4000000

//! Nominal frequency of RCFAST8M in Hz
#define OSC_RCFAST8M_NOMINAL_HZ 8000000

//! Nominal frequency of RCFAST12M in Hz
#define OSC_RCFAST12M_NOMINAL_HZ 12000000

//! Nominal frequency of RC1M in Hz
#define OSC_RC1M_NOMINAL_HZ     1000000
//@}

#ifndef __ASSEMBLY__

#include <compiler.h>

extern void osc_priv_enable_osc0(void);
extern void osc_priv_disable_osc0(void);
extern void osc_priv_enable_osc32(void);
extern void osc_priv_disable_osc32(void);
extern void osc_priv_enable_rc32k(void);
extern void osc_priv_disable_rc32k(void);
extern void osc_priv_enable_rc80m(void);
extern void osc_priv_disable_rc80m(void);
extern void osc_priv_enable_rcfast(void);
extern void osc_priv_disable_rcfast(void);
extern void osc_priv_enable_rc1m(void);
extern void osc_priv_disable_rc1m(void);

static inline void osc_enable(uint8_t id)
{
	switch (id) {
#ifdef BOARD_OSC0_HZ
	case OSC_ID_OSC0:
		osc_priv_enable_osc0();
		break;
#endif

#ifdef BOARD_OSC32_HZ
	case OSC_ID_OSC32:
		osc_priv_enable_osc32();
		break;
#endif

	case OSC_ID_RC32K:
		osc_priv_enable_rc32k();
		break;

	case OSC_ID_RC80M:
		osc_priv_enable_rc80m();
		break;

	case OSC_ID_RCFAST:
		osc_priv_enable_rcfast();
		break;

	case OSC_ID_RC1M:
		osc_priv_enable_rc1m();
		break;

	case OSC_ID_RCSYS:
		/* RCSYS is always running */
		break;

	default:
		/* unhandled_case(id); */
		break;
	}
}

static inline void osc_disable(uint8_t id)
{
	switch (id) {
#ifdef BOARD_OSC0_HZ
	case OSC_ID_OSC0:
		osc_priv_disable_osc0();
		break;
#endif

#ifdef BOARD_OSC32_HZ
	case OSC_ID_OSC32:
		osc_priv_disable_osc32();
		break;
#endif

	case OSC_ID_RC32K:
		osc_priv_disable_rc32k();
		break;

	case OSC_ID_RC80M:
		osc_priv_disable_rc80m();
		break;

	case OSC_ID_RCFAST:
		osc_priv_disable_rcfast();
		break;

	case OSC_ID_RC1M:
		osc_priv_disable_rc1m();
		break;

	case OSC_ID_RCSYS:
		/* RCSYS is always running */
		break;

	default:
		/* unhandled_case(id); */
		break;
	}
}

static inline bool osc_is_ready(uint8_t id)
{
	switch (id) {
#ifdef BOARD_OSC0_HZ
	case OSC_ID_OSC0:
		return !!(SCIF->SCIF_PCLKSR & SCIF_PCLKSR_OSC0RDY);
#endif

#ifdef BOARD_OSC32_HZ
	case OSC_ID_OSC32:
		return !!(BSCIF->BSCIF_PCLKSR & BSCIF_PCLKSR_OSC32RDY);
#endif

	case OSC_ID_RC32K:
		return !!(BSCIF->BSCIF_RC32KCR & (BSCIF_RC32KCR_EN));

	case OSC_ID_RC80M:
		return !!(SCIF->SCIF_RC80MCR & (SCIF_RC80MCR_EN));

	case OSC_ID_RCFAST:
		return !!(SCIF->SCIF_RCFASTCFG & (SCIF_RCFASTCFG_EN));

	case OSC_ID_RC1M:
		return !!(BSCIF->BSCIF_RC1MCR & (BSCIF_RC1MCR_CLKOE));

	case OSC_ID_RCSYS:
		/* RCSYS is always ready */
		return true;

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

#ifdef BOARD_OSC32_HZ
	case OSC_ID_OSC32:
		return BOARD_OSC32_HZ;
#endif

	case OSC_ID_RC32K:
		return OSC_RC32K_NOMINAL_HZ;

	case OSC_ID_RC80M:
		return OSC_RC80M_NOMINAL_HZ;

	case OSC_ID_RCFAST:
		if (CONFIG_RCFAST_FRANGE == 2) {
			return OSC_RCFAST12M_NOMINAL_HZ;

		} else if (CONFIG_RCFAST_FRANGE == 1) {
			return OSC_RCFAST8M_NOMINAL_HZ;

		} else {
			return OSC_RCFAST4M_NOMINAL_HZ;
		}

	case OSC_ID_RC1M:
		return OSC_RC1M_NOMINAL_HZ;

	case OSC_ID_RCSYS:
		return OSC_RCSYS_NOMINAL_HZ;

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
