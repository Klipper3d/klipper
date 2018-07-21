/**
 * \file
 *
 * \brief Chip-specific system clock management functions.
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

#ifndef CHIP_SYSCLK_H_INCLUDED
#define CHIP_SYSCLK_H_INCLUDED

#include <osc.h>
#include <pll.h>

/**
 * \page sysclk_quickstart Quick Start Guide for the System Clock Management
 * service (SAM4E)
 *
 * This is the quick start guide for the \ref sysclk_group "System Clock
 * Management" service, with step-by-step instructions on how to configure and
 * use the service for specific use cases.
 *
 * \section sysclk_quickstart_usecases System Clock Management use cases
 * - \ref sysclk_quickstart_basic
 * - \ref sysclk_quickstart_use_case_2
 *
 * \section sysclk_quickstart_basic Basic usage of the System Clock Management
 * service
 * This section will present a basic use case for the System Clock Management
 * service. This use case will configure the main system clock to 120MHz,
 * using an internal PLL module to multiply the frequency of a crystal attached
 * to the microcontroller.
 *
 * \subsection sysclk_quickstart_use_case_1_prereq Prerequisites
 *  - None
 *
 * \subsection sysclk_quickstart_use_case_1_setup_steps Initialization code
 * Add to the application initialization code:
 * \code
	sysclk_init();
\endcode
 *
 * \subsection sysclk_quickstart_use_case_1_setup_steps_workflow Workflow
 * -# Configure the system clocks according to the settings in conf_clock.h:
 *    \code sysclk_init(); \endcode
 *
 * \subsection sysclk_quickstart_use_case_1_example_code Example code
 *   Add or uncomment the following in your conf_clock.h header file,
 *   commenting out all other definitions of the same symbol(s):
 *   \code
	   #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLACK

	   // Fpll0 = (Fclk * PLL_mul) / PLL_div
	   #define CONFIG_PLL0_SOURCE          PLL_SRC_MAINCK_XTAL
	   #define CONFIG_PLL0_MUL             (120000000UL / BOARD_FREQ_MAINCK_XTAL)
	   #define CONFIG_PLL0_DIV             1

	   // Fbus = Fsys / BUS_div
	   #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_1
\endcode
 *
 * \subsection sysclk_quickstart_use_case_1_example_workflow Workflow
 *  -# Configure the main system clock to use the output of the PLL module as
 *     its source:
 *   \code #define CONFIG_SYSCLK_SOURCE          SYSCLK_SRC_PLLACK \endcode
 *  -# Configure the PLL module to use the fast external fast crystal
 *     oscillator as its source:
 *   \code #define CONFIG_PLL0_SOURCE            PLL_SRC_MAINCK_XTAL \endcode
 *  -# Configure the PLL module to multiply the external fast crystal
 *     oscillator frequency up to 120MHz:
 *   \code
	#define CONFIG_PLL0_MUL             (120000000UL / BOARD_FREQ_MAINCK_XTAL)
	#define CONFIG_PLL0_DIV             1
\endcode
 *   \note For user boards, \c BOARD_FREQ_MAINCK_XTAL should be defined in the
 *   board \c conf_board.h configuration
 *         file as the frequency of the fast crystal attached to the microcontroller.
 *  -# Configure the main clock to run at the full 120MHz, disable scaling of
 *     the main system clock speed:
 *    \code
	#define CONFIG_SYSCLK_PRES         SYSCLK_PRES_1
\endcode
 *    \note Some dividers are powers of two, while others are integer division
 *    factors. Refer to the formulas in the conf_clock.h template commented
 *    above each division define.
 */

