/**
 * \file
 *
 * \brief Chip-specific DFLL definitions
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
#ifndef CHIP_DFLL_H_INCLUDED
#define CHIP_DFLL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \weakgroup dfll_group
 * @{
 */

#define NR_DFLLS          1
#define DFLL_MIN_HZ       20000000UL
#define DFLL_MAX_HZ       150000000UL
#define DFLL_MIN_KHZ      (DFLL_MIN_HZ / 1000)
#define DFLL_MAX_KHZ      (DFLL_MAX_HZ / 1000)

#define DFLL_COARSE_MAX   (255)
#define DFLL_FINE_MAX     (511)
#define DFLL_FINE_HALF    (255)

#define DFLL_CALIB_VALUE  (0x0B)

#define DFLL_RANGE0       (0)
#define DFLL_RANGE1       (1)
#define DFLL_RANGE2       (2)
#define DFLL_RANGE3       (3)
#define DFLL_MAX_RANGE1   (110000000)
#define DFLL_MAX_RANGE2   (55000000)
#define DFLL_MAX_RANGE3   (30000000)

#ifndef __ASSEMBLY__

#include "compiler.h"
#include <genclk.h>
#include <osc.h>

typedef enum genclk_source dfll_refclk_t;

struct dfll_config {
	struct genclk_config    ref_cfg;        //!< Reference clock
	uint32_t                conf;           //!< DFLLnCONF
	uint32_t                mul;            //!< DFLLnMUL
	uint32_t                step;           //!< DFLLnSTEP
	uint32_t                ssg;            //!< DFLLnSSG
	uint32_t                val;            //!< DFLLnVAL
	uint8_t                 freq_range;     //!< Frequency Range
};

static inline void dfll_config_set_max_step(struct dfll_config *cfg,
		uint16_t coarse, uint16_t fine)
{
	cfg->step = (SCIF_DFLL0STEP_CSTEP(coarse)
			| SCIF_DFLL0STEP_FSTEP(fine));
}

static inline void dfll_priv_set_frequency_range(struct dfll_config *cfg,
		uint32_t freq)
{
	if (freq < DFLL_MAX_RANGE3){
		cfg->freq_range = DFLL_RANGE3;
	}
	else if (freq < DFLL_MAX_RANGE2){
		cfg->freq_range = DFLL_RANGE2;
	}
	else if (freq < DFLL_MAX_RANGE1){
		cfg->freq_range = DFLL_RANGE1;
	}
	else {
		cfg->freq_range = DFLL_RANGE0;
	}
	cfg->conf &= ~SCIF_DFLL0CONF_RANGE_Msk;
	cfg->conf |=SCIF_DFLL0CONF_RANGE(cfg->freq_range);
}

static inline void dfll_config_init_open_loop_mode(struct dfll_config *cfg)
{
	genclk_config_defaults(&cfg->ref_cfg, 0);
	// Do a sync before reading a dfll conf register
	SCIF->SCIF_DFLL0SYNC = SCIF_DFLL0SYNC_SYNC;
	while (!(SCIF->SCIF_PCLKSR & SCIF_PCLKSR_DFLL0RDY));
	cfg->conf = SCIF->SCIF_DFLL0CONF;
	// Select Open Loop Mode
	cfg->conf &= ~SCIF_DFLL0CONF_MODE;
	// Clear DFLL Frequency Range
	cfg->freq_range = 0;
	cfg->conf &= ~SCIF_DFLL0CONF_RANGE_Msk;
	cfg->conf |= SCIF_DFLL0CONF_RANGE(cfg->freq_range);

	cfg->mul = 0;
	cfg->step = 0;
	cfg->ssg = 0;
	cfg->val = 0;
}

