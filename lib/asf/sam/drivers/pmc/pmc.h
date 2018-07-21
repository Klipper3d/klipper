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

#ifndef PMC_H_INCLUDED
#define PMC_H_INCLUDED

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** Bit mask for peripheral clocks (PCER0) */
#define PMC_MASK_STATUS0        (0xFFFFFFFC)

/** Bit mask for peripheral clocks (PCER1) */
#define PMC_MASK_STATUS1        (0xFFFFFFFF)

/** Loop counter timeout value */
#if !(SAME70)
#define PMC_TIMEOUT             (2048)
#else
#define PMC_TIMEOUT             (4096)
#endif

/** Key to unlock CKGR_MOR register */
#ifndef CKGR_MOR_KEY_PASSWD
#define CKGR_MOR_KEY_PASSWD    CKGR_MOR_KEY(0x37U)
#endif

/** Key used to write SUPC registers */
#ifndef SUPC_CR_KEY_PASSWD
#define SUPC_CR_KEY_PASSWD    SUPC_CR_KEY(0xA5U)
#endif

#ifndef SUPC_MR_KEY_PASSWD
#define SUPC_MR_KEY_PASSWD    SUPC_MR_KEY(0xA5U)
#endif

/** Mask to access fast startup input */
#define PMC_FAST_STARTUP_Msk    (0x7FFFFu)

/** PMC_WPMR Write Protect KEY, unlock it */
#ifndef PMC_WPMR_WPKEY_PASSWD
#define PMC_WPMR_WPKEY_PASSWD    PMC_WPMR_WPKEY((uint32_t) 0x504D43)
#endif

/** Using external oscillator */
#define PMC_OSC_XTAL            0

/** Oscillator in bypass mode */
#define PMC_OSC_BYPASS          1

#define PMC_PCK_0               0 /* PCK0 ID */
#define PMC_PCK_1               1 /* PCK1 ID */
#define PMC_PCK_2               2 /* PCK2 ID */
#if (SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
#define PMC_PCK_3               3 /* PCK3 ID */
#define PMC_PCK_4               4 /* PCK4 ID */
#define PMC_PCK_5               5 /* PCK5 ID */
#define PMC_PCK_6               6 /* PCK6 ID */
#if SAMG55
#define PMC_PCK_7               7 /* PCK7 ID */
#endif
#endif

#if (SAM4S || SAM4E || SAM4N || SAM4C || SAM4CM || SAMG || SAM4CP || SAMV71 || SAMV70 || SAME70 || SAMS70)
/** Flash state in Wait Mode */
#define PMC_WAIT_MODE_FLASH_STANDBY         PMC_FSMR_FLPM_FLASH_STANDBY
#define PMC_WAIT_MODE_FLASH_DEEP_POWERDOWN  PMC_FSMR_FLPM_FLASH_DEEP_POWERDOWN
#define PMC_WAIT_MODE_FLASH_IDLE            PMC_FSMR_FLPM_FLASH_IDLE
#endif

/** Convert startup time from us to MOSCXTST */
#define pmc_us_to_moscxtst(startup_us, slowck_freq)      \
	((startup_us * slowck_freq / 8 / 1000000) < 0x100 ?  \
		(startup_us * slowck_freq / 8 / 1000000) : 0xFF)

/**
 * \name Master clock (MCK) Source and Prescaler configuration
 *
 * \note The following functions may be used to select the clock source and
 * prescaler for the master clock.
 */
//@{

void pmc_mck_set_prescaler(uint32_t ul_pres);
#if SAMV71 || SAMV70 || SAME70 || SAMS70
void pmc_mck_set_division(uint32_t ul_div);
#endif
void pmc_mck_set_source(uint32_t ul_source);
uint32_t pmc_switch_mck_to_sclk(uint32_t ul_pres);
uint32_t pmc_switch_mck_to_mainck(uint32_t ul_pres);
uint32_t pmc_switch_mck_to_pllack(uint32_t ul_pres);
#if (SAM3S || SAM4S || SAM4C || SAM4CM || SAM4CP || SAMG55)
uint32_t pmc_switch_mck_to_pllbck(uint32_t ul_pres);
#endif
#if (SAM3XA || SAM3U || SAMV71 || SAMV70 || SAME70 || SAMS70)
uint32_t pmc_switch_mck_to_upllck(uint32_t ul_pres);
#endif
#if (SAM4S || SAM4E || SAM4N || SAM4C || SAM4CM || SAMG || SAM4CP || SAMV71 || SAMV70 || SAME70 || SAMS70)
void pmc_set_flash_in_wait_mode(uint32_t ul_flash_state);
#endif


//@}

/**
 * \name Slow clock (SLCK) oscillator and configuration
 *
 */