/**
 * \page sysclk_quickstart_use_case_2 Advanced use case - Peripheral Bus Clock
 * Management (SAM4E)
 *
 * \section sysclk_quickstart_use_case_2 Advanced use case - Peripheral Bus
 * Clock Management
 * This section will present a more advanced use case for the System Clock
 * Management service. This use case will configure the main system clock to
 * 96MHz, using an internal PLL module to multiply the frequency of a crystal
 * attached to the microcontroller. The USB clock will be configured via the
 * same PLL module.
 *
 * \subsection sysclk_quickstart_use_case_2_prereq Prerequisites
 *  - None
 *
 * \subsection sysclk_quickstart_use_case_2_setup_steps Initialization code
 * Add to the application initialization code:
 * \code
	sysclk_init();
\endcode
 *
 * \subsection sysclk_quickstart_use_case_2_setup_steps_workflow Workflow
 * -# Configure the system clocks according to the settings in conf_clock.h:
 *    \code sysclk_init(); \endcode
 *
 * \subsection sysclk_quickstart_use_case_2_example_code Example code
 *   Add or uncomment the following in your conf_clock.h header file,
 *   commenting out all other definitions of the same symbol(s):
 *   \code
	   #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLACK

	   // Fpll0 = (Fclk * PLL_mul) / PLL_div
	   #define CONFIG_PLL0_SOURCE          PLL_SRC_MAINCK_XTAL
	   #define CONFIG_PLL0_MUL             (96000000UL / BOARD_FREQ_MAINCK_XTAL)
	   #define CONFIG_PLL0_DIV             1

	   // Fbus = Fsys / BUS_div
	   #define CONFIG_SYSCLK_PRES          SYSCLK_PRES_1

	   // Fusb = Fsys / USB_div
	   #define CONFIG_USBCLK_SOURCE        USBCLK_SRC_PLL0
	   #define CONFIG_USBCLK_DIV           2
\endcode
 *
 * \subsection sysclk_quickstart_use_case_2_example_workflow Workflow
 *  -# Configure the main system clock to use the output of the PLL0 module as
 *     its source:
 *   \code #define CONFIG_SYSCLK_SOURCE          SYSCLK_SRC_PLLACK \endcode
 *  -# Configure the PLL0 module to use the fast external fast crystal
 *     oscillator as its source:
 *   \code #define CONFIG_PLL0_SOURCE            PLL_SRC_MAINCK_XTAL \endcode
 *  -# Configure the PLL0 module to multiply the external fast crystal
 *     oscillator frequency up to 96MHz:
 *   \code
	#define CONFIG_PLL0_MUL             (96000000UL / BOARD_FREQ_MAINCK_XTAL)
	#define CONFIG_PLL0_DIV             1
\endcode
 *   \note For user boards, \c BOARD_FREQ_MAINCK_XTAL should be defined in the
 *   board \c conf_board.h configuration file as the frequency of the fast
 *   crystal attached to the microcontroller.
 *  -# Configure the main clock to run at the full 96MHz, disable scaling of
 *     the main system clock speed:
 *    \code
	#define CONFIG_SYSCLK_PRES         SYSCLK_PRES_1
\endcode
 *    \note Some dividers are powers of two, while others are integer division
 *    factors. Refer to the formulas in the conf_clock.h template commented
 *    above each division define.
 *  -# Configure the USB module clock to use the output of the PLL0 module as
 *     its source with division 2:
 *   \code
	#define CONFIG_USBCLK_SOURCE        USBCLK_SRC_PLL0
	#define CONFIG_USBCLK_DIV           2
\endcode
 */

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

//! \name Configuration Symbols
//@{
/**
 * \def CONFIG_SYSCLK_SOURCE
 * \brief Initial/static main system clock source
 *
 * The main system clock will be configured to use this clock during
 * initialization.
 */
#ifndef CONFIG_SYSCLK_SOURCE
# define CONFIG_SYSCLK_SOURCE   SYSCLK_SRC_MAINCK_4M_RC
#endif
/**
 * \def CONFIG_SYSCLK_PRES
 * \brief Initial CPU clock divider (mck)
 *
 * The MCK will run at
 * \f[
 *   f_{MCK} = \frac{f_{sys}}{\mathrm{CONFIG\_SYSCLK\_PRES}}\,\mbox{Hz}
 * \f]
 * after initialization.
 */
