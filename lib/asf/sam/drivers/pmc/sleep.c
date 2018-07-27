/**
 * \file
 *
 * \brief Sleep mode access
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
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

#include <compiler.h>
#include "sleep.h"

/* SAM3,SAM4,SAMG,SAMV,SAMS and SAME series */
#if (SAM3S || SAM3N || SAM3XA || SAM3U || SAM4S || SAM4E || SAM4N || SAM4C || \
		SAM4CM || SAMG || SAM4CP || SAMV71 || SAMV70 || SAMS70 || SAME70)
# include "pmc.h"
# include "board.h"

/* Checking board configuration of main clock xtal statup time */
#if !defined(BOARD_OSC_STARTUP_US)
# warning The board main clock xtal statup time has not been defined. Using default settings.
# define BOARD_OSC_STARTUP_US    (15625UL)
#endif

#if !defined(EFC0)
# define EFC0 EFC
#endif

/**
 * Save clock settings and shutdown PLLs
 */
__always_inline static void pmc_save_clock_settings(
		uint32_t *p_osc_setting,
		uint32_t *p_pll0_setting,
		uint32_t *p_pll1_setting,
		uint32_t *p_mck_setting,
		uint32_t *p_fmr_setting,
#if defined(EFC1)
		uint32_t *p_fmr_setting1,
#endif
		const bool disable_xtal)
{
	uint32_t mor  = PMC->CKGR_MOR;
	uint32_t mckr = PMC->PMC_MCKR;
	uint32_t fmr  = EFC0->EEFC_FMR;
# if defined(EFC1)
	uint32_t fmr1 = EFC1->EEFC_FMR;
# endif

	if (p_osc_setting) {
		*p_osc_setting = mor;
	}
	if (p_pll0_setting) {
		*p_pll0_setting = PMC->CKGR_PLLAR;
	}
	if (p_pll1_setting) {
#if (SAM3S || SAM4S || SAM4C || SAM4CM || SAM4CP)
		*p_pll1_setting = PMC->CKGR_PLLBR;
#elif (SAM3U || SAM3XA)
		*p_pll1_setting = PMC->CKGR_UCKR;
#else
		*p_pll1_setting = 0;
#endif
	}
	if (p_mck_setting) {
		*p_mck_setting  = mckr;
	}
	if (p_fmr_setting) {
		*p_fmr_setting  = fmr;
	}
#if defined(EFC1)
	if (p_fmr_setting1) {
		*p_fmr_setting1 = fmr1;
	}
#endif

	/* Enable FAST RC */
	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | mor | CKGR_MOR_MOSCRCEN;
	/* if MCK source is PLL, switch to mainck */
	if ((mckr & PMC_MCKR_CSS_Msk) > PMC_MCKR_CSS_MAIN_CLK) {
		/* MCK -> MAINCK */
		mckr = (mckr & (~PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_MAIN_CLK;
		PMC->PMC_MCKR = mckr;
		while(!(PMC->PMC_SR & PMC_SR_MCKRDY));
	}
	/* MCK prescale -> 1 */
	if (mckr & PMC_MCKR_PRES_Msk) {
		mckr = (mckr & (~PMC_MCKR_PRES_Msk));
		PMC->PMC_MCKR = mckr;
		while(!(PMC->PMC_SR & PMC_SR_MCKRDY));
	}
	/* Disable PLLs */
	pmc_disable_pllack();
#if (SAM3S || SAM4S || SAM4C || SAM4CM || SAM4CP)
	pmc_disable_pllbck();
#elif (SAM3U || SAM3XA)
	pmc_disable_upll_clock();
#endif

	/* Prepare for entering WAIT mode */
	/* Wait fast RC ready */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

	/* Switch mainck to FAST RC */
#if SAMG
	/**
	 * For the sleepwalking feature, we need an accurate RC clock. Only 24M and
	 * 16M are trimmed in production. Here we select the 24M.
	 * And so wait state need to be 1.
	 */
	EFC0->EEFC_FMR = (fmr & (~EEFC_FMR_FWS_Msk)) | EEFC_FMR_FWS(1);

	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCSEL) | CKGR_MOR_MOSCRCF_24_MHz |
			CKGR_MOR_KEY_PASSWD;
#else
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCSEL) |
			CKGR_MOR_KEY_PASSWD;
#endif
	while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));

