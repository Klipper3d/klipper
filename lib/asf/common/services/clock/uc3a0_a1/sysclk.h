/**
 * \file
 *
 * \brief Chip-specific system clock management functions
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
#ifndef CHIP_SYSCLK_H_INCLUDED
#define CHIP_SYSCLK_H_INCLUDED

#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \page sysclk_quickstart Quick Start Guide for the System Clock Management service (UC3A0/UC3A1)
 *
 * This is the quick start guide for the \ref sysclk_group "System Clock Management"
 * service, with step-by-step instructions on how to configure and use the service for
 * specific use cases.
 *
 * \section sysclk_quickstart_usecases System Clock Management use cases
 * - \ref sysclk_quickstart_basic
 * - \ref sysclk_quickstart_use_case_2
 *
 * \section sysclk_quickstart_basic Basic usage of the System Clock Management service
 * This section will present a basic use case for the System Clock Management service.
 * This use case will configure the main system clock to 48MHz, using an internal PLL
 * module to multiply the frequency of a crystal attached to the microcontroller. The
 * peripheral bus clocks are scaled down from the speed of the main system clock.
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
 *   Add or uncomment the following in your conf_clock.h header file, commenting out all other
 *   definitions of the same symbol(s):
 *   \code
	   #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLL0

	   // Fpll0 = (Fclk * PLL_mul) / PLL_div
	   #define CONFIG_PLL0_SOURCE          PLL_SRC_OSC0
	   #define CONFIG_PLL0_MUL             (48000000UL / BOARD_OSC0_HZ)
	   #define CONFIG_PLL0_DIV             1

	   // Fbus = Fsys / (2 ^ BUS_div)
	   #define CONFIG_SYSCLK_CPU_DIV       0
	   #define CONFIG_SYSCLK_PBA_DIV       1
	   #define CONFIG_SYSCLK_PBB_DIV       1
\endcode
 *
 * \subsection sysclk_quickstart_use_case_1_example_workflow Workflow
 *  -# Configure the main system clock to use the output of the PLL0 module as its source:
 *   \code #define CONFIG_SYSCLK_SOURCE      SYSCLK_SRC_PLL0 \endcode
 *  -# Configure the PLL0 module to use external crystal oscillator OSC0 as its source:
 *   \code #define CONFIG_PLL0_SOURCE        SYSCLK_SRC_OSC0 \endcode
 *  -# Configure the PLL0 module to multiply the external oscillator OSC0 frequency up to 48MHz:
 *   \code
	#define CONFIG_PLL0_MUL             (48000000UL / BOARD_OSC0_HZ)
	#define CONFIG_PLL0_DIV             1 
\endcode
 *   \note For user boards, \c BOARD_OSC0_HZ should be defined in the board \c conf_board.h configuration
 *         file as the frequency of the crystal attached to OSC0.
 *  -# Configure the main clock to run at the full 48MHz, scale the peripheral busses to run at one
 *     half (2 to the power of 1) of the system clock speed:
 *    \code
	#define CONFIG_SYSCLK_CPU_DIV       0
	#define CONFIG_SYSCLK_PBA_DIV       1
	#define CONFIG_SYSCLK_PBB_DIV       1
\endcode
 *    \note Some dividers are powers of two, while others are integer division factors. Refer to the
 *          formulas in the conf_clock.h template commented above each division define.
 */