#ifdef CONFIG_DFLL0_FREQ
static inline void dfll_config_init_closed_loop_mode(struct dfll_config *cfg,
		dfll_refclk_t refclk, uint16_t divide, uint16_t mul)
{
	/*
	 * Set up generic clock source with specified reference clock
	 * and divider.
	 */
	genclk_config_defaults(&cfg->ref_cfg, 0);
	genclk_config_set_source(&cfg->ref_cfg, refclk);
	genclk_config_set_divider(&cfg->ref_cfg, divide);

	// Do a sync before reading a dfll conf register
	SCIF->SCIF_DFLL0SYNC = SCIF_DFLL0SYNC_SYNC;
	while (!(SCIF->SCIF_PCLKSR & SCIF_PCLKSR_DFLL0RDY));
	cfg->conf = SCIF->SCIF_DFLL0CONF;
	// Select Closed Loop Mode
	cfg->conf |= SCIF_DFLL0CONF_MODE;
	// Write DFLL Frequency Range
	dfll_priv_set_frequency_range(cfg, CONFIG_DFLL0_FREQ);

	cfg->mul = mul;
	cfg->val = 0;
	/*
	 * Initial step length of 4. If this is set too high, the DFLL
	 * may fail to lock.
	 */
	dfll_config_set_max_step(cfg, 4, 4);
	cfg->ssg = 0;
}
#endif

static inline uint32_t dfll_priv_get_source_hz(dfll_refclk_t src)
{
	/*
	 * Only handle the cases that actually make sense as a DFLL
	 * source. The DFLL itself is obviously not one of those cases.
	 */
	switch (src) {
	case GENCLK_SRC_RCSYS:
		return OSC_RCSYS_NOMINAL_HZ;

#ifdef BOARD_OSC32_HZ
	case GENCLK_SRC_OSC32K:
		return BOARD_OSC32_HZ;
#endif

#ifdef BOARD_OSC0_HZ
	case GENCLK_SRC_OSC0:
		return BOARD_OSC0_HZ;
#endif

	case GENCLK_SRC_RC80M:
		return OSC_RC80M_NOMINAL_HZ;

	case GENCLK_SRC_RC32K:
		return OSC_RC32K_NOMINAL_HZ;

	default:
		/* unhandled_case(src) */
		return 0;
	}
}

