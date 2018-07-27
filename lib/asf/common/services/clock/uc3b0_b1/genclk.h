/**
 * \file
 *
 * \brief Chip-specific generic clock management
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
#ifndef CHIP_GENCLK_H_INCLUDED
#define CHIP_GENCLK_H_INCLUDED

#include <osc.h>
#include <pll.h>

/**
 * \weakgroup genclk_group
 * @{
 */

//! \name Chip-specific generic clock definitions
//@{

//! The max division factor applicable to the source clock of the generic clock
#define GENCLK_DIV_MAX    ((1 << AVR32_PM_GCCTRL_DIV_SIZE) * 2)

#ifndef __ASSEMBLY__

#include <stdint.h>
#include <avr32/io.h>

enum genclk_source {
	GENCLK_SRC_OSC0   = 0,    //!< Oscillator 0
	GENCLK_SRC_OSC1   = 1,    //!< Oscillator 1
	GENCLK_SRC_PLL0   = 2,    //!< PLL 0
	GENCLK_SRC_PLL1   = 3,    //!< PLL 1
};

//@}

struct genclk_config {
	uint32_t ctrl;
};

static inline void genclk_config_defaults(struct genclk_config *cfg,
		unsigned int id)
{
	cfg->ctrl = 0;
}

static inline void genclk_config_read(struct genclk_config *cfg,
		unsigned int id)
{
	cfg->ctrl = AVR32_PM.gcctrl[id];
}

static inline void genclk_config_write(const struct genclk_config *cfg,
		unsigned int id)
{
	AVR32_PM.gcctrl[id] = cfg->ctrl;
}

static inline void genclk_config_set_source(struct genclk_config *cfg,
		enum genclk_source src)
{
	uint32_t mask;

	mask = AVR32_PM_GCCTRL_OSCSEL_MASK | AVR32_PM_GCCTRL_PLLSEL_MASK;
	Assert(!(src & ~mask));

	cfg->ctrl = (cfg->ctrl & ~mask) | (src << AVR32_PM_GCCTRL_OSCSEL);
}

static inline void genclk_config_set_divider(struct genclk_config *cfg,
		unsigned int divider)
{
	Assert((divider > 0) && (divider <= GENCLK_DIV_MAX));

	/* Clear all the bits we're about to modify */
	cfg->ctrl &= ~(AVR32_PM_GCCTRL_DIVEN_MASK | AVR32_PM_GCCTRL_DIV_MASK);

	if (divider > 1) {
		cfg->ctrl |= 1U << AVR32_PM_GCCTRL_DIVEN;
		cfg->ctrl |= ((divider >> 1) - 1) << AVR32_PM_GCCTRL_DIV;
	}
}

static inline void genclk_enable(const struct genclk_config *cfg,
		unsigned int id)
{
	AVR32_PM.gcctrl[id] = cfg->ctrl | (1U << AVR32_PM_GCCTRL_CEN);
}

static inline void genclk_disable(unsigned int id)
{
	AVR32_PM.gcctrl[id] = 0;
}

static inline void genclk_enable_source(enum genclk_source src)
{
	switch (src) {
#ifdef BOARD_OSC0_HZ
	case GENCLK_SRC_OSC0:
		if (!osc_is_ready(OSC_ID_OSC0)) {
			osc_enable(OSC_ID_OSC0);
			osc_wait_ready(OSC_ID_OSC0);
		}
		break;
#endif

#ifdef BOARD_OSC1_HZ
	case GENCLK_SRC_OSC1:
		if (!osc_is_ready(OSC_ID_OSC1)) {
			osc_enable(OSC_ID_OSC1);
			osc_wait_ready(OSC_ID_OSC1);
		}
		break;
#endif

#ifdef CONFIG_PLL0_SOURCE
	case GENCLK_SRC_PLL0: {
		pll_enable_config_defaults(0);
		break;
	}
#endif

#ifdef CONFIG_PLL1_SOURCE
	case GENCLK_SRC_PLL1: {
		pll_enable_config_defaults(1);
		break;
	}
#endif

	default:
		Assert(false);
		break;
	}
}

#endif /* __ASSEMBLY__ */

//! @}

#endif /* CHIP_GENCLK_H_INCLUDED */