/**
 * \page sysclk_quickstart_use_case_2 Advanced use case - Peripheral Bus Clock Management (UC3A0/UC3A1)
 *
 * \section sysclk_quickstart_use_case_2 Advanced use case - Peripheral Bus Clock Management
 * This section will present a more advanced use case for the System Clock Management service.
 * This use case will configure the main system clock to 48MHz, using an internal PLL
 * module to multiply the frequency of a crystal attached to the microcontroller. The peripheral bus
 * clocks will be divided down by a factor of two, and the USB clock will be configured via a
 * separate PLL module.
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
 *   Add or uncomment the following in your conf_clock.h header file, commenting out all other
 *   definitions of the same symbol(s):
 *   \code
	   #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLL0

	   // Fpll0 = (Fclk * PLL_mul) / PLL_div
	   #define CONFIG_PLL0_SOURCE          PLL_SRC_OSC0
	   #define CONFIG_PLL0_MUL             (48000000UL / BOARD_OSC0_HZ)
	   #define CONFIG_PLL0_DIV             1

	   // Fbus = Fsys / (2 ^ BUS_div)
	   #define CONFIG_SYSCLK_CPU_DIV       0
	   #define CONFIG_SYSCLK_PBA_DIV       1
	   #define CONFIG_SYSCLK_PBB_DIV       1

	   #define CONFIG_USBCLK_SOURCE        USBCLK_SRC_PLL1

	   // Fpll1 = (Fclk * PLL_mul) / PLL_div
	   #define CONFIG_PLL1_SOURCE          PLL_SRC_OSC0
	   #define CONFIG_PLL1_MUL             (48000000UL / BOARD_OSC0_HZ)
	   #define CONFIG_PLL1_DIV             1

	   // Fusb = Fsys / USB_div
	   #define CONFIG_USBCLK_DIV           1
\endcode
 *
 * \subsection sysclk_quickstart_use_case_2_example_workflow Workflow
 *  -# Configure the main system clock to use the output of the PLL0 module as its source:
 *   \code #define CONFIG_SYSCLK_SOURCE           SYSCLK_SRC_PLL0 \endcode
 *  -# Configure the PLL0 module to use external crystal oscillator OSC0 as its source:
 *   \code #define CONFIG_PLL0_SOURCE             SYSCLK_SRC_OSC0 \endcode
 *  -# Configure the PLL0 module to multiply the external oscillator OSC0 frequency up to 48MHz:
 *   \code
	#define CONFIG_PLL0_MUL              (48000000UL / BOARD_OSC0_HZ)
	#define CONFIG_PLL0_DIV              1 
\endcode
 *   \note For user boards, \c BOARD_OSC0_HZ should be defined in the board \c conf_board.h configuration
 *         file as the frequency of the crystal attached to OSC0.
 *  -# Configure the main clock to run at the full 48MHz, scale the peripheral busses to run at one
 *     half (2 to the power of 1) of the system clock speed:
 *    \code
	#define CONFIG_SYSCLK_CPU_DIV       0
	#define CONFIG_SYSCLK_PBA_DIV       1
	#define CONFIG_SYSCLK_PBB_DIV       1
\endcode
 *    \note Some dividers are powers of two, while others are integer division factors. Refer to the
 *          formulas in the conf_clock.h template commented above each division define.
 *  -# Configure the USB module clock to use the output of the PLL1 module as its source:
 *   \code #define CONFIG_USBCLK_SOURCE           USBCLK_SRC_PLL1 \endcode
 *  -# Configure the PLL1 module to use external crystal oscillator OSC0 as its source:
 *   \code #define CONFIG_PLL1_SOURCE             SYSCLK_SRC_OSC0 \endcode
 *  -# Configure the PLL1 module to multiply the external oscillator OSC0 frequency up to 48MHz:
 *   \code
	#define CONFIG_PLL1_MUL              (48000000UL / BOARD_OSC0_HZ)
	#define CONFIG_PLL1_DIV              1
\endcode
 *  -# Configure the USB module to perform no division on the input clock speed:
 *   \code #define CONFIG_USBCLK_DIV              1 \endcode
 */

/**
 * \weakgroup sysclk_group
 * @{
 */

//! \name System Clock Sources
//@{
#define SYSCLK_SRC_RCSYS    0    //!< Use slow clock as main clock
#define SYSCLK_SRC_OSC0     1    //!< Use OSC0 as main clock
#define SYSCLK_SRC_PLL0     2    //!< Use PLL0 as main clock
//@}

//! \name USB Clock Sources
//@{
#define USBCLK_SRC_OSC0         GENCLK_SRC_OSC0  //!< Use OSC0
#define USBCLK_SRC_PLL0         GENCLK_SRC_PLL0  //!< Use PLL0
#define USBCLK_SRC_PLL1         GENCLK_SRC_PLL1  //!< Use PLL1
//@}