#ifndef CONFIG_SYSCLK_PRES
# define CONFIG_SYSCLK_PRES  0
#endif

//@}

//! \name Master Clock Sources (MCK)
//@{
#define SYSCLK_SRC_SLCK_RC         0 //!< Internal 32kHz RC oscillator as master source clock
#define SYSCLK_SRC_SLCK_XTAL       1 //!< External 32kHz crystal oscillator as master source clock
#define SYSCLK_SRC_SLCK_BYPASS     2 //!< External 32kHz bypass oscillator as master source clock
#define SYSCLK_SRC_MAINCK_4M_RC    3 //!< Internal 4MHz RC oscillator as master source clock
#define SYSCLK_SRC_MAINCK_8M_RC    4 //!< Internal 8MHz RC oscillator as master source clock
#define SYSCLK_SRC_MAINCK_12M_RC   5 //!< Internal 12MHz RC oscillator as master source clock
#define SYSCLK_SRC_MAINCK_XTAL     6 //!< External crystal oscillator as master source clock
#define SYSCLK_SRC_MAINCK_BYPASS   7 //!< External bypass oscillator as master source clock
#define SYSCLK_SRC_PLLACK          8 //!< Use PLLACK as master source clock
//@}

//! \name Master Clock Prescalers (MCK)
//@{
#define SYSCLK_PRES_1   PMC_MCKR_PRES_CLK_1  //!< Set master clock prescaler to 1
#define SYSCLK_PRES_2   PMC_MCKR_PRES_CLK_2  //!< Set master clock prescaler to 2
#define SYSCLK_PRES_4   PMC_MCKR_PRES_CLK_4  //!< Set master clock prescaler to 4
#define SYSCLK_PRES_8   PMC_MCKR_PRES_CLK_8  //!< Set master clock prescaler to 8
#define SYSCLK_PRES_16  PMC_MCKR_PRES_CLK_16 //!< Set master clock prescaler to 16
#define SYSCLK_PRES_32  PMC_MCKR_PRES_CLK_32 //!< Set master clock prescaler to 32
#define SYSCLK_PRES_64  PMC_MCKR_PRES_CLK_64 //!< Set master clock prescaler to 64
#define SYSCLK_PRES_3   PMC_MCKR_PRES_CLK_3  //!< Set master clock prescaler to 3
//@}

//! \name USB Clock Sources
//@{
#define USBCLK_SRC_PLL0       0     //!< Use PLLA
//@}

/**
 * \def CONFIG_USBCLK_SOURCE
 * \brief Configuration symbol for the USB generic clock source
 *
 * Sets the clock source to use for the USB. The source must also be properly
 * configured.
 *
 * Define this to one of the \c USBCLK_SRC_xxx settings. Leave it undefined if
 * USB is not required.
 */
#ifdef __DOXYGEN__
# define CONFIG_USBCLK_SOURCE
#endif

/**
 * \def CONFIG_USBCLK_DIV
 * \brief Configuration symbol for the USB generic clock divider setting
 *
 * Sets the clock division for the USB generic clock. If a USB clock source is
 * selected with CONFIG_USBCLK_SOURCE, this configuration symbol must also be
 * defined.
 */
#ifdef __DOXYGEN__
# define CONFIG_USBCLK_DIV
#endif

/**
 * \name Querying the system clock
 *
 * The following functions may be used to query the current frequency of
 * the system clock and the CPU and bus clocks derived from it.
 * sysclk_get_main_hz() and sysclk_get_cpu_hz() can be assumed to be
 * available on all platforms, although some platforms may define
 * additional accessors for various chip-internal bus clocks. These are
 * usually not intended to be queried directly by generic code.
 */
//@{

/**
 * \brief Return the current rate in Hz of the main system clock
 *
 * \todo This function assumes that the main clock source never changes
 * once it's been set up, and that PLL0 always runs at the compile-time
 * configured default rate. While this is probably the most common
 * configuration, which we want to support as a special case for
 * performance reasons, we will at some point need to support more
 * dynamic setups as well.
 */
