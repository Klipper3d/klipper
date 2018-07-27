/**
 * \file
 *
 * \brief Chip-specific system clock management functions
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

#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \page sysclk_quickstart Quick Start Guide for the System Clock Management service (SAM4L)
 *
 * This is the quick start guide for the \ref sysclk_group "System Clock Management"
 * service, with step-by-step instructions on how to configure and use the service for
 * specific use cases.
 *
 * \section sysclk_quickstart_usecases System Clock Management use cases
 * - \ref sysclk_quickstart_basic
 *
 * \section sysclk_quickstart_basic Basic usage of the System Clock Management service
 * This section will present a basic use case for the System Clock Management service.
 * This use case will configure the main system clock to 48MHz, using an internal DFLL
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
	   #define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_DFLL0

	   // Fdfll = (Fclk * DFLL_mul) / DFLL_div
	   #define CONFIG_DFLL0_SOURCE         GENCLK_SRC_OSC32K
	   #define CONFIG_DFLL0_FREQ           48000000UL
	   #define CONFIG_DFLL0_MUL            (CONFIG_DFLL0_FREQ / BOARD_OSC32_HZ)
	   #define CONFIG_DFLL0_DIV            1

	   // Fbus = Fsys / (2 ^ BUS_div)
	   #define CONFIG_SYSCLK_CPU_DIV       0
	   #define CONFIG_SYSCLK_PBA_DIV       1
	   #define CONFIG_SYSCLK_PBB_DIV       1
	   #define CONFIG_SYSCLK_PBC_DIV       1
	   #define CONFIG_SYSCLK_PBD_DIV       1
\endcode
 *
 * \subsection sysclk_quickstart_use_case_1_example_workflow Workflow
 *  -# Configure the main system clock to use the output of the DFLL0 module as its source:
 *   \code #define CONFIG_SYSCLK_SOURCE      SYSCLK_SRC_DFLL0 \endcode
 *  -# Configure the DFLL0 module to use external crystal oscillator OSC0 as its source:
 *   \code #define CONFIG_DFLL0_SOURCE       GENCLK_SRC_OSC32K \endcode
 *  -# Configure the DFLL0 module to multiply the external oscillator OSC0 frequency up to 48MHz:
 *   \code
	#define CONFIG_DFLL0_FREQ           48000000UL
	#define CONFIG_DFLL0_MUL            (CONFIG_DFLL0_FREQ / BOARD_OSC32_HZ)
	#define CONFIG_DFLL0_DIV            1
\endcode
 *   \note For user boards, \c BOARD_OSC0_HZ should be defined in the board \c conf_board.h configuration
 *         file as the frequency of the crystal attached to OSC0.
 *  -# Configure the main clock to run at the full 48MHz, scale the peripheral busses to run at one
 *     half (2 to the power of 1) of the system clock speed:
 *    \code
	#define CONFIG_SYSCLK_CPU_DIV       0
	#define CONFIG_SYSCLK_PBA_DIV       1
	#define CONFIG_SYSCLK_PBB_DIV       1
	#define CONFIG_SYSCLK_PBC_DIV       1
	#define CONFIG_SYSCLK_PBD_DIV       1
\endcode
 *    \note Some dividers are powers of two, while others are integer division factors. Refer to the
 *          formulas in the conf_clock.h template commented above each division define.
 */


/**
 * \weakgroup sysclk_group
 * @{
 */

//! \name System clock source
//@{
#define SYSCLK_SRC_RCSYS        0     //!< System RC oscillator
#define SYSCLK_SRC_OSC0         1     //!< Oscillator 0
#define SYSCLK_SRC_PLL0         2     //!< Phase Locked Loop 0
#define SYSCLK_SRC_DFLL         3     //!< Digital Frequency Locked Loop
#define SYSCLK_SRC_RC80M        4     //!< 80 MHz RC oscillator
#define SYSCLK_SRC_RCFAST       5     //!< 4-8-12 MHz RC oscillator
#define SYSCLK_SRC_RC1M         6     //!< 1 MHz RC oscillator
//@}