#if (!SAMG)
	/* FWS update */
	EFC0->EEFC_FMR = fmr & (~EEFC_FMR_FWS_Msk);
#if defined(EFC1)
	EFC1->EEFC_FMR = fmr1 & (~EEFC_FMR_FWS_Msk);
#endif
#endif

	/* Disable XTALs */
	if (disable_xtal) {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTEN) |
				CKGR_MOR_KEY_PASSWD;
	}
}

/**
 * Restore clock settings
 */
__always_inline static void pmc_restore_clock_setting(
		const uint32_t osc_setting,
		const uint32_t pll0_setting,
		const uint32_t pll1_setting,
		const uint32_t mck_setting,
		const uint32_t fmr_setting
#if defined(EFC1)
		, const uint32_t fmr_setting1
#endif
		)
{
	uint32_t mckr;
	uint32_t pll_sr = 0;

	/* Switch mainck to external xtal */
	if (CKGR_MOR_MOSCXTBY == (osc_setting & CKGR_MOR_MOSCXTBY)) {
		/* Bypass mode */
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTEN) |
				CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTBY |
				CKGR_MOR_MOSCSEL;
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCEN &
					~CKGR_MOR_MOSCRCF_Msk)
				| CKGR_MOR_KEY_PASSWD;
	} else if (CKGR_MOR_MOSCXTEN == (osc_setting & CKGR_MOR_MOSCXTEN)) {
		/* Enable External XTAL */
		if (!(PMC->CKGR_MOR & CKGR_MOR_MOSCXTEN)) {
			PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTBY) |
					CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN;
			/* Wait the Xtal to stabilize */
			while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));
		}
		/* Select External XTAL */
		if (!(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL)) {
			PMC->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL;
			while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));
		}
		/* Disable Fast RC */
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCEN &
						~CKGR_MOR_MOSCRCF_Msk)
					| CKGR_MOR_KEY_PASSWD;
	}

	if (pll0_setting & CKGR_PLLAR_MULA_Msk) {
#if (SAM4C || SAM4CM || SAMG || SAM4CP)
		PMC->CKGR_PLLAR = pll0_setting;
#else
		PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | pll0_setting;
#endif
		pll_sr |= PMC_SR_LOCKA;
	}
#if (SAM3S || SAM4S || SAM4C || SAM4CM || SAM4CP)
	if (pll1_setting & CKGR_PLLBR_MULB_Msk) {
		PMC->CKGR_PLLBR = pll1_setting;
		pll_sr |= PMC_SR_LOCKB;
	}
#elif (SAM3U || SAM3XA)
	if (pll1_setting & CKGR_UCKR_UPLLEN) {
		PMC->CKGR_UCKR = pll1_setting;
		pll_sr |= PMC_SR_LOCKU;
	}
#else
	UNUSED(pll1_setting);
#endif
	/* Wait MCK source ready */
	switch(mck_setting & PMC_MCKR_CSS_Msk) {
	case PMC_MCKR_CSS_PLLA_CLK:
		while (!(PMC->PMC_SR & PMC_SR_LOCKA));
		break;
#if (SAM3S || SAM4S || SAM4C || SAM4CM || SAM4CP)
	case PMC_MCKR_CSS_PLLB_CLK:
		while (!(PMC->PMC_SR & PMC_SR_LOCKB));
		break;
#elif (SAM3U || SAM3XA)
	case PMC_MCKR_CSS_UPLL_CLK:
		while (!(PMC->PMC_SR & PMC_SR_LOCKU));
		break;
#endif
	}

	/* Switch to faster clock */
	mckr = PMC->PMC_MCKR;

	/* Set PRES */
	PMC->PMC_MCKR = (mckr & ~PMC_MCKR_PRES_Msk)
		| (mck_setting & PMC_MCKR_PRES_Msk);
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

	/* Restore flash wait states */
	EFC0->EEFC_FMR = fmr_setting;
#if defined(EFC1)
	EFC1->EEFC_FMR = fmr_setting1;
#endif

	/* Set CSS and others */
	PMC->PMC_MCKR = mck_setting;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

	/* Waiting all restored PLLs ready */
	while (!(PMC->PMC_SR & pll_sr));
}