//! \name Clocks derived from the CPU clock
//@{
//! On-Chip Debug system
#define SYSCLK_OCD          AVR32_OCD_CLK_CPU
//! COUNT/COMPARE system registers
#define SYSCLK_SYSTIMER     AVR32_CORE_CLK_CPU_COUNT
//@}

//! \name Clocks derived from the HSB clock
//@{
//! Flash data interface
#define SYSCLK_FLASHC_DATA  (AVR32_FLASHC_CLK_HSB % 32)
//! HSB<->PBA bridge
#define SYSCLK_PBA_BRIDGE   (AVR32_HMATRIX_CLK_HSB_PBA_BRIDGE % 32)
//! HSB<->PBB bridge
#define SYSCLK_PBB_BRIDGE   (AVR32_HMATRIX_CLK_HSB_PBB_BRIDGE % 32)
//! USBB DMA and FIFO interface
#define SYSCLK_USBB_DATA    (AVR32_USBB_CLK_HSB % 32)
//! MACB DMA interface
#define SYSCLK_MACB_DATA    (AVR32_MACB_CLK_HSB % 32)
//! PDCA memory interface
#define SYSCLK_PDCA_HSB     (AVR32_PDCA_CLK_HSB % 32)
//! External Bus Interface
#define SYSCLK_EBI          (AVR32_EBI_CLK_HSB % 32)
//@}

//! \name Clocks derived from the PBA clock
//@{
//! Internal interrupt controller
#define SYSCLK_INTC         (AVR32_INTC_CLK_PBA % 32)
//! General-Purpose I/O
#define SYSCLK_GPIO         (AVR32_GPIO_CLK_PBA % 32)
//! PDCA peripheral bus interface
#define SYSCLK_PDCA_PB      (AVR32_PDCA_CLK_PBA % 32)
//! PM/RTC/EIM configuration
#define SYSCLK_PM           (AVR32_PM_CLK_PBA % 32)
//! A/D Converter
#define SYSCLK_ADC          (AVR32_ADC_CLK_PBA % 32)
//! SPI Controller 0
#define SYSCLK_SPI0         (AVR32_SPI0_CLK_PBA % 32)
//! SPI Controller 1
#define SYSCLK_SPI1         (AVR32_SPI1_CLK_PBA % 32)
//! TWI Controller
#define SYSCLK_TWI          (AVR32_TWI_CLK_PBA % 32)
//! USART 0
#define SYSCLK_USART0       (AVR32_USART0_CLK_PBA % 32)
//! USART 1
#define SYSCLK_USART1       (AVR32_USART1_CLK_PBA % 32)
//! USART 2
#define SYSCLK_USART2       (AVR32_USART2_CLK_PBA % 32)
//! USART 3
#define SYSCLK_USART3       (AVR32_USART3_CLK_PBA % 32)
//! PWM
#define SYSCLK_PWM          (AVR32_PWM_CLK_PBA % 32)
//! Synchronous Serial Controller
#define SYSCLK_SSC          (AVR32_SSC_CLK_PBA % 32)
//! Timer/Counter
#define SYSCLK_TC           (AVR32_TC_CLK_PBA % 32)
//! D/A Converter
#define SYSCLK_DAC          (AVR32_ABDAC_CLK_PBA % 32)
//@}

//! \name Clocks derived from the PBB clock
//@{
//! HSB Matrix configuration
#define SYSCLK_HMATRIX      (AVR32_HMATRIX_CLK_PBB % 32)
//! USBB registers
#define SYSCLK_USBB_REGS    (AVR32_USBB_CLK_PBB % 32)
//! Flash Controller registers
#define SYSCLK_FLASHC_REGS  (AVR32_FLASHC_CLK_PBB % 32)
//! MACB Controller registers
#define SYSCLK_MACB_REGS    (AVR32_MACB_CLK_PBB % 32)
//! Static Memory Controller registers
#define SYSCLK_SMC_REGS     (AVR32_SMC_CLK_PBB % 32)
//! SDRAM Controller registers
#define SYSCLK_SDRAMC_REGS  (AVR32_SDRAMC_CLK_PBB % 32)
//@}

