/**
 * \file
 *
 * \brief Provides the low-level initialization functions that called
 * on chip startup.
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

#include "sam4e.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/* Clock Settings (120MHz) */
#define SYS_BOARD_OSCOUNT   (CKGR_MOR_MOSCXTST(0x8U))
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE \
		| CKGR_PLLAR_MULA(0x13U) \
		| CKGR_PLLAR_PLLACOUNT(0x3fU) \
		| CKGR_PLLAR_DIVA(0x1U))
#define SYS_BOARD_MCKR      (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)

#define SYS_CKGR_MOR_KEY_VALUE	CKGR_MOR_KEY_PASSWD/* Key to unlock MOR register */

uint32_t SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

/**
 * \brief Setup the microcontroller system.
 * Initialize the System and update the SystemFrequency variable.
 */
void SystemInit( void )
{
	/* Set FWS according to SYS_BOARD_MCKR configuration */
	EFC->EEFC_FMR = EEFC_FMR_FWS(5)|EEFC_FMR_CLOE;

	/* Initialize main oscillator */
	if ( !(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) ) {
		PMC->CKGR_MOR = SYS_CKGR_MOR_KEY_VALUE | SYS_BOARD_OSCOUNT |
				CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;

		while ( !(PMC->PMC_SR & PMC_SR_MOSCXTS) ) {
		}
	}

	/* Switch to 3-20MHz Xtal oscillator */
	PMC->CKGR_MOR = SYS_CKGR_MOR_KEY_VALUE | SYS_BOARD_OSCOUNT |
			CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN |
			CKGR_MOR_MOSCSEL;

	while ( !(PMC->PMC_SR & PMC_SR_MOSCSELS) ) {
	}

	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk) |
			PMC_MCKR_CSS_MAIN_CLK;

	while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) ) {
	}

	/* Initialize PLLA */
	PMC->CKGR_PLLAR = SYS_BOARD_PLLAR;
	while ( !(PMC->PMC_SR & PMC_SR_LOCKA) ) {
	}

	/* Switch to main clock */
	PMC->PMC_MCKR = (SYS_BOARD_MCKR & ~PMC_MCKR_CSS_Msk) |
			PMC_MCKR_CSS_MAIN_CLK;
	while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) ) {
	}

	/* Switch to PLLA */
	PMC->PMC_MCKR = SYS_BOARD_MCKR;
	while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) ) {
	}

	SystemCoreClock = CHIP_FREQ_CPU_MAX;
}

void SystemCoreClockUpdate( void )
{
	/* Determine clock frequency according to clock register values */
	switch (PMC->PMC_MCKR & (uint32_t) PMC_MCKR_CSS_Msk) {
	case PMC_MCKR_CSS_SLOW_CLK:	/* Slow clock */
		if ( SUPC->SUPC_SR & SUPC_SR_OSCSEL ) {
			SystemCoreClock = CHIP_FREQ_XTAL_32K;
		} else {
			SystemCoreClock = CHIP_FREQ_SLCK_RC;
		}
		break;

	case PMC_MCKR_CSS_MAIN_CLK:	/* Main clock */
		if ( PMC->CKGR_MOR & CKGR_MOR_MOSCSEL ) {
			SystemCoreClock = CHIP_FREQ_XTAL_12M;
		} else {
			SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

			switch ( PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk ) {
			case CKGR_MOR_MOSCRCF_4_MHz:
				break;

			case CKGR_MOR_MOSCRCF_8_MHz:
				SystemCoreClock *= 2U;
				break;

			case CKGR_MOR_MOSCRCF_12_MHz:
				SystemCoreClock *= 3U;
				break;

			default:
				break;
			}
		}
		break;

	case PMC_MCKR_CSS_PLLA_CLK:	/* PLLA clock */
		if ( PMC->CKGR_MOR & CKGR_MOR_MOSCSEL ) {
			SystemCoreClock = CHIP_FREQ_XTAL_12M ;
		} else {
			SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

			switch ( PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk ) {
			case CKGR_MOR_MOSCRCF_4_MHz:
				break;

			case CKGR_MOR_MOSCRCF_8_MHz:
				SystemCoreClock *= 2U;
				break;

			case CKGR_MOR_MOSCRCF_12_MHz:
				SystemCoreClock *= 3U;
				break;

			default:
				break;
			}
		}

		if ((uint32_t) (PMC->PMC_MCKR & (uint32_t) PMC_MCKR_CSS_Msk) == PMC_MCKR_CSS_PLLA_CLK)
		{
			SystemCoreClock *= ((((PMC->CKGR_PLLAR) & CKGR_PLLAR_MULA_Msk) >> CKGR_PLLAR_MULA_Pos) + 1U);
			SystemCoreClock /= ((((PMC->CKGR_PLLAR) & CKGR_PLLAR_DIVA_Msk) >> CKGR_PLLAR_DIVA_Pos));
		}
		break;

	default:
		break;
	}

	if ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) == PMC_MCKR_PRES_CLK_3) {
		SystemCoreClock /= 3U;
	} else {
		SystemCoreClock >>= ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) >>
				PMC_MCKR_PRES_Pos);
	}
}

/**
 * Initialize flash.
 */
void system_init_flash( uint32_t ul_clk )
{
	/* Set FWS for embedded Flash access according to operating frequency */
	if ( ul_clk < CHIP_FREQ_FWS_0 ) {
		EFC->EEFC_FMR = EEFC_FMR_FWS(0)|EEFC_FMR_CLOE;
	} else {
		if (ul_clk < CHIP_FREQ_FWS_1) {
			EFC->EEFC_FMR = EEFC_FMR_FWS(1)|EEFC_FMR_CLOE;
		} else {
			if (ul_clk < CHIP_FREQ_FWS_2) {
				EFC->EEFC_FMR = EEFC_FMR_FWS(2)|EEFC_FMR_CLOE;
			} else {
				if ( ul_clk < CHIP_FREQ_FWS_3 ) {
					EFC->EEFC_FMR = EEFC_FMR_FWS(3)|EEFC_FMR_CLOE;
				} else {
					if ( ul_clk < CHIP_FREQ_FWS_4 ) {
						EFC->EEFC_FMR = EEFC_FMR_FWS(4)|EEFC_FMR_CLOE;
					} else {
						EFC->EEFC_FMR = EEFC_FMR_FWS(5)|EEFC_FMR_CLOE;
					}
				}
			}
		}
	}
}

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
