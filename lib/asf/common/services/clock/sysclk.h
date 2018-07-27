/**
 * \file
 *
 * \brief System clock management
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
#ifndef SYSCLK_H_INCLUDED
#define SYSCLK_H_INCLUDED

#include "parts.h"
#include "conf_clock.h"

#if SAM3S
# include "sam3s/sysclk.h"
#elif SAM3U
# include "sam3u/sysclk.h"
#elif SAM3N
# include "sam3n/sysclk.h"
#elif SAM3XA
# include "sam3x/sysclk.h"
#elif SAM4S
# include "sam4s/sysclk.h"
#elif SAM4E
# include "sam4e/sysclk.h"
#elif SAM4C
# include "sam4c/sysclk.h"
#elif SAM4CM
# include "sam4cm/sysclk.h"
#elif SAM4CP
# include "sam4cp/sysclk.h"
#elif SAM4L
# include "sam4l/sysclk.h"
#elif SAM4N
# include "sam4n/sysclk.h"
#elif SAMG
# include "samg/sysclk.h"
#elif SAMV71
# include "samv71/sysclk.h"
#elif SAMV70
# include "samv70/sysclk.h"
#elif SAME70
# include "same70/sysclk.h"
#elif SAMS70
# include "sams70/sysclk.h"
#elif (UC3A0 || UC3A1)
# include "uc3a0_a1/sysclk.h"
#elif UC3A3
# include "uc3a3_a4/sysclk.h"
#elif UC3B
# include "uc3b0_b1/sysclk.h"
#elif UC3C
# include "uc3c/sysclk.h"
#elif UC3D
# include "uc3d/sysclk.h"
#elif UC3L
# include "uc3l/sysclk.h"
#elif XMEGA
# include "xmega/sysclk.h"
#elif MEGA
# include "mega/sysclk.h"
#else
# error Unsupported chip type
#endif

/**
 * \defgroup clk_group Clock Management
 */

/**
 * \ingroup clk_group
 * \defgroup sysclk_group System Clock Management
 *
 * See \ref sysclk_quickstart.
 *
 * The <em>sysclk</em> API covers the <em>system clock</em> and all
 * clocks derived from it. The system clock is a chip-internal clock on
 * which all <em>synchronous clocks</em>, i.e. CPU and bus/peripheral
 * clocks, are based. The system clock is typically generated from one
 * of a variety of sources, which may include crystal and RC oscillators
 * as well as PLLs.  The clocks derived from the system clock are
 * sometimes also known as <em>synchronous clocks</em>, since they
 * always run synchronously with respect to each other, as opposed to
 * <em>generic clocks</em> which may run from different oscillators or
 * PLLs.
 *
 * Most applications should simply call sysclk_init() to initialize
 * everything related to the system clock and its source (oscillator,
 * PLL or DFLL), and leave it at that. More advanced applications, and
 * platform-specific drivers, may require additional services from the
 * clock system, some of which may be platform-specific.
 *
 * \section sysclk_group_platform Platform Dependencies
 *
 * The sysclk API is partially chip- or platform-specific. While all
 * platforms provide mostly the same functionality, there are some
 * variations around how different bus types and clock tree structures
 * are handled.
 *
 * The following functions are available on all platforms with the same
 * parameters and functionality. These functions may be called freely by
 * portable applications, drivers and services:
 *   - sysclk_init()
 *   - sysclk_set_source()
 *   - sysclk_get_main_hz()
 *   - sysclk_get_cpu_hz()
 *   - sysclk_get_peripheral_bus_hz()
 *
 * The following functions are available on all platforms, but there may
 * be variations in the function signature (i.e. parameters) and
 * behavior. These functions are typically called by platform-specific
 * parts of drivers, and applications that aren't intended to be
 * portable:
 *   - sysclk_enable_peripheral_clock()
 *   - sysclk_disable_peripheral_clock()
 *   - sysclk_enable_module()
 *   - sysclk_disable_module()
 *   - sysclk_module_is_enabled()
 *   - sysclk_set_prescalers()
 *
 * All other functions should be considered platform-specific.
 * Enabling/disabling clocks to specific peripherals as well as
 * determining the speed of these clocks should be done by calling
 * functions provided by the driver for that peripheral.
 *
 * @{
 */

//! \name System Clock Initialization
//@{
/**
 * \fn void sysclk_init(void)
 * \brief Initialize the synchronous clock system.
 *
 * This function will initialize the system clock and its source. This
 * includes:
 *   - Mask all synchronous clocks except for any clocks which are
 *     essential for normal operation (for example internal memory
 *     clocks).
 *   - Set up the system clock prescalers as specified by the
 *     application's configuration file.
 *   - Enable the clock source specified by the application's
 *     configuration file (oscillator or PLL) and wait for it to become
 *     stable.
 *   - Set the main system clock source to the clock specified by the
 *     application's configuration file.
 *
 * Since all non-essential peripheral clocks are initially disabled, it
 * is the responsibility of the peripheral driver to re-enable any
 * clocks that are needed for normal operation.
 */
//@}

//! @}

#endif /* SYSCLK_H_INCLUDED */
