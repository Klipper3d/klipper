/**
 * \file
 *
 * \brief Power Management Controller (PMC) driver for SAM.
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

#include "pmc.h"

#if (SAM3N)
# define MAX_PERIPH_ID    31
#elif (SAM3XA)
# define MAX_PERIPH_ID    44
#elif (SAM3U)
# define MAX_PERIPH_ID    29
#elif (SAM3S || SAM4S)
# define MAX_PERIPH_ID    34
#elif (SAM4E)
# define MAX_PERIPH_ID    47
#elif (SAMV71)
# define MAX_PERIPH_ID    63
#elif (SAMV70)
# define MAX_PERIPH_ID    63
#elif (SAME70)
# define MAX_PERIPH_ID    63
#elif (SAMS70)
# define MAX_PERIPH_ID    63
#elif (SAM4N)
# define MAX_PERIPH_ID    31
#elif (SAM4C || SAM4CM || SAM4CP)
# define MAX_PERIPH_ID    43
#elif (SAMG51)
# define MAX_PERIPH_ID    47
#elif (SAMG53)
# define MAX_PERIPH_ID    47
#elif (SAMG54)
# define MAX_PERIPH_ID    47
#elif (SAMG55)
# define MAX_PERIPH_ID    50
#endif

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_pmc_group Power Management Controller (PMC)
 *
 * \par Purpose
 *
 * The Power Management Controller (PMC) optimizes power consumption by
 * controlling all system and user peripheral clocks. The PMC enables/disables
 * the clock inputs to many of the peripherals and the Cortex-M Processor.
 *
 * @{
 */

/**
 * \brief Set the prescaler of the MCK.
 *
 * \param ul_pres Prescaler value.
 */
void pmc_mck_set_prescaler(uint32_t ul_pres)
{
	PMC->PMC_MCKR =
			(PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | ul_pres;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}

#if SAMV71 || SAMV70 || SAME70 || SAMS70
/**
 * \brief Set the division of the MCK.
 *
 * \param ul_div Division value.
 */
void pmc_mck_set_division(uint32_t ul_div)
{
	switch (ul_div) {
		case 1:
			ul_div = PMC_MCKR_MDIV_EQ_PCK;
			break;
		case 2:
			ul_div = PMC_MCKR_MDIV_PCK_DIV2;
			break;
		case 3:
			ul_div = PMC_MCKR_MDIV_PCK_DIV3;
			break;
		case 4:
			ul_div = PMC_MCKR_MDIV_PCK_DIV4;
			break;
		default:
			ul_div = PMC_MCKR_MDIV_EQ_PCK;
			break;
	}
	PMC->PMC_MCKR =
			(PMC->PMC_MCKR & (~PMC_MCKR_MDIV_Msk)) | ul_div;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}
#endif

/**
 * \brief Set the source of the MCK.
 *
 * \param ul_source Source selection value.
 */
void pmc_mck_set_source(uint32_t ul_source)
{
	PMC->PMC_MCKR =
			(PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) | ul_source;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}

/**
 * \brief Switch master clock source selection to slow clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_sclk(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) |
			PMC_MCKR_CSS_SLOW_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch master clock source selection to main clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_mainck(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) |
			PMC_MCKR_CSS_MAIN_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch master clock source selection to PLLA clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_pllack(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) |
			PMC_MCKR_CSS_PLLA_CLK;

	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

#if (SAM3S || SAM4S || SAM4C || SAM4CM || SAM4CP || SAMG55)
/**
 * \brief Switch master clock source selection to PLLB clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_pllbck(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) |
			PMC_MCKR_CSS_PLLB_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}
#endif

#if (SAM3XA || SAM3U || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Switch master clock source selection to UPLL clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_upllck(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) |
			PMC_MCKR_CSS_UPLL_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}
#endif

/**
 * \brief Switch slow clock source selection to external 32k (Xtal or Bypass).
 *
 * \note Switching SCLK back to 32krc is only possible by shutting down the
 *       VDDIO power supply.
 *
 * \param ul_bypass 0 for Xtal, 1 for bypass.
 */
void pmc_switch_sclk_to_32kxtal(uint32_t ul_bypass)
{
	/* Set Bypass mode if required */
	if (ul_bypass == 1) {
		SUPC->SUPC_MR |= SUPC_MR_KEY_PASSWD |
			SUPC_MR_OSCBYPASS;
	}

	SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_XTALSEL;
}

/**
 * \brief Check if the external 32k Xtal is ready.
 *
 * \retval 1 External 32k Xtal is ready.
 * \retval 0 External 32k Xtal is not ready.
 */
uint32_t pmc_osc_is_ready_32kxtal(void)
{
	return ((SUPC->SUPC_SR & SUPC_SR_OSCSEL)
			&& (PMC->PMC_SR & PMC_SR_OSCSELS));
}

/**
 * \brief Switch main clock source selection to internal fast RC.
 *
 * \param ul_moscrcf Fast RC oscillator(4/8/12Mhz).
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 * \retval 2 Invalid frequency.
 */
void pmc_switch_mainck_to_fastrc(uint32_t ul_moscrcf)
{
	/* Enable Fast RC oscillator but DO NOT switch to RC now */
	PMC->CKGR_MOR |= (CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN);

	/* Wait the Fast RC to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

	/* Change Fast RC oscillator frequency */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCF_Msk) |
			CKGR_MOR_KEY_PASSWD | ul_moscrcf;

	/* Wait the Fast RC to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

	/* Switch to Fast RC */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCSEL) |
			CKGR_MOR_KEY_PASSWD;
}