#ifndef __ASSEMBLY__

#include <avr32/io.h>
#include <osc.h>
#include <pll.h>
#include <genclk.h>

// Use the slow clock (RCOSC) with no prescaling if config was empty.
#ifndef CONFIG_SYSCLK_SOURCE
# define CONFIG_SYSCLK_SOURCE    SYSCLK_SRC_RCSYS
#endif /* CONFIG_SYSCLK_SOURCE */

/**
 * \def CONFIG_SYSCLK_CPU_DIV
 * \brief Configuration symbol for dividing the CPU clock frequency by
 * \f$2^{CONFIG\_SYSCLK\_CPU\_DIV}\f$
 *
 * If this symbol is not defined, the CPU clock frequency is not divided.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifndef CONFIG_SYSCLK_CPU_DIV
# define CONFIG_SYSCLK_CPU_DIV   0
#endif /* CONFIG_SYSCLK_CPU_DIV */

/**
 * \def CONFIG_SYSCLK_INIT_HSBMASK
 * \brief Configuration symbol for the HSB clocks enabled at power-on after the
 * sysclock module has been initialized. By default, all HSB clocks are left
 * enabled, however to save power these can be automatically disabled by defining
 * this value to a mask of \c SYSCLOCK_xxx settings.
 *
 * If this symbol is not defined, then all HSB clocks are left enabled.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifdef __DOXYGEN__
# define CONFIG_SYSCLK_INIT_HSBMASK
#endif

/**
 * \def CONFIG_SYSCLK_PBA_DIV
 * \brief Configuration symbol for dividing the PBA clock frequency by
 * \f$2^{CONFIG\_SYSCLK\_PBA\_DIV}\f$
 *
 * If this symbol is not defined, the PBA clock frequency is not divided.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifndef CONFIG_SYSCLK_PBA_DIV
# define CONFIG_SYSCLK_PBA_DIV   0
#endif /* CONFIG_SYSCLK_PBA_DIV */

/**
 * \def CONFIG_SYSCLK_PBB_DIV
 * \brief Configuration symbol for dividing the PBB clock frequency by
 * \f$2^{CONFIG\_SYSCLK\_PBB\_DIV}\f$
 *
 * If this symbol is not defined, the PBB clock frequency is not divided.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifndef CONFIG_SYSCLK_PBB_DIV
# define CONFIG_SYSCLK_PBB_DIV   0
#endif /* CONFIG_SYSCLK_PBB_DIV */

/**
 * \def CONFIG_SYSCLK_INIT_CPUMASK
 * \brief Configuration symbol for the CPU clocks enabled at power-on after the
 * sysclock module has been initialized. By default, all CPU clocks are left
 * enabled, however to save power these can be automatically disabled by defining
 * this value to a mask of \c SYSCLOCK_xxx settings.
 *
 * If this symbol is not defined, then all CPU clocks are left enabled.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifdef __DOXYGEN__
# define CONFIG_SYSCLK_INIT_CPUMASK
#endif

/**
 * \def CONFIG_SYSCLK_INIT_PBAMASK
 * \brief Configuration symbol for the PBA clocks enabled at power-on after the
 * sysclock module has been initialized. By default, all PBA clocks are left
 * enabled, however to save power these can be automatically disabled by defining
 * this value to a mask of \c SYSCLOCK_xxx settings.
 *
 * If this symbol is not defined, then all PBA clocks are left enabled.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifdef __DOXYGEN__
# define CONFIG_SYSCLK_INIT_PBAMASK
#endif

/**
 * \def CONFIG_SYSCLK_INIT_PBBMASK
 * \brief Configuration symbol for the PBB clocks enabled at power-on after the
 * sysclock module has been initialized. By default, all PBB clocks are left
 * enabled, however to save power these can be automatically disabled by defining
 * this value to a mask of \c SYSCLOCK_xxx settings.
 *
 * If this symbol is not defined, then all PBB clocks are left enabled.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifdef __DOXYGEN__
# define CONFIG_SYSCLK_INIT_PBBMASK
#endif

/**
 * \def CONFIG_SYSCLK_INIT_HSBMASK
 * \brief Configuration symbol for the HSB clocks enabled at power-on after the
 * sysclock module has been initialized. By default, all HSB clocks are left
 * enabled, however to save power these can be automatically disabled by defining
 * this value to a mask of \c SYSCLOCK_xxx settings.
 *
 * If this symbol is not defined, then all HSB clocks are left enabled.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifdef __DOXYGEN__
# define CONFIG_SYSCLK_INIT_HSBMASK
#endif

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
 *
 * Define this as any value that does not exceed \c GENCLK_DIV_MAX, and which
 * will give a 48 MHz clock frequency from the selected source.
 */