#if (defined CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
extern uint32_t sysclk_initialized;
#endif
static inline uint32_t sysclk_get_main_hz(void)
{
#if (defined CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
	if (!sysclk_initialized ) {
		return OSC_MAINCK_4M_RC_HZ;
	}
#endif

	/* Config system clock setting */
	if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_RC) {
		return OSC_SLCK_32K_RC_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_XTAL) {
		return OSC_SLCK_32K_XTAL_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_BYPASS) {
		return OSC_SLCK_32K_BYPASS_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_4M_RC) {
		return OSC_MAINCK_4M_RC_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_8M_RC) {
		return OSC_MAINCK_8M_RC_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_12M_RC) {
		return OSC_MAINCK_12M_RC_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_XTAL) {
		return OSC_MAINCK_XTAL_HZ;
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_BYPASS) {
		return OSC_MAINCK_BYPASS_HZ;
	}
#ifdef CONFIG_PLL0_SOURCE
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_PLLACK) {
		return pll_get_default_rate(0);
	}
#endif

	else {
		/* unhandled_case(CONFIG_SYSCLK_SOURCE); */
		return 0;
	}
}

/**
 * \brief Return the current rate in Hz of the CPU clock
 *
 * \todo This function assumes that the CPU always runs at the system
 * clock frequency. We want to support at least two more scenarios:
 * Fixed CPU/bus clock dividers (config symbols) and dynamic CPU/bus
 * clock dividers (which may change at run time). Ditto for all the bus
 * clocks.
 *
 * \return Frequency of the CPU clock, in Hz.
 */
static inline uint32_t sysclk_get_cpu_hz(void)
{
	/* CONFIG_SYSCLK_PRES is the register value for setting the expected */
	/* prescaler, not an immediate value. */
	return sysclk_get_main_hz() /
		((CONFIG_SYSCLK_PRES == SYSCLK_PRES_3) ? 3 :
			(1 << (CONFIG_SYSCLK_PRES >> PMC_MCKR_PRES_Pos)));
}

/**
 * \brief Retrieves the current rate in Hz of the peripheral clocks.
 *
 * \return Frequency of the peripheral clocks, in Hz.
 */
static inline uint32_t sysclk_get_peripheral_hz(void)
{
	/* CONFIG_SYSCLK_PRES is the register value for setting the expected */
	/* prescaler, not an immediate value. */
	return sysclk_get_main_hz() /
		((CONFIG_SYSCLK_PRES == SYSCLK_PRES_3) ? 3 :
			(1 << (CONFIG_SYSCLK_PRES >> PMC_MCKR_PRES_Pos)));
}

/**
 * \brief Retrieves the current rate in Hz of the Peripheral Bus clock attached
 *        to the specified peripheral.
 *
 * \param module Pointer to the module's base address.
 *
 * \return Frequency of the bus attached to the specified peripheral, in Hz.
 */
static inline uint32_t sysclk_get_peripheral_bus_hz(const volatile void *module)
{
	UNUSED(module);
	return sysclk_get_peripheral_hz();
}
//@}

//! \name Enabling and disabling synchronous clocks
//@{

/**
 * \brief Enable a peripheral's clock.
 *
 * \param ul_id Id (number) of the peripheral clock.
 */
static inline void sysclk_enable_peripheral_clock(uint32_t ul_id)
{
	pmc_enable_periph_clk(ul_id);
}

/**
 * \brief Disable a peripheral's clock.
 *
 * \param ul_id Id (number) of the peripheral clock.
 */
static inline void sysclk_disable_peripheral_clock(uint32_t ul_id)
{
	pmc_disable_periph_clk(ul_id);
}

//@}

//! \name System Clock Source and Prescaler configuration
//@{

extern void sysclk_set_prescalers(uint32_t ul_pres);
extern void sysclk_set_source(uint32_t ul_src);

//@}

extern void sysclk_enable_usb(void);
extern void sysclk_disable_usb(void);

extern void sysclk_init(void);

//! @}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* CHIP_SYSCLK_H_INCLUDED */
