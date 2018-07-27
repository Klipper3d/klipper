/**
 * \file
 *
 * \brief Chip-specific system clock management functions.
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

#include <sysclk.h>

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \weakgroup sysclk_group
 * @{
 */

#if defined(CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
/**
 * \brief boolean signalling that the sysclk_init is done.
 */
uint32_t sysclk_initialized = 0;
#endif

/**
 * \brief Set system clock prescaler configuration
 *
 * This function will change the system clock prescaler configuration to
 * match the parameters.
 *
 * \note The parameters to this function are device-specific.
 *
 * \param cpu_shift The CPU clock will be divided by \f$2^{mck\_pres}\f$
 */
void sysclk_set_prescalers(uint32_t ul_pres)
{
	pmc_mck_set_prescaler(ul_pres);
	SystemCoreClockUpdate();
}

/**
 * \brief Change the source of the main system clock.
 *
 * \param src The new system clock source. Must be one of the constants
 * from the <em>System Clock Sources</em> section.
 */
void sysclk_set_source(uint32_t ul_src)
{
	switch (ul_src) {
	case SYSCLK_SRC_SLCK_RC:
	case SYSCLK_SRC_SLCK_XTAL:
	case SYSCLK_SRC_SLCK_BYPASS:
		pmc_mck_set_source(PMC_MCKR_CSS_SLOW_CLK);
		break;

	case SYSCLK_SRC_MAINCK_4M_RC:
	case SYSCLK_SRC_MAINCK_8M_RC:
	case SYSCLK_SRC_MAINCK_12M_RC:
	case SYSCLK_SRC_MAINCK_XTAL:
	case SYSCLK_SRC_MAINCK_BYPASS:
		pmc_mck_set_source(PMC_MCKR_CSS_MAIN_CLK);
		break;

	case SYSCLK_SRC_PLLACK:
		pmc_mck_set_source(PMC_MCKR_CSS_PLLA_CLK);
		break;

	case SYSCLK_SRC_PLLBCK:
		pmc_mck_set_source(PMC_MCKR_CSS_PLLB_CLK);
		break;
	}

	SystemCoreClockUpdate();
}

#if defined(CONFIG_USBCLK_SOURCE) || defined(__DOXYGEN__)
/**
 * \brief Enable USB clock.
 *
 * \note The SAM3S UDP hardware interprets div as div+1. For readability the hardware div+1
 * is hidden in this implementation. Use div as div effective value.
 *
 * \param pll_id Source of the USB clock.
 * \param div Actual clock divisor. Must be superior to 0.
 */
void sysclk_enable_usb(void)
{
	Assert(CONFIG_USBCLK_DIV > 0);

#ifdef CONFIG_PLL0_SOURCE
	if (CONFIG_USBCLK_SOURCE == USBCLK_SRC_PLL0) {
		struct pll_config pllcfg;

		pll_enable_source(CONFIG_PLL0_SOURCE);
		pll_config_defaults(&pllcfg, 0);
		pll_enable(&pllcfg, 0);
		pll_wait_for_lock(0);
		pmc_switch_udpck_to_pllack(CONFIG_USBCLK_DIV - 1);
		pmc_enable_udpck();
		return;
	}
#endif

#ifdef CONFIG_PLL1_SOURCE
	if (CONFIG_USBCLK_SOURCE == USBCLK_SRC_PLL1) {
		struct pll_config pllcfg;

		pll_enable_source(CONFIG_PLL1_SOURCE);
		pll_config_defaults(&pllcfg, 1);
		pll_enable(&pllcfg, 1);
		pll_wait_for_lock(1);
		pmc_switch_udpck_to_pllbck(CONFIG_USBCLK_DIV - 1);
		pmc_enable_udpck();
		return;
	}
#endif
}

/**
 * \brief Disable the USB clock.
 *
 * \note This implementation does not switch off the PLL, it just turns off the USB clock.
 */
void sysclk_disable_usb(void)
{
	pmc_disable_udpck();
}
#endif // CONFIG_USBCLK_SOURCE

void sysclk_init(void)
{
	/* Set flash wait state to max in case the below clock switching. */
	system_init_flash(CHIP_FREQ_CPU_MAX);

	/* Config system clock setting */
	if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_RC) {
		osc_enable(OSC_SLCK_32K_RC);
		osc_wait_ready(OSC_SLCK_32K_RC);
		pmc_switch_mck_to_sclk(CONFIG_SYSCLK_PRES);
	}

	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_XTAL) {
		osc_enable(OSC_SLCK_32K_XTAL);
		osc_wait_ready(OSC_SLCK_32K_XTAL);
		pmc_switch_mck_to_sclk(CONFIG_SYSCLK_PRES);
	}

	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_BYPASS) {
		osc_enable(OSC_SLCK_32K_BYPASS);
		osc_wait_ready(OSC_SLCK_32K_BYPASS);
		pmc_switch_mck_to_sclk(CONFIG_SYSCLK_PRES);
	}

	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_4M_RC) {
		/* Already running from SYSCLK_SRC_MAINCK_4M_RC */
	}

	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_8M_RC) {
		osc_enable(OSC_MAINCK_8M_RC);
		osc_wait_ready(OSC_MAINCK_8M_RC);
		pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
	}

	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_12M_RC) {
		osc_enable(OSC_MAINCK_12M_RC);
		osc_wait_ready(OSC_MAINCK_12M_RC);
		pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
	}

	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_XTAL) {
		osc_enable(OSC_MAINCK_XTAL);
		osc_wait_ready(OSC_MAINCK_XTAL);
		pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
	}

	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_BYPASS) {
		osc_enable(OSC_MAINCK_BYPASS);
		osc_wait_ready(OSC_MAINCK_BYPASS);
		pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
	}

#ifdef CONFIG_PLL0_SOURCE
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_PLLACK) {
		struct pll_config pllcfg;

		pll_enable_source(CONFIG_PLL0_SOURCE);
		pll_config_defaults(&pllcfg, 0);
		pll_enable(&pllcfg, 0);
		pll_wait_for_lock(0);
		pmc_switch_mck_to_pllack(CONFIG_SYSCLK_PRES);
	}
#endif

#ifdef CONFIG_PLL1_SOURCE
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_PLLBCK) {
		struct pll_config pllcfg;

		pll_enable_source(CONFIG_PLL1_SOURCE);
		pll_config_defaults(&pllcfg, 1);
		pll_enable(&pllcfg, 1);
		pll_wait_for_lock(1);
		pmc_switch_mck_to_pllbck(CONFIG_SYSCLK_PRES);
	}
#endif

	/* Update the SystemFrequency variable */
	SystemCoreClockUpdate();

	/* Set a flash wait state depending on the new cpu frequency */
	system_init_flash(sysclk_get_cpu_hz());

#if (defined CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
	/* Signal that the internal frequencies are setup */
	sysclk_initialized = 1;
#endif
}

//! @}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