#ifdef __DOXYGEN__
# define CONFIG_USBCLK_DIV
#endif

/**
 * \name Querying the system clock and its derived clocks
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
extern bool sysclk_initialized;
#endif
static inline uint32_t sysclk_get_main_hz(void)
{
#if (defined CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
	if (!sysclk_initialized ) {
		return OSC_SLOW_NOMINAL_HZ;
	}
#endif

	switch (CONFIG_SYSCLK_SOURCE) {
	case SYSCLK_SRC_RCSYS:
		return OSC_SLOW_NOMINAL_HZ;

#ifdef BOARD_OSC0_HZ
	case SYSCLK_SRC_OSC0:
		return BOARD_OSC0_HZ;
#endif

#ifdef CONFIG_PLL0_SOURCE
	case SYSCLK_SRC_PLL0:
		return pll_get_default_rate(0);
#endif

	default:
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
	return sysclk_get_main_hz() >> CONFIG_SYSCLK_CPU_DIV;
}

/**
 * \brief Return the current rate in Hz of the High-Speed Bus clock
 *
 * \return Frequency of the High Speed Peripheral Bus clock, in Hz.
 */
static inline uint32_t sysclk_get_hsb_hz(void)
{
	return sysclk_get_main_hz() >> CONFIG_SYSCLK_CPU_DIV;
}

/**
 * \brief Return the current rate in Hz of the Peripheral Bus A clock
 *
 * \return Frequency of the Peripheral Bus A clock, in Hz.
 */
static inline uint32_t sysclk_get_pba_hz(void)
{
	return sysclk_get_main_hz() >> CONFIG_SYSCLK_PBA_DIV;
}

/**
 * \brief Return the current rate in Hz of the Peripheral Bus B clock
 *
 * \return Frequency of the Peripheral Bus B clock, in Hz.
 */
static inline uint32_t sysclk_get_pbb_hz(void)
{
	return sysclk_get_main_hz() >> CONFIG_SYSCLK_PBB_DIV;
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
	/* Fallthroughs intended for modules sharing the same peripheral bus. */
	switch ((uintptr_t)module) {
	case AVR32_INTC_ADDRESS:
	case AVR32_GPIO_ADDRESS:
	case AVR32_PDCA_ADDRESS:
	case AVR32_PM_ADDRESS:
	case AVR32_ADC_ADDRESS:
	case AVR32_SPI0_ADDRESS:
	case AVR32_SPI1_ADDRESS:
	case AVR32_TWI_ADDRESS:
	case AVR32_USART0_ADDRESS:
	case AVR32_USART1_ADDRESS:
	case AVR32_USART2_ADDRESS:
	case AVR32_USART3_ADDRESS:
	case AVR32_PWM_ADDRESS:
	case AVR32_SSC_ADDRESS:
	case AVR32_TC_ADDRESS:
	case AVR32_ABDAC_ADDRESS:
		return sysclk_get_pba_hz();

	case AVR32_HMATRIX_ADDRESS:
	case AVR32_USBB_ADDRESS:
	case AVR32_FLASHC_ADDRESS:
	case AVR32_MACB_ADDRESS:
#if defined(AVR32_SMC_ADDRESS)
	case AVR32_SMC_ADDRESS:
#endif
#if defined(AVR32_SDRAMC_ADDRESS)
	case AVR32_SDRAMC_ADDRESS:
#endif
		return sysclk_get_pbb_hz();

	default:
		Assert(false);
		return 0;
	}
}

//@}

