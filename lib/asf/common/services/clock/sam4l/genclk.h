/**
 * \file
 *
 * \brief Chip-specific generic clock management
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
#ifndef CHIP_GENCLK_H_INCLUDED
#define CHIP_GENCLK_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

// dfll.h is not included to avoid a circular dependency.
extern void dfll_enable_config_defaults(uint32_t dfll_id);

/**
 * \weakgroup genclk_group
 * @{
 */

//! \name Chip-specific generic clock definitions
//@{

#define GENCLK_DIV_MAX          256

#ifndef __ASSEMBLY__

#include <compiler.h>
#include <osc.h>
#include <pll.h>

enum genclk_source {
	GENCLK_SRC_RCSYS        = 0,    //!< System RC oscillator
	GENCLK_SRC_OSC32K       = 1,    //!< 32 kHz oscillator
	GENCLK_SRC_DFLL         = 2,    //!< DFLL
	GENCLK_SRC_OSC0         = 3,    //!< Oscillator 0
	GENCLK_SRC_RC80M        = 4,    //!< 80 MHz RC oscillator
	GENCLK_SRC_RCFAST       = 5,    //!< 4-8-12 MHz RC oscillator
	GENCLK_SRC_RC1M         = 6,    //!< 1 MHz RC oscillator
	GENCLK_SRC_CLK_CPU      = 7,    //!< CPU clock
	GENCLK_SRC_CLK_HSB      = 8,    //!< High Speed Bus clock
	GENCLK_SRC_CLK_PBA      = 9,    //!< Peripheral Bus A clock
	GENCLK_SRC_CLK_PBB      = 10,   //!< Peripheral Bus B clock
	GENCLK_SRC_CLK_PBC      = 11,   //!< Peripheral Bus C clock
	GENCLK_SRC_CLK_PBD      = 12,   //!< Peripheral Bus D clock
	GENCLK_SRC_RC32K        = 13,   //!< 32 kHz RC oscillator
	GENCLK_SRC_CLK_1K       = 15,   //!< 1 kHz output from OSC32K
	GENCLK_SRC_PLL0         = 16,   //!< PLL0
	GENCLK_SRC_HRPCLK       = 17,   //!< High resolution prescaler
	GENCLK_SRC_FPCLK        = 18,   //!< Fractional prescaler
	GENCLK_SRC_GCLKIN0      = 19,   //!< GCLKIN0
	GENCLK_SRC_GCLKIN1      = 20,   //!< GCLKIN1
	GENCLK_SRC_GCLK11       = 21,   //!< GCLK11
};

//@}

struct genclk_config {
	uint32_t ctrl;
};

static inline void genclk_config_defaults(struct genclk_config *cfg,
		uint32_t id)
{
	UNUSED(id);
	cfg->ctrl = 0;
}

static inline void genclk_config_read(struct genclk_config *cfg,
		uint32_t id)
{
	cfg->ctrl = SCIF->SCIF_GCCTRL[id].SCIF_GCCTRL;
}

static inline void genclk_config_write(const struct genclk_config *cfg,
		uint32_t id)
{
	SCIF->SCIF_GCCTRL[id].SCIF_GCCTRL = cfg->ctrl;
}

static inline void genclk_config_set_source(struct genclk_config *cfg,
		enum genclk_source src)
{
	cfg->ctrl = (cfg->ctrl & ~SCIF_GCCTRL_OSCSEL_Msk)
			| SCIF_GCCTRL_OSCSEL(src);
}

static inline void genclk_config_set_divider(struct genclk_config *cfg,
		uint32_t divider)
{
	Assert(divider > 0 && divider <= GENCLK_DIV_MAX);

	/* Clear all the bits we're about to modify */
	cfg->ctrl &= ~(SCIF_GCCTRL_DIVEN
			| SCIF_GCCTRL_DIV_Msk);

	if (divider > 1) {
		cfg->ctrl |= SCIF_GCCTRL_DIVEN;
		cfg->ctrl |= SCIF_GCCTRL_DIV(((divider + 1) / 2) - 1);
	}
}

static inline void genclk_enable(const struct genclk_config *cfg,
		uint32_t id)
{
	 SCIF->SCIF_GCCTRL[id].SCIF_GCCTRL = cfg->ctrl | SCIF_GCCTRL_CEN;
}

static inline void genclk_disable(uint32_t id)
{
	SCIF->SCIF_GCCTRL[id].SCIF_GCCTRL = 0;
}

static inline void genclk_enable_source(enum genclk_source src)
{
	switch (src) {
	case GENCLK_SRC_RCSYS:
	case GENCLK_SRC_CLK_CPU:
	case GENCLK_SRC_CLK_HSB:
	case GENCLK_SRC_CLK_PBA:
	case GENCLK_SRC_CLK_PBB:
	case GENCLK_SRC_CLK_PBC:
	case GENCLK_SRC_CLK_PBD:
		// Nothing to do
		break;

#ifdef BOARD_OSC32_HZ
	case GENCLK_SRC_OSC32K:
	case GENCLK_SRC_CLK_1K: // The 1K linked on OSC32K
		if (!osc_is_ready(OSC_ID_OSC32)) {
			osc_enable(OSC_ID_OSC32);
			osc_wait_ready(OSC_ID_OSC32);
		}
		break;
#endif

	case GENCLK_SRC_RC80M:
		if (!osc_is_ready(OSC_ID_RC80M)) {
			osc_enable(OSC_ID_RC80M);
			osc_wait_ready(OSC_ID_RC80M);
		}
		break;

	case GENCLK_SRC_RCFAST:
		if (!osc_is_ready(OSC_ID_RCFAST)) {
			osc_enable(OSC_ID_RCFAST);
			osc_wait_ready(OSC_ID_RCFAST);
		}
		break;

	case GENCLK_SRC_RC1M:
		if (!osc_is_ready(OSC_ID_RC1M)) {
			osc_enable(OSC_ID_RC1M);
			osc_wait_ready(OSC_ID_RC1M);
		}
		break;

	case GENCLK_SRC_RC32K:
		if (!osc_is_ready(OSC_ID_RC32K)) {
			osc_enable(OSC_ID_RC32K);
			osc_wait_ready(OSC_ID_RC32K);
		}
		break;

#ifdef CONFIG_DFLL0_SOURCE
	case GENCLK_SRC_DFLL:
		dfll_enable_config_defaults(0);
		break;
#endif

#ifdef BOARD_OSC0_HZ
	case GENCLK_SRC_OSC0:
		if (!osc_is_ready(OSC_ID_OSC0)) {
			osc_enable(OSC_ID_OSC0);
			osc_wait_ready(OSC_ID_OSC0);
		}
		break;
#endif

# ifdef CONFIG_PLL0_SOURCE
	case GENCLK_SRC_PLL0: {
		pll_enable_config_defaults(0);
		break;
	}
# endif

	default:
		Assert(false);
		break;
	}
}

#endif /* __ASSEMBLY__ */

//! @}

#ifdef __cplusplus
}
#endif

#endif /* CHIP_GENCLK_H_INCLUDED */