//! \name USB Clock Sources
//@{
#define USBCLK_SRC_OSC0         GENCLK_SRC_OSC0     //!< Use OSC0
#define USBCLK_SRC_PLL0         GENCLK_SRC_PLL0     //!< Use PLL0
#define USBCLK_SRC_DFLL         GENCLK_SRC_DFLL     //!< Use DFLL
#define USBCLK_SRC_GCLKIN0      GENCLK_SRC_GCLKIN0  //!< Use GCLKIN0
//@}

//! \name Bus index of maskable module clocks
//@{
#define PM_CLK_GRP_CPU               0
#define PM_CLK_GRP_HSB               1
#define PM_CLK_GRP_PBA               2
#define PM_CLK_GRP_PBB               3
#define PM_CLK_GRP_PBC               4
#define PM_CLK_GRP_PBD               5
//@}

//! \name Clocks derived from the CPU clock
//@{
//! On-Chip Debug system
#define SYSCLK_OCD              0
//@}

//! \name Clocks derived from the HSB clock
//@{
//! PDCA memory interface
#define SYSCLK_PDCA_HSB         0
//! Flash data interface
#define SYSCLK_HFLASHC_DATA     1
//! HRAMC data interface
#define SYSCLK_HRAMC1_DATA      2
//! USBC DMA and FIFO interface
#define SYSCLK_USBC_DATA        3
//! CRCCU data interface
#define SYSCLK_CRCCU_DATA       4
//! HSB<->PBA bridge
#define SYSCLK_PBA_BRIDGE       5
//! HSB<->PBB bridge
#define SYSCLK_PBB_BRIDGE       6
//! HSB<->PBC bridge
#define SYSCLK_PBC_BRIDGE       7
//! HSB<->PBD bridge
#define SYSCLK_PBD_BRIDGE       8
//! Advanced Encryption Standard
#define SYSCLK_AESA_HSB         9
//@}

//! \name Clocks derived from the PBA clock
//@{
//! IISC Controller
#define SYSCLK_IISC             0
//! SPI Controller
#define SYSCLK_SPI              1
//! Timer/Counter 0
#define SYSCLK_TC0              2
//! Timer/Counter 1
#define SYSCLK_TC1              3
//! TWI Master 0
#define SYSCLK_TWIM0            4
//! TWI Slave 0
#define SYSCLK_TWIS0            5
//! TWI Master 1
#define SYSCLK_TWIM1            6
//! TWI Slave 1
#define SYSCLK_TWIS1            7
//! USART 0
#define SYSCLK_USART0           8
//! USART 1
#define SYSCLK_USART1           9
//! USART 2
#define SYSCLK_USART2           10
//! USART 3
#define SYSCLK_USART3           11
//! A/D Converter
#define SYSCLK_ADCIFE           12
//! D/A Converter
#define SYSCLK_DACC             13
//! Analog Comparator
#define SYSCLK_ACIFC            14
//! Glue Logic Controller
#define SYSCLK_GLOC             15
//! ABDACB Controller
#define SYSCLK_ABDACB           16
//! TRNG Controller
#define SYSCLK_TRNG             17
//! PARC Controller
#define SYSCLK_PARC             18
//! Capacitive Touch
#define SYSCLK_CATB             19
//! TWI Master 2
#define SYSCLK_TWIM2            21
//! TWI Master 3
#define SYSCLK_TWIM3            22
//! LCD Controller
#define SYSCLK_LCDCA            23

//@}

//! \name Clocks derived from the PBB clock
//@{
//! Flash Controller registers
#define SYSCLK_HFLASHC_REGS     0
//! HRAMC Controller registers
#define SYSCLK_HRAMC1_REGS      1
//! HSB Matrix configuration
#define SYSCLK_HMATRIX          2
//! PDCA peripheral bus interface
#define SYSCLK_PDCA_PB          3
//! CRCCU registers
#define SYSCLK_CRCCU_REGS       4
//! USBC registers
#define SYSCLK_USBC_REGS        5
//! PEVC Controller
#define SYSCLK_PEVC             6
//@}