//@{

void pmc_switch_sclk_to_32kxtal(uint32_t ul_bypass);
uint32_t pmc_osc_is_ready_32kxtal(void);

//@}

/**
 * \name Main Clock (MAINCK) oscillator and configuration
 *
 */
//@{

void pmc_switch_mainck_to_fastrc(uint32_t ul_moscrcf);
void pmc_osc_enable_fastrc(uint32_t ul_rc);
void pmc_osc_disable_fastrc(void);
uint32_t pmc_osc_is_ready_fastrc(void);
void pmc_osc_enable_main_xtal(uint32_t ul_xtal_startup_time);
void pmc_osc_bypass_main_xtal(void);
void pmc_osc_disable_main_xtal(void);
uint32_t pmc_osc_is_bypassed_main_xtal(void);
uint32_t pmc_osc_is_ready_main_xtal(void);
void pmc_switch_mainck_to_xtal(uint32_t ul_bypass,
		uint32_t ul_xtal_startup_time);
void pmc_osc_disable_xtal(uint32_t ul_bypass);
uint32_t pmc_osc_is_ready_mainck(void);
void pmc_mainck_osc_select(uint32_t ul_xtal_rc);

//@}

/**
 * \name PLL oscillator and configuration
 *
 */
//@{

void pmc_enable_pllack(uint32_t mula, uint32_t pllacount, uint32_t diva);
void pmc_disable_pllack(void);
uint32_t pmc_is_locked_pllack(void);

#if (SAM3S || SAM4S || SAM4C || SAM4CM || SAM4CP || SAMG55)
void pmc_enable_pllbck(uint32_t mulb, uint32_t pllbcount, uint32_t divb);
void pmc_disable_pllbck(void);
uint32_t pmc_is_locked_pllbck(void);
#endif

#if (SAM3XA || SAM3U || SAMV71 || SAMV70 || SAME70 || SAMS70)
void pmc_enable_upll_clock(void);
void pmc_disable_upll_clock(void);
uint32_t pmc_is_locked_upll(void);
#endif

//@}

/**
 * \name Peripherals clock configuration
 *
 */
//@{

uint32_t pmc_enable_periph_clk(uint32_t ul_id);
uint32_t pmc_disable_periph_clk(uint32_t ul_id);
void pmc_enable_all_periph_clk(void);
void pmc_disable_all_periph_clk(void);
uint32_t pmc_is_periph_clk_enabled(uint32_t ul_id);

//@}

/**
 * \name Programmable clock Source and Prescaler configuration
 *
 * The following functions may be used to select the clock source and
 * prescaler for the specified programmable clock.
 */
//@{

void pmc_pck_set_prescaler(uint32_t ul_id, uint32_t ul_pres);
void pmc_pck_set_source(uint32_t ul_id, uint32_t ul_source);
uint32_t pmc_switch_pck_to_sclk(uint32_t ul_id, uint32_t ul_pres);
uint32_t pmc_switch_pck_to_mainck(uint32_t ul_id, uint32_t ul_pres);
uint32_t pmc_switch_pck_to_pllack(uint32_t ul_id, uint32_t ul_pres);
#if (SAM4C || SAM4CM || SAM4CP)
void pmc_enable_cpck(void);
void pmc_disable_cpck(void);
bool pmc_is_cpck_enabled(void);
void pmc_enable_cpbmck(void);
void pmc_disable_cpbmck(void);
bool pmc_is_cpbmck_enabled(void);
void pmc_cpck_set_prescaler(uint32_t ul_pres);
void pmc_cpck_set_source(uint32_t ul_source);
#endif
#if (SAM3S || SAM4S || SAM4C || SAM4CM || SAM4CP || SAMG55)
uint32_t pmc_switch_pck_to_pllbck(uint32_t ul_id, uint32_t ul_pres);
#endif
#if (SAM3XA || SAM3U || SAMV71 || SAMV70 || SAME70 || SAMS70)
uint32_t pmc_switch_pck_to_upllck(uint32_t ul_id, uint32_t ul_pres);
#endif
uint32_t pmc_switch_pck_to_mck(uint32_t ul_id, uint32_t ul_pres);
void pmc_enable_pck(uint32_t ul_id);
void pmc_disable_pck(uint32_t ul_id);
void pmc_enable_all_pck(void);
void pmc_disable_all_pck(void);
uint32_t pmc_is_pck_enabled(uint32_t ul_id);

//@}

/**
 * \name USB clock configuration
 *
 */
//@{

