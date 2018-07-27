/**
 * \file
 *
 * \brief Oscillator management
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
#ifndef OSC_H_INCLUDED
#define OSC_H_INCLUDED

#include "parts.h"
#include "conf_clock.h"

#if SAM3S
# include "sam3s/osc.h"
#elif SAM3XA
# include "sam3x/osc.h"
#elif SAM3U
# include "sam3u/osc.h"
#elif SAM3N
# include "sam3n/osc.h"
#elif SAM4S
# include "sam4s/osc.h"
#elif SAM4E
# include "sam4e/osc.h"
#elif SAM4C
# include "sam4c/osc.h"
#elif SAM4CM
# include "sam4cm/osc.h"
#elif SAM4CP
# include "sam4cp/osc.h"
#elif SAM4L
# include "sam4l/osc.h"
#elif SAM4N
# include "sam4n/osc.h"
#elif SAMG
# include "samg/osc.h"
#elif SAMV71
# include "samv71/osc.h"
#elif SAMV70
# include "samv70/osc.h"
#elif SAME70
# include "same70/osc.h"
#elif SAMS70
# include "sams70/osc.h"
#elif (UC3A0 || UC3A1)
# include "uc3a0_a1/osc.h"
#elif UC3A3
# include "uc3a3_a4/osc.h"
#elif UC3B
# include "uc3b0_b1/osc.h"
#elif UC3C
# include "uc3c/osc.h"
#elif UC3D
# include "uc3d/osc.h"
#elif UC3L
# include "uc3l/osc.h"
#elif XMEGA
# include "xmega/osc.h"
#else
# error Unsupported chip type
#endif

/**
 * \ingroup clk_group
 * \defgroup osc_group Oscillator Management
 *
 * This group contains functions and definitions related to configuring
 * and enabling/disabling on-chip oscillators. Internal RC-oscillators,
 * external crystal oscillators and external clock generators are
 * supported by this module. What all of these have in common is that
 * they swing at a fixed, nominal frequency which is normally not
 * adjustable.
 *
 * \par Example: Enabling an oscillator
 *
 * The following example demonstrates how to enable the external
 * oscillator on XMEGA A and wait for it to be ready to use. The
 * oscillator identifiers are platform-specific, so while the same
 * procedure is used on all platforms, the parameter to osc_enable()
 * will be different from device to device.
 * \code
	osc_enable(OSC_ID_XOSC);
	osc_wait_ready(OSC_ID_XOSC); \endcode
 *
 * \section osc_group_board Board-specific Definitions
 * If external oscillators are used, the board code must provide the
 * following definitions for each of those:
 *   - \b BOARD_<osc name>_HZ: The nominal frequency of the oscillator.
 *   - \b BOARD_<osc name>_STARTUP_US: The startup time of the
 *     oscillator in microseconds.
 *   - \b BOARD_<osc name>_TYPE: The type of oscillator connected, i.e.
 *     whether it's a crystal or external clock, and sometimes what kind
 *     of crystal it is. The meaning of this value is platform-specific.
 *
 * @{
 */

//! \name Oscillator Management
//@{
/**
 * \fn void osc_enable(uint8_t id)
 * \brief Enable oscillator \a id
 *
 * The startup time and mode value is automatically determined based on
 * definitions in the board code.
 */
/**
 * \fn void osc_disable(uint8_t id)
 * \brief Disable oscillator \a id
 */
/**
 * \fn osc_is_ready(uint8_t id)
 * \brief Determine whether oscillator \a id is ready.
 * \retval true Oscillator \a id is running and ready to use as a clock
 * source.
 * \retval false Oscillator \a id is not running.
 */
/**
 * \fn uint32_t osc_get_rate(uint8_t id)
 * \brief Return the frequency of oscillator \a id in Hz
 */

#ifndef __ASSEMBLY__

/**
 * \brief Wait until the oscillator identified by \a id is ready
 *
 * This function will busy-wait for the oscillator identified by \a id
 * to become stable and ready to use as a clock source.
 *
 * \param id A number identifying the oscillator to wait for.
 */
static inline void osc_wait_ready(uint8_t id)
{
	while (!osc_is_ready(id)) {
		/* Do nothing */
	}
}

#endif /* __ASSEMBLY__ */

//@}

//! @}

#endif /* OSC_H_INCLUDED */