/** If clocks are switched for some sleep mode */
static volatile bool b_is_sleep_clock_used = false;
/** Callback invoked once when clocks are restored */
static pmc_callback_wakeup_clocks_restored_t callback_clocks_restored = NULL;

void pmc_sleep(int sleep_mode)
{
	switch (sleep_mode) {
	case SAM_PM_SMODE_SLEEP_WFI:
	case SAM_PM_SMODE_SLEEP_WFE:
#if (SAM4S || SAM4E || SAM4N || SAM4C || SAM4CM || SAM4CP || SAMG || SAMV71 || SAMV70 || SAMS70 || SAME70)
		SCB->SCR &= (uint32_t)~SCR_SLEEPDEEP;
		cpu_irq_enable();
		__DSB();
		__WFI();
		break;
#else
		PMC->PMC_FSMR &= (uint32_t)~PMC_FSMR_LPM;
		SCB->SCR &= (uint32_t)~SCR_SLEEPDEEP;
		cpu_irq_enable();
		if (sleep_mode == SAM_PM_SMODE_SLEEP_WFI) {
			__DSB();
			__WFI();
		} else {
			__DSB();
			__WFE();
		}
		break;
#endif

	case SAM_PM_SMODE_WAIT_FAST:
	case SAM_PM_SMODE_WAIT: {
		uint32_t mor, pllr0, pllr1, mckr;
		uint32_t fmr;
#if defined(EFC1)
		uint32_t fmr1;
#endif
#if (SAM4S || SAM4E || SAM4N || SAM4C || SAM4CM || SAM4CP || SAMG || SAMV71 || SAMV70 || SAMS70 || SAME70)
		(sleep_mode == SAM_PM_SMODE_WAIT_FAST) ?
				pmc_set_flash_in_wait_mode(PMC_FSMR_FLPM_FLASH_STANDBY) :
				pmc_set_flash_in_wait_mode(PMC_FSMR_FLPM_FLASH_DEEP_POWERDOWN);
#endif
		cpu_irq_disable();
		b_is_sleep_clock_used = true;

#if (SAM4C || SAM4CM || SAM4CP)
		/* Backup the sub-system 1 status and stop sub-system 1 */
		uint32_t cpclk_backup = PMC->PMC_SCSR &
				(PMC_SCSR_CPCK | PMC_SCSR_CPBMCK);
		PMC->PMC_SCDR = cpclk_backup | PMC_SCDR_CPKEY_PASSWD;
#endif
		pmc_save_clock_settings(&mor, &pllr0, &pllr1, &mckr, &fmr,
#if defined(EFC1)
				&fmr1,
#endif
				(sleep_mode == SAM_PM_SMODE_WAIT));

		/* Enter wait mode */
		cpu_irq_enable();

		pmc_enable_waitmode();

		cpu_irq_disable();
		pmc_restore_clock_setting(mor, pllr0, pllr1, mckr, fmr
#if defined(EFC1)
				, fmr1
#endif
				);

#if (SAM4C || SAM4CM || SAM4CP)
		/* Restore the sub-system 1 */
		PMC->PMC_SCER = cpclk_backup | PMC_SCER_CPKEY_PASSWD;
#endif
		b_is_sleep_clock_used = false;
		if (callback_clocks_restored) {
			callback_clocks_restored();
			callback_clocks_restored = NULL;
		}
		cpu_irq_enable();

		break;
	}
#if (!(SAMG51 || SAMG53 || SAMG54))
	case SAM_PM_SMODE_BACKUP:
		SCB->SCR |= SCR_SLEEPDEEP;
#if (SAM4S || SAM4E || SAM4N || SAM4C || SAM4CM || SAM4CP || SAMG55 || SAMV71 || SAMV70 || SAMS70 || SAME70)
		SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_VROFF_STOP_VREG;
		cpu_irq_enable();
		__WFI() ;
#else
		cpu_irq_enable();
		__WFE() ;
#endif
		break;
#endif
	}
}

bool pmc_is_wakeup_clocks_restored(void)
{
	return !b_is_sleep_clock_used;
}

void pmc_wait_wakeup_clocks_restore(
		pmc_callback_wakeup_clocks_restored_t callback)
{
	if (b_is_sleep_clock_used) {
		cpu_irq_disable();
		callback_clocks_restored = callback;
	} else if (callback) {
		callback();
	}
}

#endif