extern void sysclk_priv_enable_module(unsigned int bus_id,
		unsigned int module_index);
extern void sysclk_priv_disable_module(unsigned int bus_id,
		unsigned int module_index);

//! \name Enabling and disabling synchronous clocks
//@{

/**
 * \brief Enable a module clock derived from the CPU clock
 * \param index Index of the module clock in the CPUMASK register
 */
static inline void sysclk_enable_cpu_module(unsigned int index)
{
	sysclk_priv_enable_module(AVR32_PM_CLK_GRP_CPU, index);
}

/**
 * \brief Disable a module clock derived from the CPU clock
 * \param index Index of the module clock in the CPUMASK register
 */
static inline void sysclk_disable_cpu_module(unsigned int index)
{
	sysclk_priv_disable_module(AVR32_PM_CLK_GRP_CPU, index);
}

/**
 * \brief Enable a module clock derived from the HSB clock
 * \param index Index of the module clock in the HSBMASK register
 */
static inline void sysclk_enable_hsb_module(unsigned int index)
{
	sysclk_priv_enable_module(AVR32_PM_CLK_GRP_HSB, index);
}

/**
 * \brief Disable a module clock derived from the HSB clock
 * \param index Index of the module clock in the HSBMASK register
 */
static inline void sysclk_disable_hsb_module(unsigned int index)
{
	sysclk_priv_disable_module(AVR32_PM_CLK_GRP_HSB, index);
}

/**
 * \brief Enable a module clock derived from the PBA clock
 * \param index Index of the module clock in the PBAMASK register
 */
static inline void sysclk_enable_pba_module(unsigned int index)
{
	sysclk_priv_enable_module(AVR32_PM_CLK_GRP_PBA, index);
}

/**
 * \brief Disable a module clock derived from the PBA clock
 * \param index Index of the module clock in the PBAMASK register
 */
static inline void sysclk_disable_pba_module(unsigned int index)
{
	sysclk_priv_disable_module(AVR32_PM_CLK_GRP_PBA, index);
}

extern void sysclk_enable_pbb_module(unsigned int index);
extern void sysclk_disable_pbb_module(unsigned int index);

/**
 * \brief Enable a peripheral's clock from its base address.
 *
 *  Enables the clock to a peripheral, given its base address. If the peripheral
 *  has an associated clock on the HSB bus, this will be enabled also.
 *
 * \param module Pointer to the module's base address.
 */
static inline void sysclk_enable_peripheral_clock(const volatile void *module)
{
	switch ((uintptr_t)module) {
	case AVR32_INTC_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_INTC);
		break;

	case AVR32_GPIO_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_GPIO);
		break;

	case AVR32_PDCA_ADDRESS:
		sysclk_enable_hsb_module(SYSCLK_PDCA_HSB);
		sysclk_enable_pba_module(SYSCLK_PDCA_PB);
		break;

	case AVR32_PM_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_PM);
		break;

	case AVR32_ADC_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_ADC);
		break;

	case AVR32_SPI0_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_SPI0);
		break;

	case AVR32_SPI1_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_SPI1);
		break;

	case AVR32_TWI_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_TWI);
		break;

	case AVR32_USART0_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_USART0);
		break;

	case AVR32_USART1_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_USART1);
		break;

	case AVR32_USART2_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_USART2);
		break;

	case AVR32_USART3_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_USART3);
		break;

	case AVR32_PWM_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_PWM);
		break;

	case AVR32_SSC_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_SSC);
		break;

	case AVR32_TC_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_TC);
		break;

	case AVR32_ABDAC_ADDRESS:
		sysclk_enable_pba_module(SYSCLK_DAC);
		break;

	case AVR32_HMATRIX_ADDRESS:
		sysclk_enable_pbb_module(SYSCLK_HMATRIX);
		break;

	case AVR32_USBB_ADDRESS:
		sysclk_enable_hsb_module(SYSCLK_USBB_DATA);
		sysclk_enable_pbb_module(SYSCLK_USBB_REGS);
		break;

	case AVR32_FLASHC_ADDRESS:
		sysclk_enable_hsb_module(SYSCLK_FLASHC_DATA);
		sysclk_enable_pbb_module(SYSCLK_FLASHC_REGS);
		break;

	case AVR32_MACB_ADDRESS:
		sysclk_enable_hsb_module(SYSCLK_MACB_DATA);
		sysclk_enable_pbb_module(SYSCLK_MACB_REGS);
		break;