/**
 * \brief Enable fast RC oscillator.
 *
 * \param ul_rc Fast RC oscillator(4/8/12Mhz).
 */
void pmc_osc_enable_fastrc(uint32_t ul_rc)
{
	/* Enable Fast RC oscillator but DO NOT switch to RC */
	PMC->CKGR_MOR |= (CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN);
	/* Wait the Fast RC to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

	/* Change Fast RC oscillator frequency */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCF_Msk) |
			CKGR_MOR_KEY_PASSWD | ul_rc;
	/* Wait the Fast RC to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));
}

/**
 * \brief Disable the internal fast RC.
 */
void pmc_osc_disable_fastrc(void)
{
	/* Disable Fast RC oscillator */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCEN &
					~CKGR_MOR_MOSCRCF_Msk)
				| CKGR_MOR_KEY_PASSWD;
}

/**
 * \brief Check if the main fastrc is ready.
 *
 * \retval 0 Xtal is not ready, otherwise ready.
 */
uint32_t pmc_osc_is_ready_fastrc(void)
{
	return (PMC->PMC_SR & PMC_SR_MOSCRCS);
}

/**
 * \brief Enable main XTAL oscillator.
 *
 * \param ul_xtal_startup_time Xtal start-up time, in number of slow clocks.
 */
void pmc_osc_enable_main_xtal(uint32_t ul_xtal_startup_time)
{
	uint32_t mor = PMC->CKGR_MOR;
	mor &= ~(CKGR_MOR_MOSCXTBY|CKGR_MOR_MOSCXTEN);
	mor |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN |
			CKGR_MOR_MOSCXTST(ul_xtal_startup_time);
	PMC->CKGR_MOR = mor;
	/* Wait the main Xtal to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));
}

/**
 * \brief Bypass main XTAL.
 */
void pmc_osc_bypass_main_xtal(void)
{
	uint32_t mor = PMC->CKGR_MOR;
	mor &= ~(CKGR_MOR_MOSCXTBY|CKGR_MOR_MOSCXTEN);
	mor |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTBY;
	/* Enable Crystal oscillator but DO NOT switch now. Keep MOSCSEL to 0 */
	PMC->CKGR_MOR = mor;
	/* The MOSCXTS in PMC_SR is automatically set */
}

/**
 * \brief Disable the main Xtal.
 */
void pmc_osc_disable_main_xtal(void)
{
	uint32_t mor = PMC->CKGR_MOR;
	mor &= ~(CKGR_MOR_MOSCXTBY|CKGR_MOR_MOSCXTEN);
	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | mor;
}

/**
 * \brief Check if the main crystal is bypassed.
 *
 * \retval 0 Xtal is bypassed, otherwise not.
 */
uint32_t pmc_osc_is_bypassed_main_xtal(void)
{
	return (PMC->CKGR_MOR & CKGR_MOR_MOSCXTBY);
}

/**
 * \brief Check if the main crystal is ready.
 *
 * \note If main crystal is bypassed, it's always ready.
 *
 * \retval 0 main crystal is not ready, otherwise ready.
 */
uint32_t pmc_osc_is_ready_main_xtal(void)
{
	return (PMC->PMC_SR & PMC_SR_MOSCXTS);
}

/**
 * \brief Switch main clock source selection to external Xtal/Bypass.
 *
 * \note The function may switch MCK to SCLK if MCK source is MAINCK to avoid
 *       any system crash.
 *
 * \note If used in Xtal mode, the Xtal is automatically enabled.
 *
 * \param ul_bypass 0 for Xtal, 1 for bypass.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
void pmc_switch_mainck_to_xtal(uint32_t ul_bypass,
		uint32_t ul_xtal_startup_time)
{
	/* Enable Main Xtal oscillator */
	if (ul_bypass) {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTEN) |
				CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTBY |
				CKGR_MOR_MOSCSEL;
	} else {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTBY) |
				CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN |
				CKGR_MOR_MOSCXTST(ul_xtal_startup_time);
		/* Wait the Xtal to stabilize */
		while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));

		PMC->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL;
	}
}

/**
 * \brief Disable the external Xtal.
 *
 * \param ul_bypass 0 for Xtal, 1 for bypass.
 */
void pmc_osc_disable_xtal(uint32_t ul_bypass)
{
	/* Disable xtal oscillator */
	if (ul_bypass) {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTBY) |
				CKGR_MOR_KEY_PASSWD;
	} else {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTEN) |
				CKGR_MOR_KEY_PASSWD;
	}
}

/**
 * \brief Check if the MAINCK is ready. Depending on MOSCEL, MAINCK can be one
 * of Xtal, bypass or internal RC.
 *
 * \retval 1 Xtal is ready.
 * \retval 0 Xtal is not ready.
 */
uint32_t pmc_osc_is_ready_mainck(void)
{
	return PMC->PMC_SR & PMC_SR_MOSCSELS;
}

/**
 * \brief Select Main Crystal or internal RC as main clock source.
 *
 * \note This function will not enable/disable RC or Main Crystal.
 *
 * \param ul_xtal_rc 0 internal RC is selected, otherwise Main Crystal.
 */