#define dfll_config_defaults(cfg, dfll_id)                                     \
	dfll_config_init_closed_loop_mode(cfg,                                 \
			CONFIG_DFLL##dfll_id##_SOURCE,                         \
			CONFIG_DFLL##dfll_id##_DIV,                            \
			CONFIG_DFLL##dfll_id##_MUL)

#define dfll_get_default_rate(dfll_id)                                         \
	((dfll_priv_get_source_hz(CONFIG_DFLL##dfll_id##_SOURCE)               \
			* CONFIG_DFLL##dfll_id##_MUL)                          \
		/ CONFIG_DFLL##dfll_id##_DIV)

static inline void dfll_config_set_initial_tuning(struct dfll_config *cfg,
		uint16_t coarse, uint16_t fine)
{
	cfg->val = (SCIF_DFLL0VAL_COARSE(coarse)
			| SCIF_DFLL0VAL_FINE(fine));
}

/**
 * \brief Tune the DFLL configuration for a specific target frequency
 *
 * This will set the initial coarse and fine DFLL tuning to match the
 * given target frequency. In open loop mode, this will cause the DFLL
 * to run close to the specified frequency, though it may not match
 * exactly. In closed loop mode, the DFLL will automatically tune itself
 * to the target frequency regardless of the initial tuning, but this
 * function may be used to set a starting point close to the desired
 * frequency in order to reduce the startup time.
 *
 * \par Calculating the DFLL frequency
 *
 * \f{eqnarray*}{
	f_{DFLL} &=& \left[f_{min} + \left(f_{max} - f_{min}\right)
		\frac{\mathrm{COARSE}}{\mathrm{COARSE}_{max}}\right]
		\left(1 + x \frac{\mathrm{FINE}
			- \mathrm{FINE}_{half}}{\mathrm{FINE}_{max}}\right)
		= f_{coarse} \left(1 + x
		\frac{\mathrm{FINE}
			- \mathrm{FINE}_{half}}{\mathrm{FINE}_{max}}\right) \\
	\mathrm{COARSE} &=& \frac{\left(f_{DFLL} - f_{min}\right)}
		{f_{max} - f_{min}} \mathrm{COARSE}_{max} \\
	f_{coarse} &=& f_{min} + \left(f_{max} - f_{min}\right)
		\frac{\mathrm{COARSE}}{\mathrm{COARSE}_{max}} \\
	\mathrm{FINE} &=& \left(10 \frac{f_{DFLL} - f_{coarse}}
		{f_{coarse}} + \mathrm{FINE}_{half}\right) / 4
   \f}
 *
 * \param cfg The DFLL configuration to be tuned.
 * \param target_hz Target frequency in Hz.
 */
static inline void dfll_config_tune_for_target_hz(struct dfll_config *cfg,
		uint32_t target_hz)
{
	uint32_t target_khz;
	uint32_t coarse_khz;
	uint32_t delta_khz;
	uint32_t coarse;
	uint32_t fine;

	target_khz = target_hz / 1000;
	coarse = ((target_khz - DFLL_MIN_KHZ) * DFLL_COARSE_MAX)
			/ (DFLL_MAX_KHZ - DFLL_MIN_KHZ);
	coarse_khz = DFLL_MIN_KHZ + (((DFLL_MAX_KHZ - DFLL_MIN_KHZ)
			/ DFLL_COARSE_MAX) * coarse);
	delta_khz = target_khz - coarse_khz;
	fine = (((delta_khz * DFLL_FINE_MAX) * 2) / coarse_khz) * 5;
	fine += DFLL_FINE_HALF;
	fine /= 4;

	dfll_config_set_initial_tuning(cfg, coarse, fine);
	dfll_priv_set_frequency_range(cfg, target_hz);
}

static inline void dfll_config_enable_ssg(struct dfll_config *cfg,
		uint16_t amplitude, uint16_t step_size)
{
	cfg->ssg = (SCIF_DFLL0SSG_EN
			| SCIF_DFLL0SSG_AMPLITUDE(amplitude)
			| SCIF_DFLL0SSG_STEPSIZE(step_size));
}

static inline void dfll_config_disable_ssg(struct dfll_config *cfg)
{
	cfg->ssg = 0;
}

extern void dfll_enable_open_loop(const struct dfll_config *cfg,
		uint32_t dfll_id);
extern void dfll_disable_open_loop(uint32_t dfll_id);
extern void dfll_enable_closed_loop(const struct dfll_config *cfg,
		uint32_t dfll_id);
extern void dfll_disable_closed_loop(uint32_t dfll_id);
#ifndef CHIP_GENCLK_H_INCLUDED
// This function already has a prototype in genclk.h.
extern void dfll_enable_config_defaults(uint32_t dfll_id);
#endif

static inline bool dfll_is_coarse_locked(uint32_t dfll_id)
{
	UNUSED(dfll_id);
	return !!(SCIF->SCIF_PCLKSR & SCIF_PCLKSR_DFLL0LOCKC);
}

static inline bool dfll_is_fine_locked(uint32_t dfll_id)
{
	UNUSED(dfll_id);
	return !!(SCIF->SCIF_PCLKSR & SCIF_PCLKSR_DFLL0LOCKF);
}

static inline bool dfll_is_accurate_locked(uint32_t dfll_id)
{
	UNUSED(dfll_id);

	return (dfll_is_coarse_locked(dfll_id) &&
			dfll_is_fine_locked(dfll_id));
}

static inline void dfll_enable_source(dfll_refclk_t src)
{
	switch (src) {
	case GENCLK_SRC_RCSYS:
		/* Nothing to do */
		break;

#ifdef BOARD_OSC32_HZ
	case GENCLK_SRC_OSC32K:
		if (!osc_is_ready(OSC_ID_OSC32)) {
			osc_enable(OSC_ID_OSC32);
			osc_wait_ready(OSC_ID_OSC32);
		}
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

	case GENCLK_SRC_RC80M:
		if (!osc_is_ready(OSC_ID_RC80M)) {
			osc_enable(OSC_ID_RC80M);
			osc_wait_ready(OSC_ID_RC80M);
		}
		break;

	case GENCLK_SRC_RC32K:
		if (!osc_is_ready(OSC_ID_RC32K)) {
			osc_enable(OSC_ID_RC32K);
			osc_wait_ready(OSC_ID_RC32K);
		}
		break;

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

#endif /* CHIP_DFLL_H_INCLUDED */