//! \name Clocks derived from the PBC clock
//@{
//! PM configuration
#define SYSCLK_PM               0
//! CHIPID Controller
#define SYSCLK_CHIPID           1
//! System Control Interface
#define SYSCLK_SCIF             2
//! Frequency Meter
#define SYSCLK_FREQM            3
//! General-Purpose I/O
#define SYSCLK_GPIO             4
//@}

//! \name Clocks derived from the PBD clock
//@{
//! BPM configuration
#define SYSCLK_BPM              0
//! BSCIF configuration
#define SYSCLK_BSCIF            1
//! Asynchronous Timer
#define SYSCLK_AST              2
//! Watchdog Timer
#define SYSCLK_WDT              3
//! External Interrupt Controller
#define SYSCLK_EIC              4
//! PICOUART
#define SYSCLK_PICOUART         5
//@}

//! \name Divided clock mask derived from the PBA clock
//@{
//! TIMER_CLOCK2 mask
#define PBA_DIVMASK_TIMER_CLOCK2     (1u << 0)
//! TIMER_CLOCK3 mask
#define PBA_DIVMASK_TIMER_CLOCK3     (1u << 2)
//! CLK_USART/DIV mask
#define PBA_DIVMASK_CLK_USART        (1u << 2)
//! TIMER_CLOCK4 mask
#define PBA_DIVMASK_TIMER_CLOCK4     (1u << 4)
//! TIMER_CLOCK5 mask
#define PBA_DIVMASK_TIMER_CLOCK5     (1u << 6)
//! Bitfield mask
#define PBA_DIVMASK_Msk              (0x7Fu << 0)
//@}

#ifndef __ASSEMBLY__

#include <compiler.h>
#include <dfll.h>
#include <osc.h>
#include <pll.h>
#include <genclk.h>

// Use the slow clock (RCOSC) with no prescaling if config was empty.
#ifndef CONFIG_SYSCLK_SOURCE
# define CONFIG_SYSCLK_SOURCE    SYSCLK_SRC_RCSYS
#endif /* CONFIG_SYSCLK_SOURCE */

/*
 * Enable PicoCache for flash access by default.
 * 0: disable PicoCache, 1: enable PicoCache.
 */
#ifndef CONFIG_HCACHE_ENABLE
#define CONFIG_HCACHE_ENABLE          1
#endif

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
 * \def CONFIG_SYSCLK_PBC_DIV
 * \brief Configuration symbol for dividing the PBC clock frequency by
 * \f$2^{CONFIG\_SYSCLK\_PBC\_DIV}\f$
 *
 * If this symbol is not defined, the PBC clock frequency is not divided.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifndef CONFIG_SYSCLK_PBC_DIV
# define CONFIG_SYSCLK_PBC_DIV   0
#endif /* CONFIG_SYSCLK_PBC_DIV */

/**
 * \def CONFIG_SYSCLK_PBD_DIV
 * \brief Configuration symbol for dividing the PBD clock frequency by
 * \f$2^{CONFIG\_SYSCLK\_PBD\_DIV}\f$
 *
 * If this symbol is not defined, the PBD clock frequency is not divided.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifndef CONFIG_SYSCLK_PBD_DIV
# define CONFIG_SYSCLK_PBD_DIV   0
#endif /* CONFIG_SYSCLK_PBD_DIV */

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
 * \def CONFIG_SYSCLK_INIT_PBCMASK
 * \brief Configuration symbol for the PBC clocks enabled at power-on after the
 * sysclock module has been initialized. By default, all PBC clocks are left
 * enabled, however to save power these can be automatically disabled by defining
 * this value to a mask of \c SYSCLOCK_xxx settings.
 *
 * If this symbol is not defined, then all PBC clocks are left enabled.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifdef __DOXYGEN__