void pmc_mainck_osc_select(uint32_t ul_xtal_rc)
{
	uint32_t mor = PMC->CKGR_MOR;
	if (ul_xtal_rc) {
		mor |=  CKGR_MOR_MOSCSEL;
	} else {
		mor &= ~CKGR_MOR_MOSCSEL;
	}
	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | mor;
}

/**
 * \brief Enable PLLA clock.
 *
 * \param mula PLLA multiplier.
 * \param pllacount PLLA counter.
 * \param diva Divider.
 */
void pmc_enable_pllack(uint32_t mula, uint32_t pllacount, uint32_t diva)
{
	/* first disable the PLL to unlock the lock */
	pmc_disable_pllack();

#if (SAM4C || SAM4CM || SAM4CP || SAMG)
	PMC->CKGR_PLLAR = CKGR_PLLAR_PLLAEN(diva) |
			CKGR_PLLAR_PLLACOUNT(pllacount) | CKGR_PLLAR_MULA(mula);
#else
	PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_DIVA(diva) |
			CKGR_PLLAR_PLLACOUNT(pllacount) | CKGR_PLLAR_MULA(mula);
#endif
	while ((PMC->PMC_SR & PMC_SR_LOCKA) == 0);
}

/**
 * \brief Disable PLLA clock.
 */
void pmc_disable_pllack(void)
{
#if (SAM4C || SAM4CM || SAM4CP || SAMG)
	PMC->CKGR_PLLAR = CKGR_PLLAR_MULA(0);
#else
	PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0);
#endif
}

/**
 * \brief Is PLLA locked?
 *
 * \retval 0 Not locked.
 * \retval 1 Locked.
 */
uint32_t pmc_is_locked_pllack(void)
{
	return (PMC->PMC_SR & PMC_SR_LOCKA);
}

#if (SAM3S || SAM4S || SAM4C || SAM4CM || SAM4CP || SAMG55)
/**
 * \brief Enable PLLB clock.
 *
 * \param mulb PLLB multiplier.
 * \param pllbcount PLLB counter.
 * \param divb Divider.
 */
void pmc_enable_pllbck(uint32_t mulb, uint32_t pllbcount, uint32_t divb)
{
	/* first disable the PLL to unlock the lock */
	pmc_disable_pllbck();

#if SAMG55
	PMC->CKGR_PLLAR = CKGR_PLLAR_PLLAEN(divb) |
		CKGR_PLLAR_PLLACOUNT(pllbcount) | CKGR_PLLAR_MULA(mulb);
#else
	PMC->CKGR_PLLBR =
			CKGR_PLLBR_DIVB(divb) | CKGR_PLLBR_PLLBCOUNT(pllbcount)
			| CKGR_PLLBR_MULB(mulb);
#endif
	while ((PMC->PMC_SR & PMC_SR_LOCKB) == 0);
}

/**
 * \brief Disable PLLB clock.
 */
void pmc_disable_pllbck(void)
{
	PMC->CKGR_PLLBR = CKGR_PLLBR_MULB(0);
}

/**
 * \brief Is PLLB locked?
 *
 * \retval 0 Not locked.
 * \retval 1 Locked.
 */
uint32_t pmc_is_locked_pllbck(void)
{
	return (PMC->PMC_SR & PMC_SR_LOCKB);
}
#endif

#if (SAM3XA || SAM3U || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Enable UPLL clock.
 */
void pmc_enable_upll_clock(void)
{
	PMC->CKGR_UCKR = CKGR_UCKR_UPLLCOUNT(3) | CKGR_UCKR_UPLLEN;

	/* Wait UTMI PLL Lock Status */
	while (!(PMC->PMC_SR & PMC_SR_LOCKU));
}

/**
 * \brief Disable UPLL clock.
 */
void pmc_disable_upll_clock(void)
{
	PMC->CKGR_UCKR &= ~CKGR_UCKR_UPLLEN;
}

/**
 * \brief Is UPLL locked?
 *
 * \retval 0 Not locked.
 * \retval 1 Locked.
 */
uint32_t pmc_is_locked_upll(void)
{
	return (PMC->PMC_SR & PMC_SR_LOCKU);
}
#endif

/**
 * \brief Enable the specified peripheral clock.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_enable_periph_clk(uint32_t ul_id)
{
#if defined(REG_PMC_PCR) && !SAMG55
	uint32_t pcr;
	PMC->PMC_PCR = ul_id & 0x7F;
	pcr = PMC->PMC_PCR | PMC_PCR_EN | PMC_PCR_CMD;
	PMC->PMC_PCR = pcr;
	return 0;
#else
	if (ul_id > MAX_PERIPH_ID) {
		return 1;
	}

	if (ul_id < 32) {
		if ((PMC->PMC_PCSR0 & (1u << ul_id)) != (1u << ul_id)) {
			PMC->PMC_PCER0 = 1 << ul_id;
		}
#if (SAM3S || SAM3XA || SAM4S || SAM4E || SAM4C || SAM4CM || SAM4CP || SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
	} else {
		ul_id -= 32;
		if ((PMC->PMC_PCSR1 & (1u << ul_id)) != (1u << ul_id)) {
			PMC->PMC_PCER1 = 1 << ul_id;
		}
#endif
	}

	return 0;
#endif /* defined(REG_PMC_PCR) && !SAMG55 */
}