#if defined(AVR32_SMC_ADDRESS)
	case AVR32_SMC_ADDRESS:
		sysclk_enable_pbb_module(SYSCLK_SMC_REGS);
		break;
#endif

#if defined(AVR32_SDRAMC_ADDRESS)
	case AVR32_SDRAMC_ADDRESS:
		sysclk_enable_pbb_module(SYSCLK_SDRAMC_REGS);
		break;
#endif

	default:
		Assert(false);
		return;
	}
}

/**
 * \brief Disable a peripheral's clock from its base address.
 *
 *  Disables the clock to a peripheral, given its base address. If the peripheral
 *  has an associated clock on the HSB bus, this will be disabled also.
 *
 * \param module Pointer to the module's base address.
 */
static inline void sysclk_disable_peripheral_clock(const volatile void *module)
{
	switch ((uintptr_t)module) {
	case AVR32_INTC_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_INTC);
		break;

	case AVR32_GPIO_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_GPIO);
		break;

	case AVR32_PDCA_ADDRESS:
		sysclk_disable_hsb_module(SYSCLK_PDCA_HSB);
		sysclk_disable_pba_module(SYSCLK_PDCA_PB);
		break;

	case AVR32_PM_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_PM);
		break;

	case AVR32_ADC_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_ADC);
		break;

	case AVR32_SPI0_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_SPI0);
		break;

	case AVR32_SPI1_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_SPI1);
		break;

	case AVR32_TWI_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_TWI);
		break;

	case AVR32_USART0_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_USART0);
		break;

	case AVR32_USART1_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_USART1);
		break;

	case AVR32_USART2_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_USART2);
		break;

	case AVR32_USART3_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_USART3);
		break;

	case AVR32_PWM_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_PWM);
		break;

	case AVR32_SSC_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_SSC);
		break;

	case AVR32_TC_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_TC);
		break;

	case AVR32_ABDAC_ADDRESS:
		sysclk_disable_pba_module(SYSCLK_DAC);
		break;

	case AVR32_HMATRIX_ADDRESS:
		sysclk_disable_pbb_module(SYSCLK_HMATRIX);
		break;

	case AVR32_USBB_ADDRESS:
		sysclk_disable_hsb_module(SYSCLK_USBB_DATA);
		sysclk_disable_pbb_module(SYSCLK_USBB_REGS);
		break;

	case AVR32_FLASHC_ADDRESS:
		sysclk_disable_hsb_module(SYSCLK_FLASHC_DATA);
		sysclk_disable_pbb_module(SYSCLK_FLASHC_REGS);
		break;

	case AVR32_MACB_ADDRESS:
		sysclk_disable_hsb_module(SYSCLK_MACB_DATA);
		sysclk_disable_pbb_module(SYSCLK_MACB_REGS);
		break;

#if defined(AVR32_SMC_ADDRESS)
	case AVR32_SMC_ADDRESS:
		sysclk_disable_pbb_module(SYSCLK_SMC_REGS);
		break;
#endif

#if defined(AVR32_SDRAMC_ADDRESS)
	case AVR32_SDRAMC_ADDRESS:
		sysclk_disable_pbb_module(SYSCLK_SDRAMC_REGS);
		break;
#endif

	default:
		Assert(false);
		return;
	}
}

//@}

//! \name System Clock Source and Prescaler configuration
//@{

/**
 * \brief Set system clock prescaler configuration
 *
 * This function will change the system clock prescaler configuration to
 * match the parameters.
 *
 * \note The parameters to this function are device-specific.
 *
 * \param cpu_shift The CPU clock will be divided by \f$2^{cpu\_shift}\f$
 * \param pba_shift The PBA clock will be divided by \f$2^{pba\_shift}\f$
 * \param pbb_shift The PBB clock will be divided by \f$2^{pbb\_shift}\f$
 */