#if (SAM3S || SAM3XA || SAM4S || SAM4E || SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
void pmc_switch_udpck_to_pllack(uint32_t ul_usbdiv);
#endif
#if (SAM3S || SAM4S || SAMG55)
void pmc_switch_udpck_to_pllbck(uint32_t ul_usbdiv);
#endif
#if (SAM3XA || SAMV71 || SAMV70 || SAME70 || SAMS70)
void pmc_switch_udpck_to_upllck(uint32_t ul_usbdiv);
#endif
#if (SAM3S || SAM3XA || SAM4S || SAM4E || SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
void pmc_enable_udpck(void);
void pmc_disable_udpck(void);
#endif
#if SAMG55
void pmc_switch_uhpck_to_pllack(uint32_t ul_usbdiv);
void pmc_switch_uhpck_to_pllbck(uint32_t ul_usbdiv);
void pmc_enable_uhpck(void);
#endif

//@}

/**
 * \name Interrupt and status management
 *
 */
//@{

void pmc_enable_interrupt(uint32_t ul_sources);
void pmc_disable_interrupt(uint32_t ul_sources);
uint32_t pmc_get_interrupt_mask(void);
uint32_t pmc_get_status(void);

//@}

/**
 * \name Power management
 *
 * The following functions are used to configure sleep mode and additional
 * wake up inputs.
 */
//@{

void pmc_set_fast_startup_input(uint32_t ul_inputs);
void pmc_clr_fast_startup_input(uint32_t ul_inputs);
#if (SAM4C || SAM4CM || SAM4CP)
void pmc_cp_set_fast_startup_input(uint32_t ul_inputs);
void pmc_cp_clr_fast_startup_input(uint32_t ul_inputs);
#endif
void pmc_enable_sleepmode(uint8_t uc_type);
void pmc_enable_waitmode(void);
#if (!(SAMG51 || SAMG53 || SAMG54))
void pmc_enable_backupmode(void);
#endif
//@}

/**
 * \name Failure detector
 *
 */
//@{

void pmc_enable_clock_failure_detector(void);
void pmc_disable_clock_failure_detector(void);

//@}

#if (SAM4N || SAM4C || SAM4CM || SAM4CP || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \name Slow Crystal Oscillator Frequency Monitoring
 *
 */
//@{

void pmc_enable_sclk_osc_freq_monitor(void);
void pmc_disable_sclk_osc_freq_monitor(void);

//@}
#endif

/**
 * \name Write protection
 *
 */
//@{

void pmc_set_writeprotect(uint32_t ul_enable);
uint32_t pmc_get_writeprotect_status(void);

//@}

#if (SAMG53 || SAMG54 || SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
/**
 * \name Sleepwalking configuration
 *
 */
//@{

uint32_t pmc_enable_sleepwalking(uint32_t ul_id);
uint32_t pmc_disable_sleepwalking(uint32_t ul_id);
uint32_t pmc_get_sleepwalking_status0(void);
uint32_t pmc_get_active_status0(void);
#if (SAMV71 || SAMV70 || SAME70 || SAMS70)
uint32_t pmc_get_sleepwalking_status1(void);
uint32_t pmc_get_active_status1(void);
#endif
//@}
#endif

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

//! @}

