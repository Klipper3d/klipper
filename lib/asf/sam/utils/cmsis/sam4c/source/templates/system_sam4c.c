/**
 * \file
 *
 * \brief Provides the low-level initialization functions that called
 * on chip startup.
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

#include "sam4c.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */


/**
 * System Clock Frequency (Core Clock)
 */
uint32_t SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

/**
 * \brief Setup the microcontroller system.
 *
 * Initialize the System and update the SystemFrequency variable.
 */
void SystemInit(void)
{
	/*
	 * TODO:
	 * Add code to initialize the system according to your application.
	 *
	 * For SAM4C, the internal 4MHz fast RC oscillator is the deault clock
	 * selected to start up the system.
	 *
	 * Note:
	 * After reset, the core 1 is hold in reset and with no clock.
	 */
	SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;
}

/**
 * \brief Get Core Clock Frequency.
 */
void SystemCoreClockUpdate(void)
{
	/*
	 * TODO:
	 * Add code to calculate the system frequency based upon the current
	 * register settings.
	 *
	 * This function can be used to retrieve the system core clock frequeny
	 * after user changed register sittings.
	 */
}

/**
 * \brief Initialize flash wait state according to operating frequency.
 *
 * \param ul_clk System clock frequency.
 */
#ifdef _SAM4C_EFC_INSTANCE_
void system_init_flash(uint32_t ul_clk)
{
	/* Set FWS for embedded Flash access according to operating frequency */
	if (ul_clk < CHIP_FREQ_FWS_0) {
		EFC->EEFC_FMR = EEFC_FMR_FWS(0)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_1) {
		EFC->EEFC_FMR = EEFC_FMR_FWS(1)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_2) {
		EFC->EEFC_FMR = EEFC_FMR_FWS(2)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_3) {
		EFC->EEFC_FMR = EEFC_FMR_FWS(3)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_4) {
		EFC->EEFC_FMR = EEFC_FMR_FWS(4)|EEFC_FMR_CLOE;
	} else {
		EFC->EEFC_FMR = EEFC_FMR_FWS(5)|EEFC_FMR_CLOE;
	}
}
#else
void system_init_flash( uint32_t ul_clk )
{
  /* Set FWS for embedded Flash access according to operating frequency */
#if !defined(ID_EFC1)
    if (ul_clk < CHIP_FREQ_FWS_0) {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(0)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_1) {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(1)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_2) {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(2)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_3) {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(3)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_4) {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(4)|EEFC_FMR_CLOE;
	} else {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(5)|EEFC_FMR_CLOE;
	}
#else
	if (ul_clk < CHIP_FREQ_FWS_0) {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(0)|EEFC_FMR_CLOE;
		EFC1->EEFC_FMR = EEFC_FMR_FWS(0)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_1) {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(1)|EEFC_FMR_CLOE;
		EFC1->EEFC_FMR = EEFC_FMR_FWS(1)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_2) {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(2)|EEFC_FMR_CLOE;
		EFC1->EEFC_FMR = EEFC_FMR_FWS(2)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_3) {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(3)|EEFC_FMR_CLOE;
		EFC1->EEFC_FMR = EEFC_FMR_FWS(3)|EEFC_FMR_CLOE;
	} else if (ul_clk < CHIP_FREQ_FWS_4) {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(4)|EEFC_FMR_CLOE;
		EFC1->EEFC_FMR = EEFC_FMR_FWS(4)|EEFC_FMR_CLOE;
	} else {
		EFC0->EEFC_FMR = EEFC_FMR_FWS(5)|EEFC_FMR_CLOE;
		EFC1->EEFC_FMR = EEFC_FMR_FWS(5)|EEFC_FMR_CLOE;
	}
	
#endif
}

#endif
/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