static inline void sysclk_set_prescalers(unsigned int cpu_shift,
		unsigned int pba_shift, unsigned int pbb_shift)
{
	uint32_t cksel = 0;

	Assert(cpu_shift <= pba_shift);
	Assert(cpu_shift <= pbb_shift);

	if (cpu_shift > 0)
		cksel = ((cpu_shift - 1) << AVR32_PM_CKSEL_CPUSEL)
				| (1U << AVR32_PM_CKSEL_CPUDIV);

	if (pba_shift > 0)
		cksel |= ((pba_shift - 1) << AVR32_PM_CKSEL_PBASEL)
				| (1U << AVR32_PM_CKSEL_PBADIV);

	if (pbb_shift > 0)
		cksel |= ((pbb_shift - 1) << AVR32_PM_CKSEL_PBBSEL)
				| (1U << AVR32_PM_CKSEL_PBBDIV);

	AVR32_PM.cksel = cksel;
}

/**
 * \brief Change the source of the main system clock.
 *
 * \pre The appropriate Flash Wait state must be set previously.
 *
 * \param src The new system clock source. Must be one of the constants
 * from the <em>System Clock Sources</em> section.
 */
static inline void sysclk_set_source(uint_fast8_t src)
{
	irqflags_t flags;
	uint32_t   mcctrl;

	Assert(src <= SYSCLK_SRC_PLL0);

	flags = cpu_irq_save();
	mcctrl = AVR32_PM.mcctrl & ~AVR32_PM_MCCTRL_MCSEL_MASK;
	mcctrl |= src << AVR32_PM_MCCTRL_MCSEL;
	AVR32_PM.mcctrl = mcctrl;
	cpu_irq_restore(flags);
}

//@}

#if defined(CONFIG_USBCLK_SOURCE) || defined(__DOXYGEN__)

/**
 * \def USBCLK_STARTUP_TIMEOUT
 * \brief Number of us to wait for USB clock to start
 */
#if CONFIG_USBCLK_SOURCE==USBCLK_SRC_OSC0
#  define USBCLK_STARTUP_TIMEOUT    (OSC0_STARTUP_TIMEOUT*(1000000/AVR32_PM_RCOSC_FREQUENCY))
#elif CONFIG_USBCLK_SOURCE==USBCLK_SRC_PLL0
#  if CONFIG_PLL0_SOURCE==PLL_SRC_OSC0
#    define USBCLK_STARTUP_TIMEOUT    (OSC0_STARTUP_TIMEOUT*(1000000/AVR32_PM_RCOSC_FREQUENCY))
#  elif CONFIG_PLL0_SOURCE==PLL_SRC_OSC1
#    define USBCLK_STARTUP_TIMEOUT    (OSC1_STARTUP_TIMEOUT*(1000000/AVR32_PM_RCOSC_FREQUENCY))
#  else
#    error Unknown value for CONFIG_PLL0_SOURCE, see conf_clock.h.
#  endif
#elif CONFIG_USBCLK_SOURCE==USBCLK_SRC_PLL1
#  if CONFIG_PLL1_SOURCE==PLL_SRC_OSC0
#    define USBCLK_STARTUP_TIMEOUT    (OSC0_STARTUP_TIMEOUT*(1000000/AVR32_PM_RCOSC_FREQUENCY))
#  elif CONFIG_PLL1_SOURCE==PLL_SRC_OSC1
#    define USBCLK_STARTUP_TIMEOUT    (OSC1_STARTUP_TIMEOUT*(1000000/AVR32_PM_RCOSC_FREQUENCY))
#  else
#    error Unknown value for CONFIG_PLL1_SOURCE, see conf_clock.h.
#  endif
#else
#  error Unknown value for CONFIG_USBCLK_SOURCE, see conf_clock.h.
#endif

extern void sysclk_enable_usb(void);
extern void sysclk_disable_usb(void);
#endif

extern void sysclk_init(void);

#endif /* !__ASSEMBLY__ */

//! @}

#ifdef __cplusplus
}
#endif

#endif /* CHIP_SYSCLK_H_INCLUDED */