/**
 * \brief Disable the specified peripheral clock.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_disable_periph_clk(uint32_t ul_id)
{
#if defined(REG_PMC_PCR) && !SAMG55
	uint32_t pcr;
	PMC->PMC_PCR = ul_id & 0x7F;
	pcr = PMC->PMC_PCR | PMC_PCR_CMD;
	PMC->PMC_PCR = pcr;
	return 0;
#else
	if (ul_id > MAX_PERIPH_ID) {
		return 1;
	}

	if (ul_id < 32) {
		if ((PMC->PMC_PCSR0 & (1u << ul_id)) == (1u << ul_id)) {
			PMC->PMC_PCDR0 = 1 << ul_id;
		}
#if (SAM3S || SAM3XA || SAM4S || SAM4E || SAM4C || SAM4CM || SAM4CP || SAMG55 || SAMV71 \
		|| SAMV70 || SAME70 || SAMS70)
	} else {
		ul_id -= 32;
		if ((PMC->PMC_PCSR1 & (1u << ul_id)) == (1u << ul_id)) {
			PMC->PMC_PCDR1 = 1 << ul_id;
		}
#endif
	}
	return 0;
#endif /* defined(REG_PMC_PCR) && !SAMG55 */
}

/**
 * \brief Enable all peripheral clocks.
 */
void pmc_enable_all_periph_clk(void)
{
	PMC->PMC_PCER0 = PMC_MASK_STATUS0;
	while ((PMC->PMC_PCSR0 & PMC_MASK_STATUS0) != PMC_MASK_STATUS0);

#if (SAM3S || SAM3XA || SAM4S || SAM4E || SAM4C || SAM4CM || SAM4CP || SAMV71  \
		|| SAMV70 || SAME70 || SAMS70)
	PMC->PMC_PCER1 = PMC_MASK_STATUS1;
	while ((PMC->PMC_PCSR1 & PMC_MASK_STATUS1) != PMC_MASK_STATUS1);
#endif

#if defined(REG_PMC_PCR) && !SAMG55
	for (uint32_t id = 64; id <= 0x7F; id ++) {
		pmc_enable_periph_clk(id);
	}
#endif
}

/**
 * \brief Disable all peripheral clocks.
 */
void pmc_disable_all_periph_clk(void)
{
	PMC->PMC_PCDR0 = PMC_MASK_STATUS0;
	while ((PMC->PMC_PCSR0 & PMC_MASK_STATUS0) != 0);

#if (SAM3S || SAM3XA || SAM4S || SAM4E || SAM4C || SAM4CM || SAM4CP || SAMV71 \
		|| SAMV70 || SAME70 || SAMS70)
	PMC->PMC_PCDR1 = PMC_MASK_STATUS1;
	while ((PMC->PMC_PCSR1 & PMC_MASK_STATUS1) != 0);
#endif

#if defined(REG_PMC_PCR) && !SAMG55
	for (uint32_t id = 64; id <= 0x7F; id ++) {
		pmc_disable_periph_clk(id);
	}
#endif
}

/**
 * \brief Check if the specified peripheral clock is enabled.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Peripheral clock is disabled or unknown.
 * \retval 1 Peripheral clock is enabled.
 */
uint32_t pmc_is_periph_clk_enabled(uint32_t ul_id)
{
#if defined(REG_PMC_PCR) && !SAMG55
	PMC->PMC_PCR = ul_id & 0x7F;
	return (PMC->PMC_PCR & PMC_PCR_EN) ? 1 : 0;
#else
	if (ul_id > MAX_PERIPH_ID) {
		return 0;
	}

#if (SAM3S || SAM3XA || SAM4S || SAM4E || SAM4C || SAM4CM || SAM4CP || SAMV71 \
		|| SAMV70 || SAME70 || SAMS70)
	if (ul_id < 32) {
#endif
		if ((PMC->PMC_PCSR0 & (1u << ul_id))) {
			return 1;
		} else {
			return 0;
		}
#if (SAM3S || SAM3XA || SAM4S || SAM4E || SAM4C || SAM4CM || SAM4CP || SAMV71 \
		|| SAMV70 || SAME70 || SAMS70)
	} else {
		ul_id -= 32;
		if ((PMC->PMC_PCSR1 & (1u << ul_id))) {
			return 1;
		} else {
			return 0;
		}
	}
#endif
#endif /* defined(REG_PMC_PCR) && !SAMG55 */
}

/**
 * \brief Set the prescaler for the specified programmable clock.
 *
 * \param ul_id Peripheral ID.
 * \param ul_pres Prescaler value.
 */
void pmc_pck_set_prescaler(uint32_t ul_id, uint32_t ul_pres)
{
	PMC->PMC_PCK[ul_id] =
			(PMC->PMC_PCK[ul_id] & ~PMC_PCK_PRES_Msk) | ul_pres;
	while ((PMC->PMC_SCER & (PMC_SCER_PCK0 << ul_id))
			&& !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)));
}

/**
 * \brief Set the source oscillator for the specified programmable clock.
 *
 * \param ul_id Peripheral ID.
 * \param ul_source Source selection value.
 */