/**
 * \page sam_pmc_quickstart Quick start guide for the SAM PMC module
 *
 * This is the quick start guide for the \ref sam_drivers_pmc_group "PMC module",
 * with step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section pmc_use_cases PMC use cases
 * - \ref pmc_basic_use_case Basic use case - Switch Main Clock sources
 * - \ref pmc_use_case_2 Advanced use case - Configure Programmable Clocks
 *
 * \section pmc_basic_use_case Basic use case - Switch Main Clock sources
 * In this use case, the PMC module is configured for a variety of system clock
 * sources and speeds. A LED is used to visually indicate the current clock
 * speed as the source is switched.
 *
 * \section pmc_basic_use_case_setup Setup
 *
 * \subsection pmc_basic_use_case_setup_prereq Prerequisites
 * -# \ref gpio_group "General Purpose I/O Management (gpio)"
 *
 * \subsection pmc_basic_use_case_setup_code Code
 * The following function needs to be added to the user application, to flash a
 * board LED a variable number of times at a rate given in CPU ticks.
 *
 * \code
	 #define FLASH_TICK_COUNT   0x00012345

	 void flash_led(uint32_t tick_count, uint8_t flash_count)
	 {
	     SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
	     SysTick->LOAD = tick_count;

	     while (flash_count--)
	     {
	         gpio_toggle_pin(LED0_GPIO);
	         while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	         gpio_toggle_pin(LED0_GPIO);
	         while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	     }
	 }
\endcode
 *
 * \section pmc_basic_use_case_usage Use case
 *
 * \subsection pmc_basic_use_case_usage_code Example code
 * Add to application C-file:
 * \code
	for (;;)
	{
	    pmc_switch_mainck_to_fastrc(CKGR_MOR_MOSCRCF_12_MHz);
	    flash_led(FLASH_TICK_COUNT, 5);
	    pmc_switch_mainck_to_fastrc(CKGR_MOR_MOSCRCF_8_MHz);
	    flash_led(FLASH_TICK_COUNT, 5);
	    pmc_switch_mainck_to_fastrc(CKGR_MOR_MOSCRCF_4_MHz);
	    flash_led(FLASH_TICK_COUNT, 5);
	    pmc_switch_mainck_to_xtal(0);
	    flash_led(FLASH_TICK_COUNT, 5);
	}
\endcode
 *
 * \subsection pmc_basic_use_case_usage_flow Workflow
 * -# Wrap the code in an infinite loop:
 *   \code
	for (;;)
\endcode
 * -# Switch the Master CPU frequency to the internal 12MHz RC oscillator, flash
 *    a LED on the board several times:
 *   \code
	pmc_switch_mainck_to_fastrc(CKGR_MOR_MOSCRCF_12_MHz);
	flash_led(FLASH_TICK_COUNT, 5);
\endcode
 * -# Switch the Master CPU frequency to the internal 8MHz RC oscillator, flash
 *    a LED on the board several times:
 *   \code
	pmc_switch_mainck_to_fastrc(CKGR_MOR_MOSCRCF_8_MHz);
	flash_led(FLASH_TICK_COUNT, 5);
\endcode
 * -# Switch the Master CPU frequency to the internal 4MHz RC oscillator, flash
 *    a LED on the board several times:
 *   \code
	pmc_switch_mainck_to_fastrc(CKGR_MOR_MOSCRCF_4_MHz);
	flash_led(FLASH_TICK_COUNT, 5);
\endcode
 * -# Switch the Master CPU frequency to the external crystal oscillator, flash
 *    a LED on the board several times:
 *   \code
	pmc_switch_mainck_to_xtal(0, BOARD_OSC_STARTUP_US);
	flash_led(FLASH_TICK_COUNT, 5);
\endcode
 *
 * \section pmc_use_case_2 Use case #2 - Configure Programmable Clocks
 * In this use case, the PMC module is configured to start the Slow Clock from
 * an attached 32KHz crystal, and start one of the Programmable Clock modules
 * sourced from the Slow Clock divided down with a prescale factor of 64.
 *
 * \section pmc_use_case_2_setup Setup
 *
 * \subsection pmc_use_case_2_setup_prereq Prerequisites
 * -# \ref pio_group "Parallel Input/Output Controller (pio)"
 *
 * \subsection pmc_use_case_2_setup_code Code
 * The following code must be added to the user application:
 * \code
	pio_set_peripheral(PIOA, PIO_PERIPH_B, PIO_PA17);
\endcode
 *
 * \subsection pmc_use_case_2_setup_code_workflow Workflow
 * -# Configure the PCK1 pin to output on a specific port pin (in this case,
 *    PIOA pin 17) of the microcontroller.
 *   \code
	pio_set_peripheral(PIOA, PIO_PERIPH_B, PIO_PA17);
\endcode
 *   \note The peripheral selection and pin will vary according to your selected
 *       SAM device model. Refer to the "Peripheral Signal Multiplexing on I/O
 *       Lines" of your device's datasheet.
 *
 * \section pmc_use_case_2_usage Use case
 * The generated PCK1 clock output can be viewed on an oscilloscope attached to
 * the correct pin of the microcontroller.
 *
 * \subsection pmc_use_case_2_usage_code Example code
 * Add to application C-file:
 * \code
	  pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);
	  pmc_switch_pck_to_sclk(PMC_PCK_1, PMC_PCK_PRES_CLK_64);
	  pmc_enable_pck(PMC_PCK_1);

	  for (;;)
	  {
	      // Do Nothing
	  }
\endcode
 *
 * \subsection pmc_use_case_2_usage_flow Workflow
 * -# Switch the Slow Clock source input to an external 32KHz crystal:
 *   \code
	pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);
\endcode
 * -# Switch the Programmable Clock module PCK1 source clock to the Slow Clock,
 *    with a prescaler of 64:
 *   \code
	pmc_switch_pck_to_sclk(PMC_PCK_1, PMC_PCK_PRES_CLK_64);
\endcode
 * -# Enable Programmable Clock module PCK1:
 *   \code
	pmc_enable_pck(PMC_PCK_1);
\endcode
 * -# Enter an infinite loop:
 *   \code
	for (;;)
	{
	   // Do Nothing
	}
\endcode
 */

#endif /* PMC_H_INCLUDED */