# define CONFIG_SYSCLK_INIT_PBCMASK
#endif

/**
 * \def CONFIG_SYSCLK_INIT_PBDMASK
 * \brief Configuration symbol for the PBD clocks enabled at power-on after the
 * sysclock module has been initialized. By default, all PBD clocks are left
 * enabled, however to save power these can be automatically disabled by defining
 * this value to a mask of \c SYSCLOCK_xxx settings.
 *
 * If this symbol is not defined, then all PBD clocks are left enabled.
 *
 * This symbol may be defined in \ref conf_clock.h.
 */
#ifdef __DOXYGEN__
# define CONFIG_SYSCLK_INIT_PBDMASK
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
		return OSC_RCSYS_NOMINAL_HZ;
	}
#endif

	if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_RCSYS) {
		return OSC_RCSYS_NOMINAL_HZ;
	}

#ifdef BOARD_OSC0_HZ
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_OSC0) {
		return BOARD_OSC0_HZ;
	}
#endif

#ifdef CONFIG_PLL0_SOURCE
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_PLL0) {
		return pll_get_default_rate(0);
	}
#endif

#ifdef CONFIG_DFLL0_SOURCE
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_DFLL) {
		return dfll_get_default_rate(0);
	}
#endif

	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_RC80M) {
		return OSC_RC80M_NOMINAL_HZ;
	}

	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_RCFAST) {
		if (CONFIG_RCFAST_FRANGE == 2) {
			return OSC_RCFAST12M_NOMINAL_HZ;
		} else if (CONFIG_RCFAST_FRANGE == 1) {
			return OSC_RCFAST8M_NOMINAL_HZ;
		} else {
			return OSC_RCFAST4M_NOMINAL_HZ;
		}
	}

	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_RC1M) {
		return OSC_RC1M_NOMINAL_HZ;
	}


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
 * \brief Return the current rate in Hz of the Peripheral Bus C clock
 *
 * \return Frequency of the Peripheral Bus C clock, in Hz.
 */
static inline uint32_t sysclk_get_pbc_hz(void)
{
	return sysclk_get_main_hz() >> CONFIG_SYSCLK_PBC_DIV;
}

/**
 * \brief Return the current rate in Hz of the Peripheral Bus D clock
 *
 * \return Frequency of the Peripheral Bus D clock, in Hz.
 */
static inline uint32_t sysclk_get_pbd_hz(void)
{
	return sysclk_get_main_hz() >> CONFIG_SYSCLK_PBD_DIV;
}

extern uint32_t sysclk_get_peripheral_bus_hz(const volatile void *module);
//@}

extern void sysclk_priv_enable_module(uint32_t bus_id, uint32_t module_index);
extern void sysclk_priv_disable_module(uint32_t bus_id, uint32_t module_index);

//! \name Enabling and disabling synchronous clocks
//@{

/**
 * \brief Enable a module clock derived from the CPU clock
 * \param module_index Index of the module clock in the CPUMASK register
 */
static inline void sysclk_enable_cpu_module(uint32_t module_index)
{
	sysclk_priv_enable_module(PM_CLK_GRP_CPU, module_index);
}

/**
 * \brief Disable a module clock derived from the CPU clock
 * \param module_index Index of the module clock in the CPUMASK register
 */
static inline void sysclk_disable_cpu_module(uint32_t module_index)
{
	sysclk_priv_disable_module(PM_CLK_GRP_CPU, module_index);
}

/**
 * \brief Enable a module clock derived from the HSB clock
 * \param module_index Index of the module clock in the HSBMASK register
 */
static inline void sysclk_enable_hsb_module(uint32_t module_index)
{
	sysclk_priv_enable_module(PM_CLK_GRP_HSB, module_index);
}