void pmc_pck_set_source(uint32_t ul_id, uint32_t ul_source)
{
	PMC->PMC_PCK[ul_id] =
			(PMC->PMC_PCK[ul_id] & ~PMC_PCK_CSS_Msk) | ul_source;
	while ((PMC->PMC_SCER & (PMC_SCER_PCK0 << ul_id))
			&& !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)));
}

/**
 * \brief Switch programmable clock source selection to slow clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_sclk(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = PMC_PCK_CSS_SLOW_CLK | ul_pres;
	for (ul_timeout = PMC_TIMEOUT;
	!(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to main clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_mainck(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = PMC_PCK_CSS_MAIN_CLK | ul_pres;
	for (ul_timeout = PMC_TIMEOUT;
	!(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to PLLA clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_pllack(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = PMC_PCK_CSS_PLLA_CLK | ul_pres;
	for (ul_timeout = PMC_TIMEOUT;
	!(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

#if (SAM3S || SAM4S || SAM4C || SAM4CM || SAM4CP || SAMG55)
/**
 * \brief Switch programmable clock source selection to PLLB clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_pllbck(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = PMC_PCK_CSS_PLLB_CLK | ul_pres;
	for (ul_timeout = PMC_TIMEOUT;
			!(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id));
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}
#endif

#if (SAM3XA || SAM3U || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Switch programmable clock source selection to UPLL clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_upllck(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = PMC_PCK_CSS_UPLL_CLK | ul_pres;
	for (ul_timeout = PMC_TIMEOUT;
			!(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id));
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}
#endif

/**
 * \brief Switch programmable clock source selection to mck.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_mck(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = PMC_PCK_CSS_MCK | ul_pres;
	for (ul_timeout = PMC_TIMEOUT;
	!(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Enable the specified programmable clock.
 *
 * \param ul_id Id of the programmable clock.
 */
void pmc_enable_pck(uint32_t ul_id)
{
	PMC->PMC_SCER = PMC_SCER_PCK0 << ul_id;
}

/**
 * \brief Disable the specified programmable clock.
 *
 * \param ul_id Id of the programmable clock.
 */
void pmc_disable_pck(uint32_t ul_id)
{
	PMC->PMC_SCDR = PMC_SCER_PCK0 << ul_id;
}

/**
 * \brief Enable all programmable clocks.
 */
void pmc_enable_all_pck(void)
{
	PMC->PMC_SCER = PMC_SCER_PCK0 | PMC_SCER_PCK1 | PMC_SCER_PCK2;
}

/**
 * \brief Disable all programmable clocks.
 */
void pmc_disable_all_pck(void)
{
	PMC->PMC_SCDR = PMC_SCDR_PCK0 | PMC_SCDR_PCK1 | PMC_SCDR_PCK2;
}

/**
 * \brief Check if the specified programmable clock is enabled.
 *
 * \param ul_id Id of the programmable clock.
 *
 * \retval 0 Programmable clock is disabled or unknown.
 * \retval 1 Programmable clock is enabled.
 */
uint32_t pmc_is_pck_enabled(uint32_t ul_id)
{
	if (ul_id > 2) {
		return 0;
	}

	return (PMC->PMC_SCSR & (PMC_SCSR_PCK0 << ul_id));
}

#if (SAM4C || SAM4CM || SAM4CP)
/**
 * \brief Enable Coprocessor Clocks.
 */
void pmc_enable_cpck(void)
{
	PMC->PMC_SCER = PMC_SCER_CPCK | PMC_SCER_CPKEY_PASSWD;
}

/**
 * \brief Disable Coprocessor Clocks.
 */
void pmc_disable_cpck(void)
{
	PMC->PMC_SCDR = PMC_SCDR_CPCK | PMC_SCDR_CPKEY_PASSWD;
}

/**
 * \brief Check if the Coprocessor Clocks is enabled.
 *
 * \retval 0 Coprocessor Clocks is disabled.
 * \retval 1 Coprocessor Clocks is enabled.
 */
