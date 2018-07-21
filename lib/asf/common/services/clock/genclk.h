/**
 * \file
 *
 * \brief Generic clock management
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
#ifndef CLK_GENCLK_H_INCLUDED
#define CLK_GENCLK_H_INCLUDED

#include "parts.h"

#if SAM3S
# include "sam3s/genclk.h"
#elif SAM3U
# include "sam3u/genclk.h"
#elif SAM3N
# include "sam3n/genclk.h"
#elif SAM3XA
# include "sam3x/genclk.h"
#elif SAM4S
# include "sam4s/genclk.h"
#elif SAM4L
# include "sam4l/genclk.h"
#elif SAM4E
# include "sam4e/genclk.h"
#elif SAM4N
# include "sam4n/genclk.h"
#elif SAM4C
# include "sam4c/genclk.h"
#elif SAM4CM
# include "sam4cm/genclk.h"
#elif SAM4CP
# include "sam4cp/genclk.h"
#elif SAMG
# include "samg/genclk.h"
#elif SAMV71
# include "samv71/genclk.h"
#elif SAMV70
# include "samv70/genclk.h"
#elif SAME70
# include "same70/genclk.h"
#elif SAMS70
# include "sams70/genclk.h"
#elif (UC3A0 || UC3A1)
# include "uc3a0_a1/genclk.h"
#elif UC3A3
# include "uc3a3_a4/genclk.h"
#elif UC3B
# include "uc3b0_b1/genclk.h"
#elif UC3C
# include "uc3c/genclk.h"
#elif UC3D
# include "uc3d/genclk.h"
#elif UC3L
# include "uc3l/genclk.h"
#else
# error Unsupported chip type
#endif

/**
 * \ingroup clk_group
 * \defgroup genclk_group Generic Clock Management
 *
 * Generic clocks are configurable clocks which run outside the system
 * clock domain. They are often connected to peripherals which have an
 * asynchronous component running independently of the bus clock, e.g.
 * USB controllers, low-power timers and RTCs, etc.
 *
 * Note that not all platforms have support for generic clocks; on such
 * platforms, this API will not be available.
 *
 * @{
 */

/**
 * \def GENCLK_DIV_MAX
 * \brief Maximum divider supported by the generic clock implementation
 */
/**
 * \enum genclk_source
 * \brief Generic clock source ID
 *
 * Each generic clock may be generated from a different clock source.
 * These are the available alternatives provided by the chip.
 */

//! \name Generic clock configuration
//@{
/**
 * \struct genclk_config
 * \brief Hardware representation of a set of generic clock parameters
 */
/**
 * \fn void genclk_config_defaults(struct genclk_config *cfg,
 *              unsigned int id)
 * \brief Initialize \a cfg to the default configuration for the clock
 * identified by \a id.
 */
/**
 * \fn void genclk_config_read(struct genclk_config *cfg, unsigned int id)
 * \brief Read the currently active configuration of the clock
 * identified by \a id into \a cfg.
 */
/**
 * \fn void genclk_config_write(const struct genclk_config *cfg,
 *              unsigned int id)
 * \brief Activate the configuration \a cfg on the clock identified by
 * \a id.
 */
/**
 * \fn void genclk_config_set_source(struct genclk_config *cfg,
 *              enum genclk_source src)
 * \brief Select a new source clock \a src in configuration \a cfg.
 */
/**
 * \fn void genclk_config_set_divider(struct genclk_config *cfg,
 *              unsigned int divider)
 * \brief Set a new \a divider in configuration \a cfg.
 */
/**
 * \fn void genclk_enable_source(enum genclk_source src)
 * \brief Enable the source clock \a src used by a generic clock.
 */
 //@}

//! \name Enabling and disabling Generic Clocks
//@{
/**
 * \fn void genclk_enable(const struct genclk_config *cfg, unsigned int id)
 * \brief Activate the configuration \a cfg on the clock identified by
 * \a id and enable it.
 */
/**
 * \fn void genclk_disable(unsigned int id)
 * \brief Disable the generic clock identified by \a id.
 */
//@}

/**
 * \brief Enable the configuration defined by \a src and \a divider
 * for the generic clock identified by \a id.
 *
 * \param id      The ID of the generic clock.
 * \param src     The source clock of the generic clock.
 * \param divider The divider used to generate the generic clock.
 */
static inline void genclk_enable_config(unsigned int id, enum genclk_source src, unsigned int divider)
{
	struct genclk_config gcfg;

	genclk_config_defaults(&gcfg, id);
	genclk_enable_source(src);
	genclk_config_set_source(&gcfg, src);
	genclk_config_set_divider(&gcfg, divider);
	genclk_enable(&gcfg, id);
}

//! @}

#endif /* CLK_GENCLK_H_INCLUDED */