/**
 * \brief Disable a module clock derived from the HSB clock
 * \param module_index Index of the module clock in the HSBMASK register
 */
static inline void sysclk_disable_hsb_module(uint32_t module_index)
{
	sysclk_priv_disable_module(PM_CLK_GRP_HSB, module_index);
}

extern void sysclk_enable_pba_module(uint32_t module_index);
extern void sysclk_disable_pba_module(uint32_t module_index);
extern void sysclk_enable_pbb_module(uint32_t module_index);
extern void sysclk_disable_pbb_module(uint32_t module_index);

/**
 * \brief Enable a module clock derived from the PBC clock
 * \param module_index Index of the module clock in the PBAMASK register
 */
static inline void sysclk_enable_pbc_module(uint32_t module_index)
{
	sysclk_priv_enable_module(PM_CLK_GRP_PBC, module_index);
}

/**
 * \brief Disable a module clock derived from the PBC clock
 * \param module_index Index of the module clock in the PBAMASK register
 */
static inline void sysclk_disable_pbc_module(uint32_t module_index)
{
	sysclk_priv_disable_module(PM_CLK_GRP_PBC, module_index);
}

/**
 * \brief Enable a module clock derived from the PBD clock
 * \param module_index Index of the module clock in the PBAMASK register
 */
static inline void sysclk_enable_pbd_module(uint32_t module_index)
{
	sysclk_priv_enable_module(PM_CLK_GRP_PBD, module_index);
}

/**
 * \brief Disable a module clock derived from the PBD clock
 * \param module_index Index of the module clock in the PBAMASK register
 */
static inline void sysclk_disable_pbd_module(uint32_t module_index)
{
	sysclk_priv_disable_module(PM_CLK_GRP_PBD, module_index);
}

/**
 * \brief Enable divided clock mask derived from the PBA clock
 * \param mask mask of the divided clock in the PBADIVMASK register
 */
static inline void sysclk_enable_pba_divmask(uint32_t mask)
{
	uint32_t temp_mask;

	temp_mask = PM->PM_PBADIVMASK;
	temp_mask |= mask;
	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
			| PM_UNLOCK_ADDR((uint32_t)&PM->PM_PBADIVMASK - (uint32_t)PM);
	PM->PM_PBADIVMASK = temp_mask;
}

/**
 * \brief Disable divided clock mask derived from the PBA clock
 * \param mask mask of the divided clock in the PBADIVMASK register
 */
static inline void sysclk_disable_pba_divmask(uint32_t mask)
{
	uint32_t temp_mask;

	temp_mask = PM->PM_PBADIVMASK;
	temp_mask &= ~mask;
	PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu)
			| PM_UNLOCK_ADDR((uint32_t)&PM->PM_PBADIVMASK - (uint32_t)PM);
	PM->PM_PBADIVMASK = temp_mask;
}

extern void sysclk_enable_peripheral_clock(const volatile void *module);
extern void sysclk_disable_peripheral_clock(const volatile void *module);

//@}

//! \name System Clock Source and Prescaler configuration
//@{

extern void sysclk_set_prescalers(uint32_t cpu_shift,
		uint32_t pba_shift, uint32_t pbb_shift,
		uint32_t pbc_shift, uint32_t pbd_shift);
extern void sysclk_set_source(uint32_t src);

//@}

#if defined(CONFIG_USBCLK_SOURCE) || defined(__DOXYGEN__)

/**
 * \def USBCLK_STARTUP_TIMEOUT
 * \brief Number of us to wait for USB clock to start
 */
#ifdef CONFIG_USBCLK_STARTUP_TIMEOUT
#   define USBCLK_STARTUP_TIMEOUT    (CONFIG_USBCLK_STARTUP_TIMEOUT)
#else
#   define USBCLK_STARTUP_TIMEOUT    (OSC0_STARTUP_TIMEOUT*(1000000/OSC_RCSYS_NOMINAL_HZ))
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