bool pmc_is_cpck_enabled(void)
{
	if(PMC->PMC_SCSR & PMC_SCSR_CPCK) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable Coprocessor Bus Master Clocks.
 */
void pmc_enable_cpbmck(void)
{
	PMC->PMC_SCER = PMC_SCER_CPBMCK | PMC_SCER_CPKEY_PASSWD;
}

/**
 * \brief Disable Coprocessor Bus Master Clocks.
 */
void pmc_disable_cpbmck(void)
{
	PMC->PMC_SCDR = PMC_SCDR_CPBMCK | PMC_SCDR_CPKEY_PASSWD;
}

/**
 * \brief Check if the Coprocessor Bus Master Clocks is enabled.
 *
 * \retval 0 Coprocessor Bus Master Clocks is disabled.
 * \retval 1 Coprocessor Bus Master Clocks is enabled.
 */
bool pmc_is_cpbmck_enabled(void)
{
	if(PMC->PMC_SCSR & PMC_SCSR_CPBMCK) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Set the prescaler for the Coprocessor Master Clock.
 *
 * \param ul_pres Prescaler value.
 */
void pmc_cpck_set_prescaler(uint32_t ul_pres)
{
	PMC->PMC_MCKR =
			(PMC->PMC_MCKR & (~PMC_MCKR_CPPRES_Msk)) | PMC_MCKR_CPPRES(ul_pres);
}

/**
 * \brief Set the source for the Coprocessor Master Clock.
 *
 * \param ul_source Source selection value.
 */
void pmc_cpck_set_source(uint32_t ul_source)
{
	PMC->PMC_MCKR =
			(PMC->PMC_MCKR & (~PMC_MCKR_CPCSS_Msk)) | ul_source;
}
#endif

#if (SAM3S || SAM3XA || SAM4S || SAM4E || SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Switch UDP (USB) clock source selection to PLLA clock.
 *
 * \param ul_usbdiv Clock divisor.
 */
void pmc_switch_udpck_to_pllack(uint32_t ul_usbdiv)
{
	PMC->PMC_USB = PMC_USB_USBDIV(ul_usbdiv);
}
#endif

#if (SAM3S || SAM4S || SAMG55)
/**
 * \brief Switch UDP (USB) clock source selection to PLLB clock.
 *
 * \param ul_usbdiv Clock divisor.
 */
void pmc_switch_udpck_to_pllbck(uint32_t ul_usbdiv)
{
	PMC->PMC_USB = PMC_USB_USBDIV(ul_usbdiv) | PMC_USB_USBS;
}
#endif

#if (SAM3XA || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Switch UDP (USB) clock source selection to UPLL clock.
 *
 * \param ul_usbdiv Clock divisor.
 */
void pmc_switch_udpck_to_upllck(uint32_t ul_usbdiv)
{
	PMC->PMC_USB = PMC_USB_USBS | PMC_USB_USBDIV(ul_usbdiv);
}
#endif

#if (SAM3S || SAM3XA || SAM4S || SAM4E || SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Enable UDP (USB) clock.
 */
void pmc_enable_udpck(void)
{
#if (SAM3S || SAM4S || SAM4E || SAMG55)
	PMC->PMC_SCER = PMC_SCER_UDP;
#elif (SAMV71 || SAMV70 || SAME70 || SAMS70)
	PMC->PMC_SCER = PMC_SCER_USBCLK;
#else
	PMC->PMC_SCER = PMC_SCER_UOTGCLK;
# endif
}

/**
 * \brief Disable UDP (USB) clock.
 */
void pmc_disable_udpck(void)
{
#if (SAM3S || SAM4S || SAM4E || SAMG55)
	PMC->PMC_SCDR = PMC_SCDR_UDP;
#elif (SAMV71 || SAMV70 || SAME70 || SAMS70)
	PMC->PMC_SCDR = PMC_SCDR_USBCLK;
#else
	PMC->PMC_SCDR = PMC_SCDR_UOTGCLK;
# endif
}
#endif

#if SAMG55
/**
 * \brief Switch UHP (USB) clock source selection to PLLA clock.
 *
 * \param ul_usbdiv Clock divisor.
 */
void pmc_switch_uhpck_to_pllack(uint32_t ul_usbdiv)
{
	PMC->PMC_USB = PMC_USB_USBDIV(ul_usbdiv);
}

/**
 * \brief Switch UHP (USB) clock source selection to PLLB clock.
 *
 * \param ul_usbdiv Clock divisor.
 */
void pmc_switch_uhpck_to_pllbck(uint32_t ul_usbdiv)
{
	PMC->PMC_USB = PMC_USB_USBDIV(ul_usbdiv) | PMC_USB_USBS;
}

/**
 * \brief Enable UHP (USB) clock.
 */
void pmc_enable_uhpck(void)
{
	PMC->PMC_SCER = PMC_SCER_UHP;
}
#endif

/**
 * \brief Enable PMC interrupts.
 *
 * \param ul_sources Interrupt sources bit map.
 */
void pmc_enable_interrupt(uint32_t ul_sources)
{
	PMC->PMC_IER = ul_sources;
}

/**
 * \brief Disable PMC interrupts.
 *
 * \param ul_sources Interrupt sources bit map.
 */
void pmc_disable_interrupt(uint32_t ul_sources)
{
	PMC->PMC_IDR = ul_sources;
}

/**
 * \brief Get PMC interrupt mask.
 *
 * \return The interrupt mask value.
 */
uint32_t pmc_get_interrupt_mask(void)
{
	return PMC->PMC_IMR;
}

/**
 * \brief Get current status.
 *
 * \return The current PMC status.
 */
uint32_t pmc_get_status(void)
{
	return PMC->PMC_SR;
}

/**
 * \brief Set the wake-up inputs for fast startup mode registers
 *        (event generation).
 *
 * \param ul_inputs Wake up inputs to enable.
 */
void pmc_set_fast_startup_input(uint32_t ul_inputs)
{
	ul_inputs &= PMC_FAST_STARTUP_Msk;
	PMC->PMC_FSMR |= ul_inputs;
}

/**
 * \brief Clear the wake-up inputs for fast startup mode registers
 *        (remove event generation).
 *
 * \param ul_inputs Wake up inputs to disable.
 */
void pmc_clr_fast_startup_input(uint32_t ul_inputs)
{
	ul_inputs &= PMC_FAST_STARTUP_Msk;
	PMC->PMC_FSMR &= ~ul_inputs;
}

#if (SAM4C || SAM4CM || SAM4CP)
/**
 * \brief Set the wake-up inputs of coprocessor for fast startup mode registers
 *        (event generation).
 *
 * \param ul_inputs Wake up inputs to enable.
 */
void pmc_cp_set_fast_startup_input(uint32_t ul_inputs)
{
	ul_inputs &= PMC_FAST_STARTUP_Msk;
	PMC->PMC_CPFSMR |= ul_inputs;
}

/**
 * \brief Clear the wake-up inputs of coprocessor for fast startup mode registers
 *        (remove event generation).
 *
 * \param ul_inputs Wake up inputs to disable.
 */
void pmc_cp_clr_fast_startup_input(uint32_t ul_inputs)
{
	ul_inputs &= PMC_FAST_STARTUP_Msk;
	PMC->PMC_CPFSMR &= ~ul_inputs;
}
#endif

/**
 * \brief Enable Sleep Mode.
 * Enter condition: (WFE or WFI) + (SLEEPDEEP bit = 0) + (LPM bit = 0)
 *
 * \param uc_type 0 for wait for interrupt, 1 for wait for event.
 * \note For SAM4S, SAM4C, SAM4CM, SAM4CP, SAMV71 and SAM4E series,
 * since only WFI is effective, uc_type = 1 will be treated as uc_type = 0.
 */
void pmc_enable_sleepmode(uint8_t uc_type)
{
#if !(SAM4S || SAM4E || SAM4N || SAM4C || SAM4CM || SAM4CP || SAMV71 || SAMV70 || SAME70 || SAMS70)
	PMC->PMC_FSMR &= (uint32_t) ~ PMC_FSMR_LPM; // Enter Sleep mode
#endif
	SCB->SCR &= (uint32_t) ~ SCB_SCR_SLEEPDEEP_Msk; // Deep sleep

#if (SAM4S || SAM4E || SAM4N || SAM4C || SAM4CM || SAM4CP || SAMV71 || SAMV70 || SAME70 || SAMS70)
	UNUSED(uc_type);
	__DSB();
	__WFI();
#else
	if (uc_type == 0) {
		__DSB();
		__WFI();
	} else {
		__DSB();
		__WFE();
	}
#endif
}

#if (SAM4S || SAM4E || SAM4N || SAM4C || SAM4CM || SAMG || SAM4CP || SAMV71 || SAMV70 || SAME70 || SAMS70)
static uint32_t ul_flash_in_wait_mode = PMC_WAIT_MODE_FLASH_DEEP_POWERDOWN;
/**
 * \brief Set the embedded flash state in wait mode
 *
 * \param ul_flash_state PMC_WAIT_MODE_FLASH_STANDBY flash in standby mode,
 * PMC_WAIT_MODE_FLASH_DEEP_POWERDOWN flash in deep power down mode.
 */
void pmc_set_flash_in_wait_mode(uint32_t ul_flash_state)
{
	ul_flash_in_wait_mode = ul_flash_state;
}

/**
 * \brief Enable Wait Mode. Enter condition: (WAITMODE bit = 1) + FLPM
 *
 * \note In this function, FLPM will retain, WAITMODE bit will be set,
 * Generally, this function will be called by pmc_sleep() in order to
 * complete all sequence entering wait mode.
 * See \ref pmc_sleep() for entering different sleep modes.
 */
void pmc_enable_waitmode(void)
{
	uint32_t i;

	/* Flash in wait mode */
	i = PMC->PMC_FSMR;
	i &= ~PMC_FSMR_FLPM_Msk;
#if !(SAMV71 || SAMV70 || SAME70 || SAMS70)
	i |= ul_flash_in_wait_mode;
#else
	(void)ul_flash_in_wait_mode;
	i |= PMC_WAIT_MODE_FLASH_IDLE;
#endif
	PMC->PMC_FSMR = i;

	/* Set the WAITMODE bit = 1 */
	PMC->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_WAITMODE;

	/* Waiting for Master Clock Ready MCKRDY = 1 */
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

	/* Waiting for MOSCRCEN bit cleared is strongly recommended
	 * to ensure that the core will not execute undesired instructions
	 */
	for (i = 0; i < 500; i++) {
		__NOP();
	}
	while (!(PMC->CKGR_MOR & CKGR_MOR_MOSCRCEN));

#if (!SAMG)
	/* Restore Flash in idle mode */
	i = PMC->PMC_FSMR;
	i &= ~PMC_FSMR_FLPM_Msk;
	i |= PMC_WAIT_MODE_FLASH_IDLE;
	PMC->PMC_FSMR = i;
#endif
}
#else
/**
 * \brief Enable Wait Mode. Enter condition: WFE + (SLEEPDEEP bit = 0) +
 * (LPM bit = 1)
 */
void pmc_enable_waitmode(void)
{
	uint32_t i;

	PMC->PMC_FSMR |= PMC_FSMR_LPM; /* Enter Wait mode */
	SCB->SCR &= (uint32_t) ~ SCB_SCR_SLEEPDEEP_Msk; /* Deep sleep */

	__DSB();
	__WFE();

	/* Waiting for MOSCRCEN bit cleared is strongly recommended
	 * to ensure that the core will not execute undesired instructions
	 */
	for (i = 0; i < 500; i++) {
		__NOP();
	}
	while (!(PMC->CKGR_MOR & CKGR_MOR_MOSCRCEN));

}
#endif

#if (!(SAMG51 || SAMG53 || SAMG54))
/**
 * \brief Enable Backup Mode. Enter condition: WFE/(VROFF bit = 1) +
 * (SLEEPDEEP bit = 1)
 */
void pmc_enable_backupmode(void)
{
#if (SAM4C || SAM4CM || SAM4CP)
	uint32_t tmp = SUPC->SUPC_MR & ~(SUPC_MR_BUPPOREN | SUPC_MR_KEY_Msk);
	SUPC->SUPC_MR = tmp | SUPC_MR_KEY_PASSWD;
	while (SUPC->SUPC_SR & SUPC_SR_BUPPORS);
#endif
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
#if (SAM4S || SAM4E || SAM4N || SAM4C || SAM4CM || SAM4CP || SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
	SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_VROFF_STOP_VREG;
	uint32_t ul_dummy = SUPC->SUPC_MR;
	UNUSED(ul_dummy);
	__DSB();
	__WFE();
	__WFI();
#else
	__DSB();
	__WFE();
#endif
}
#endif

/**
 * \brief Enable Clock Failure Detector.
 */
void pmc_enable_clock_failure_detector(void)
{
	uint32_t ul_reg = PMC->CKGR_MOR;

	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_CFDEN | ul_reg;
}

/**
 * \brief Disable Clock Failure Detector.
 */
void pmc_disable_clock_failure_detector(void)
{
	uint32_t ul_reg = PMC->CKGR_MOR & (~CKGR_MOR_CFDEN);

	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | ul_reg;
}

#if (SAM4N || SAM4C || SAM4CM || SAM4CP || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Enable Slow Crystal Oscillator Frequency Monitoring.
 */
void pmc_enable_sclk_osc_freq_monitor(void)
{
	uint32_t ul_reg = PMC->CKGR_MOR;

	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_XT32KFME | ul_reg;
}

/**
 * \brief Disable Slow Crystal Oscillator Frequency Monitoring.
 */
void pmc_disable_sclk_osc_freq_monitor(void)
{
	uint32_t ul_reg = PMC->CKGR_MOR & (~CKGR_MOR_XT32KFME);

	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | ul_reg;
}
#endif

/**
 * \brief Enable or disable write protect of PMC registers.
 *
 * \param ul_enable 1 to enable, 0 to disable.
 */
void pmc_set_writeprotect(uint32_t ul_enable)
{
	if (ul_enable) {
		PMC->PMC_WPMR = PMC_WPMR_WPKEY_PASSWD | PMC_WPMR_WPEN;
	} else {
		PMC->PMC_WPMR = PMC_WPMR_WPKEY_PASSWD;
	}
}

/**
 * \brief Return write protect status.
 *
 * \return Return write protect status.
 */
uint32_t pmc_get_writeprotect_status(void)
{
	return PMC->PMC_WPSR;
}

#if (SAMG53 || SAMG54 || SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Enable the specified peripheral clock.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Fail.
 */
uint32_t pmc_enable_sleepwalking(uint32_t ul_id)
{
	uint32_t temp;
#if (SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
	if ((7 <= ul_id) && (ul_id<= 29)) {
#else
	if ((8 <= ul_id) && (ul_id<= 29)) {
#endif
		temp = pmc_get_active_status0();
		if (temp & (1 << ul_id)) {
			return 1;
		}
		PMC->PMC_SLPWK_ER0 = 1 << ul_id;
		temp = pmc_get_active_status0();
		if (temp & (1 << ul_id)) {
			pmc_disable_sleepwalking(ul_id);
			return 1;
		}
		return 0;
	}
#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
	else if ((32 <= ul_id) && (ul_id<= 60)) {
		ul_id -= 32;
		temp = pmc_get_active_status1();
		if (temp & (1 << ul_id)) {
			return 1;
		}
		PMC->PMC_SLPWK_ER1 = 1 << ul_id;
		temp = pmc_get_active_status1();
		if (temp & (1 << ul_id)) {
			pmc_disable_sleepwalking(ul_id);
			return 1;
		}
		return 0;
	}
#endif
	else {
		return 1;
	}
}

/**
 * \brief Disable the sleepwalking of specified peripheral.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_disable_sleepwalking(uint32_t ul_id)
{
#if (SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
	if ((7 <= ul_id) && (ul_id<= 29)) {
#else
	if ((8 <= ul_id) && (ul_id<= 29)) {
#endif
		PMC->PMC_SLPWK_DR0 = 1 << ul_id;
		return 0;
	}
#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
	else if ((32 <= ul_id) && (ul_id<= 60)) {
		ul_id -= 32;
		PMC->PMC_SLPWK_DR1 = 1 << ul_id;
		return 0;
	}
#endif
	else {
		return 1;
	}
}

/**
 * \brief Return peripheral sleepwalking enable status.
 *
 * \return the status register value.
 */
uint32_t pmc_get_sleepwalking_status0(void)
{
	return PMC->PMC_SLPWK_SR0;
}

/**
 * \brief Return peripheral active status.
 *
 * \return the status register value.
 */
uint32_t pmc_get_active_status0(void)
{
	return PMC->PMC_SLPWK_ASR0;
}

#endif

#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \brief Return peripheral sleepwalking enable status.
 *
 * \return the status register value.
 */
uint32_t pmc_get_sleepwalking_status1(void)
{
	return PMC->PMC_SLPWK_SR1;
}

/**
 * \brief Return peripheral active status.
 *
 * \return the status register value.
 */
uint32_t pmc_get_active_status1(void)
{
	return PMC->PMC_SLPWK_ASR1;
}
#endif

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
